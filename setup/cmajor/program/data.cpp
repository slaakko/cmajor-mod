#include "data.hpp"

namespace setup {

std::string PackageResourceName()
{
    return "cmajor.package";
}

int64_t UncompressedPackageSize()
{
    return 2500936453;
}

std::string UnicodeDBResourceName()
{
    return "wng_unicode_db";
}

int64_t UncompressedUnicodeDBSize()
{
    return 53233116;
}

std::string SetupIconResourceName()
{
    return "setup_icon";
}

std::string AppName()
{
    return "Cmajor";
}

std::string AppVersion()
{
    return "5.0.0";
}

int Compression()
{
    return 1; // deflate
}

std::string DefaultContainingDirPath()
{
    return "C:/";
}

std::string InstallDirName()
{
    return "cmajor";
}


} // setup

