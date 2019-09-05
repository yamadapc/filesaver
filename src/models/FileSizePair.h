//
// Created by Pedro Tacla Yamada on 2019-09-05.
//

#ifndef FILESAVER_FILESIZEPAIR_H
#define FILESAVER_FILESIZEPAIR_H

#include <string>

namespace filesaver {

class FileSizePair {
public:
  FileSizePair(std::string filename, off_t size);

  const std::string &getFilename();
  const off_t getSize();

private:
  std::string filename;
  off_t size;
};

} // namespace filesaver

#endif // FILESAVER_FILESIZEPAIR_H
