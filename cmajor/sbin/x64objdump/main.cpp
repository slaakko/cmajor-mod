import std.core;
import util;
import cmajor.sbin.coff;

void DumpObjectFile(const std::string& objFilePath, cmajor::sbin::coff::DumpFlags dumpFlags, bool verbose)
{
    if (verbose)
    {
        std::cout << "> " << objFilePath << "\n";
    }
    cmajor::sbin::coff::ReadFlags readFlags = cmajor::sbin::coff::ReadFlags::none;
    if ((dumpFlags & cmajor::sbin::coff::DumpFlags::dumpRelocations) != cmajor::sbin::coff::DumpFlags::none)
    {
        readFlags = readFlags | cmajor::sbin::coff::ReadFlags::readRelocations;
    }
    if ((dumpFlags & cmajor::sbin::coff::DumpFlags::dumpSectionData) != cmajor::sbin::coff::DumpFlags::none)
    {
        readFlags = readFlags | cmajor::sbin::coff::ReadFlags::readSectionData;
    }
    cmajor::sbin::coff::CoffObjectFile objFile(objFilePath, readFlags);
    std::string txtFilePath = objFilePath + ".txt";
    std::ofstream txtFile(txtFilePath);
    util::CodeFormatter formatter(txtFile);
    objFile.Dump(formatter, dumpFlags);
    if (verbose)
    {
        std::cout << "==> " << txtFilePath << "\n";
    }
}

std::string Version()
{
    return "5.1.0";
}

void PrintHelp()
{
    std::cout << "x64 object file dump version " << Version() << "\n";
    std::cout << "usage: x64objdump [options] { FILE.obj }" << "\n";
    std::cout << "options" << "\n";
    std::cout << "--help | -h" << "\n";
    std::cout << "  Print help and exit." << "\n";
    std::cout << "--verbose | -v" << "\n";
    std::cout << "  Be verbose." << "\n";
    std::cout << "--relocations | -r" << "\n";
    std::cout << "  Dump relocations." << "\n";
    std::cout << "--data | -d" << "\n";
    std::cout << "  Dump section data." << "\n";
    std::cout << "--all | -a" << "\n";
    std::cout << "  Dump relocations and section data." << "\n";
}

int main(int argc, const char** argv)
{
    try
    {
        util::Init();
        bool verbose = false;
        std::vector<std::string> files;
        cmajor::sbin::coff::DumpFlags dumpFlags = cmajor::sbin::coff::DumpFlags::none;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.starts_with("--"))
            {
                if (arg == "--help")
                {
                    PrintHelp();
                    return 0;
                }
                else if (arg == "--verbose")
                {
                    verbose = true;
                }
                else if (arg == "--relocations")
                {
                    dumpFlags = dumpFlags | cmajor::sbin::coff::DumpFlags::dumpRelocations;
                }
                else if (arg == "--data")
                {
                    dumpFlags = dumpFlags | cmajor::sbin::coff::DumpFlags::dumpSectionData;
                }
                else if (arg == "--all")
                {
                    dumpFlags = dumpFlags | cmajor::sbin::coff::DumpFlags::dumpSectionData | cmajor::sbin::coff::DumpFlags::dumpRelocations;
                }
                else
                {
                    throw std::runtime_error("unknown option '" + arg + "'");
                }
            }
            else if (arg.starts_with("-"))
            {
                std::string options = arg.substr(1);
                for (char o : options)
                {
                    switch (o)
                    {
                        case 'h':
                        {
                            PrintHelp();
                            return 0;
                        }
                        case 'v':
                        {
                            verbose = true;
                            break;
                        }
                        case 'r':
                        {
                            dumpFlags = dumpFlags | cmajor::sbin::coff::DumpFlags::dumpRelocations;
                            break;
                        }
                        case 'd':
                        {
                            dumpFlags = dumpFlags | cmajor::sbin::coff::DumpFlags::dumpSectionData;
                            break;
                        }
                        case 'a':
                        {
                            dumpFlags = dumpFlags | cmajor::sbin::coff::DumpFlags::dumpSectionData | cmajor::sbin::coff::DumpFlags::dumpRelocations;
                            break;
                        }
                        default:
                        {
                            throw std::runtime_error("unknown option '-" + std::string(1, o) + "'");
                        }
                    }
                }
            }
            else
            {
                files.push_back(util::GetFullPath(arg));
            }
        }
        for (const auto& file : files)
        {
            DumpObjectFile(file, dumpFlags, verbose);
        }
    }
    catch (const std::exception & ex)
    {
        std::cerr << ex.what() << "\n";
        return 1;
    }
    return 0;
}

