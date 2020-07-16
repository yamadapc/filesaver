//
// Created by Pedro Tacla Yamada on 11/7/20.
//

#include <fstream>
#include <spdlog/spdlog.h>

#include "RandomRootGenerator.h"
#include "TestUtils.h"

namespace filesaver::test
{

void generateLayer (boost::filesystem::path root, int fileSizeBytes, int filesInLayer, int currentDepth, int layerSize)
{
    if (currentDepth <= 0)
    {
        return;
    }

    spdlog::info ("Generating layer root={} depth={}", root.string (), currentDepth);

    for (int childDir = 0; childDir < layerSize; childDir++)
    {
        boost::filesystem::path childPath{root};
        childPath.append (randomString (4));
        spdlog::info ("Creating directory path={}", childPath.string ());
        boost::filesystem::create_directories (childPath);
        generateLayer (childPath, fileSizeBytes, filesInLayer, currentDepth - 1, layerSize);
    }
}

void RandomRootGenerator::generateRootAt (boost::filesystem::path path)
{
    int fileSizeBytes = 10;
    int filesInLayer = 10;
    int depth = 3;
    int layerSize = 2;

    generateLayer (path, fileSizeBytes, filesInLayer, depth, layerSize);
}

} // namespace filesaver::test
