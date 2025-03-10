// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.sbin.assembly;
import cmajor.sbin.compare_utils;
import cmajor.backend;
import cmajor.symbols;
import cmajor.build;
import cmajor.ast;
import std.core;
import util;

int main(int argc, const char** argv)
{
    try
    {
        util::Init();
        cmajor::symbols::SetBackEnd(cmajor::symbols::BackEnd::sbin);
        cmajor::backend::SetCurrentBackEnd(cmajor::backend::BackEndKind::sbinBackEnd);
        bool verbose = false;
        std::vector<std::string> files;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.starts_with("--"))
            {
                if (arg == "--verbose")
                {
                    verbose = true;
                }
                else if (arg.find('=') != std::string::npos)
                {
                    std::vector<std::string> components = util::Split(arg, '=');
                    if (components.size() == 2)
                    {
                        if (components[0] == "--config")
                        {
                            if (components[1] == "release")
                            {
                                cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::release);
                            }
                            else if (components[1] != "debug")
                            {
                                throw std::runtime_error("unknown configuration '" + components[1] + "'");
                            }
                        }
                        else if (components[0] == "--opt")
                        {
                            int optimizationLevel = std::stoi(components[1]);
                            if (optimizationLevel >= 0 && optimizationLevel <= 3)
                            {
                                cmajor::symbols::SetOptimizationLevel(optimizationLevel);
                            }
                            else
                            {
                                throw std::runtime_error("unknown optimization level '" + components[1] + "'");
                            }
                        }
                    }
                    else
                    {
                        throw std::runtime_error("unknown option '" + arg + "'");
                    }
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
                        case 'v':
                        {
                            verbose = true;
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
            if (file.ends_with(".cmp"))
            {
                std::unique_ptr<cmajor::ast::Project> project = cmajor::build::ReadProject(file);
                std::string libraryDir = project->LibraryDir();
                for (const auto& sourceFileName : project->RelativeSourceFilePaths())
                {
                    std::string asmFilePath = util::Path::Combine(libraryDir, util::Path::ChangeExtension(sourceFileName, ".asm"));
                    std::string masmAsmFilePath = util::Path::ChangeExtension(asmFilePath, ".masm.asm");
                    std::filesystem::copy_file(asmFilePath, masmAsmFilePath, std::filesystem::copy_options::overwrite_existing);
                }
                std::filesystem::current_path(libraryDir);
                for (const auto& sourceFileName : project->RelativeSourceFilePaths())
                {
                    std::string asmFilePath = util::Path::Combine(libraryDir, util::Path::GetFileName(util::Path::ChangeExtension(sourceFileName, ".asm")));
                    std::string objectFilePath = util::Path::Combine(libraryDir, util::Path::ChangeExtension(sourceFileName, ".obj"));
                    std::string masmObjectFilePath = cmajor::sbin::MakeMasmObject(asmFilePath, verbose);
                    cmajor::sbin::MakeStrippedAsm(masmObjectFilePath, verbose);
                    cmajor::sbin::assembly::Assembler assembler(asmFilePath, -1, verbose, false);
                    assembler.Assemble();
                    cmajor::sbin::MakeStrippedAsm(objectFilePath, verbose);
                    cmajor::sbin::MakeObjectFileDump(objectFilePath, verbose);
                }
            }
            else
            {
                throw std::runtime_error(".cmp file expected");
            }
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
        return 1;
    }
    util::Done();
    return 0;
}
