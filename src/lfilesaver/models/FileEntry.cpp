//
// Created by Pedro Tacla Yamada on 2019-08-20.
//

#include "FileEntry.h"

#include <utility>

namespace filesaver
{

FileEntry::FileEntry (FileType _type, off_t _size, uintmax_t _dev, uintmax_t _ino, std::string _filename)
    : dev (_dev), ino (_ino), type (_type), size (_size), filepath (_filename), isFinished (!isDirectory ())
{
}

FileEntry::FileEntry (FileType _type, off_t _size, uintmax_t _dev, uintmax_t _ino, boost::filesystem::path _filepath)
    : dev (_dev), ino (_ino), type (_type), size (_size), filepath (std::move(_filepath)), isFinished (!isDirectory ())
{
}

const std::vector<boost::filesystem::path>& FileEntry::children ()
{
    if (hasCachedChildren)
    {
        return cachedChildren;
    }

    if (type != FileType::directory)
    {
        hasCachedChildren = true;
        return cachedChildren;
    }

    auto* dir = opendir (filepath.string ().c_str ());

    if (dir == nullptr)
    {
        hasCachedChildren = true;
        return cachedChildren;
    }

    auto* ent = readdir (dir);
    if (ent == nullptr)
    {
        closedir (dir);
        hasCachedChildren = true;
        return cachedChildren;
    }

    {
        std::vector<boost::filesystem::path> childPaths;

        do
        {
            std::string file{ent->d_name};
            if (file != "." && file != "..")
            {
                boost::filesystem::path path{filepath};
                path.append (file);
                childPaths.push_back (path);
            }
        } while ((ent = readdir (dir)) != nullptr);

        closedir (dir);
        free (ent);

        cachedChildren = childPaths;
        hasCachedChildren = true;
        return cachedChildren;
    }
}

std::shared_ptr<FileEntry> FileEntry::fromPath (const boost::filesystem::path& filepath)
{
    struct stat buffer;
    int result = lstat (filepath.string ().c_str (), &buffer);

    if (result != 0)
    {
        return std::make_shared<FileEntry> (FileType::unknown, 0, 0, 0, filepath);
    }

    FileType type;
    mode_t mode = buffer.st_mode;

    if (S_ISDIR (mode))
    {
        type = FileType::directory;
    }
    else if (S_ISREG (mode))
    {
        type = FileType::regular_file;
    }
    else if (S_ISLNK (mode))
    {
        type = FileType ::symbolic_link;
    }
    else if (S_ISFIFO (mode))
    {
        type = FileType ::fifo_special;
    }
    else if (S_ISBLK (mode))
    {
        type = FileType::block_special;
    }
    else if (S_ISCHR (mode))
    {
        type = FileType::character_special;
    }
    else
    {
        type = FileType::unknown;
    }

    auto fileEntry = std::make_shared<FileEntry> (type, buffer.st_size, buffer.st_dev, buffer.st_ino, filepath);

    return fileEntry;
}

bool FileEntry::operator== (const FileEntry& rhs) const
{
    return dev == rhs.dev && ino == rhs.ino && type == rhs.type && size == rhs.size && filepath == rhs.filepath;
}

bool FileEntry::operator!= (const FileEntry& rhs) const
{
    return !(rhs == *this);
}

bool FileEntry::isDirectory () const
{
    return type == FileType::directory;
}

bool FileEntry::getHasCachedChildren () const
{
    return hasCachedChildren;
}

FileSizePair FileEntry::toPair () const
{
    return {filepath.string (), size};
}

} // namespace filesaver
