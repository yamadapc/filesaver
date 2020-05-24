//
// Created by Pedro Tacla Yamada on 24/5/20.
//

#include "TestUtils.h"

std::string filesaver::test::randomString (int size)
{
    static std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    std::uniform_int_distribution indexDistribution (0, static_cast<int> (chars.size ()));
    std::random_device randomDevice;

    std::string result;
    for (int i = 0; i < size; i++)
    {
        auto c = indexDistribution (randomDevice);
        result.push_back (chars[static_cast<size_t> (c)]);
    }
    return result;
}

boost::filesystem::path filesaver::test::getRandomTemporaryPath ()
{
    auto randomStr = randomString (10);
    return boost::filesystem::temp_directory_path ().append (randomStr);
}
