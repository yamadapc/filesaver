//
// Created by Pedro Tacla Yamada on 2/7/20.
//

#include <array>
#include <boost/format.hpp>
#include <math.h>

#include "Utils.h"

namespace filesaver::utils
{

std::string prettyPrintBytes (off_t bytes)
{
    const std::array<std::string, 7> suffixes{
        "B",
        "KB",
        "MB",
        "GB",
        "TB",
        "PB",
        "EB",
    };
    uint suffixIndex = 0;
    auto count = static_cast<double> (bytes);
    while (count >= 1024 && suffixIndex < 7)
    {
        suffixIndex++;
        count /= 1024;
    }

    if (count - floor (count) == 0.0)
    {
        return (boost::format ("%d%s") % count % suffixes[suffixIndex]).str ();
    }

    return (boost::format ("%.1f%s") % count % suffixes[suffixIndex]).str ();
}

} // namespace filesaver::utils
