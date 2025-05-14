// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.sxcdb.command;

import cmajor.systemx.sxcdb.debugger;
import cmajor.systemx.sxcdb.break_point;

namespace cmajor::systemx::sxcdb {

std::string CommandKindStr(CommandKind kind)
{
    switch (kind)
    {
        case CommandKind::exitCommand: return "exit";
        case CommandKind::helpCommand: return "help";
        case CommandKind::filesCommand: return "files";
        case CommandKind::fileCommand: return "file";
        case CommandKind::listCommand: return "list";
        case CommandKind::framesCommand: return "frames";
        case CommandKind::frameCommand: return "frame";
        case CommandKind::modesCommand: return "modes";
        case CommandKind::modeCommand: return "mode";
        case CommandKind::procsCommand: return "procs";
        case CommandKind::breakCommand: return "break";
        case CommandKind::breakPointsCommand: return "breakpoints";
        case CommandKind::deleteCommand: return "delete";
        case CommandKind::clearCommand: return "clear";
        case CommandKind::continueCommand: return "continue";
        case CommandKind::nextCommand: return "next";
        case CommandKind::stepCommand: return "step";
        case CommandKind::outCommand: return "out";
        case CommandKind::localsCommand: return "locals";
        case CommandKind::printCommand: return "print";
    }
    return std::string();
}

Command::Command(CommandKind kind_) : kind(kind_)
{
}

Command::~Command()
{
}

void Command::SetArgs(const std::vector<int64_t>& args_)
{
    if (static_cast<int64_t>(args_.size()) < MinArgs() || static_cast<int64_t>(args_.size()) > MaxArgs())
    {
        throw std::runtime_error(CommandKindStr(kind) + " command takes " + std::to_string(MinArgs()) + " to " + std::to_string(MaxArgs()) + " arguments");
    }
    args = args_;
}

CommandMap& CommandMap::Instance()
{
    static CommandMap instance;
    return instance;
}

CommandMap::CommandMap()
{
    commandMap["exit"] = CommandKind::exitCommand;
    commandMap["ex"] = CommandKind::exitCommand;
    commandMap["help"] = CommandKind::helpCommand;
    commandMap["he"] = CommandKind::helpCommand;
    commandMap["files"] = CommandKind::filesCommand;
    commandMap["file"] = CommandKind::fileCommand;
    commandMap["fi"] = CommandKind::fileCommand;
    commandMap["list"] = CommandKind::listCommand;
    commandMap["li"] = CommandKind::listCommand;
    commandMap["frames"] = CommandKind::framesCommand;
    commandMap["frame"] = CommandKind::frameCommand;
    commandMap["fr"] = CommandKind::frameCommand;
    commandMap["modes"] = CommandKind::modesCommand;
    commandMap["mode"] = CommandKind::modeCommand;
    commandMap["mo"] = CommandKind::modeCommand;
    commandMap["procs"] = CommandKind::procsCommand;
    commandMap["pr"] = CommandKind::procsCommand;
    commandMap["break"] = CommandKind::breakCommand;
    commandMap["br"] = CommandKind::breakCommand;
    commandMap["breakpoints"] = CommandKind::breakPointsCommand;
    commandMap["delete"] = CommandKind::deleteCommand;
    commandMap["de"] = CommandKind::deleteCommand;
    commandMap["clear"] = CommandKind::clearCommand;
    commandMap["cl"] = CommandKind::clearCommand;
    commandMap["continue"] = CommandKind::continueCommand;
    commandMap["co"] = CommandKind::continueCommand;
    commandMap["next"] = CommandKind::nextCommand;
    commandMap["ne"] = CommandKind::nextCommand;
    commandMap["step"] = CommandKind::stepCommand;
    commandMap["st"] = CommandKind::stepCommand;
    commandMap["out"] = CommandKind::outCommand;
    commandMap["ou"] = CommandKind::outCommand;
    commandMap["locals"] = CommandKind::localsCommand;
    commandMap["lo"] = CommandKind::localsCommand;
    commandMap["print"] = CommandKind::printCommand;
    commandMap["pr"] = CommandKind::printCommand;
}

Command* CommandMap::GetCommand(const std::string& commandName) const
{
    auto it = commandMap.find(commandName);
    if (it != commandMap.end())
    {
        CommandKind kind = it->second;
        switch (kind)
        {
            case CommandKind::exitCommand:
            {
                return new ExitCommand();
            }
            case CommandKind::helpCommand:
            {
                return new HelpCommand();
            }
            case CommandKind::filesCommand:
            {
                return new FilesCommand();
            }
            case CommandKind::fileCommand:
            {
                return new FileCommand();
            }
            case CommandKind::listCommand:
            {
                return new ListCommand();
            }
            case CommandKind::framesCommand:
            {
                return new FramesCommand();
            }
            case CommandKind::frameCommand:
            {
                return new FrameCommand();
            }
            case CommandKind::modesCommand:
            {
                return new ModesCommand();
            }
            case CommandKind::modeCommand:
            {
                return new ModeCommand();
            }
            case CommandKind::procsCommand:
            {
                return new ProcsCommand();
            }
            case CommandKind::breakCommand:
            {
                return new BreakCommand();
            }
            case CommandKind::breakPointsCommand:
            {
                return new BreakPointsCommand();
            }
            case CommandKind::deleteCommand:
            {
                return new DeleteCommand();
            }
            case CommandKind::clearCommand:
            {
                return new ClearCommand();
            }
            case CommandKind::continueCommand:
            {
                return new ContinueCommand();
            }
            case CommandKind::nextCommand:
            {
                return new NextCommand();
            }
            case CommandKind::stepCommand:
            {
                return new StepCommand();
            }
            case CommandKind::outCommand:
            {
                return new OutCommand();
            }
            case CommandKind::localsCommand:
            {
                return new LocalsCommand();
            }
            case CommandKind::printCommand:
            {
                return new PrintCommand();
            }
        }
    }
    throw std::runtime_error("unknown command '" + commandName + "', try 'help'");
}

Command* GetCommand(const std::string& commandName)
{
    return CommandMap::Instance().GetCommand(commandName);
}

ExitCommand::ExitCommand() : Command(CommandKind::exitCommand)
{
}

void ExitCommand::Execute(Debugger& debugger)
{
    debugger.Exit();
}

HelpCommand::HelpCommand() : Command(CommandKind::helpCommand)
{
}

void HelpCommand::Execute(Debugger& debugger)
{
    debugger.PrintHelp();
}

FilesCommand::FilesCommand() : Command(CommandKind::filesCommand)
{
}

void FilesCommand::Execute(Debugger& debugger)
{
    if (GetArgCount() == 0)
    {
        debugger.Files(0);
        std::vector<int64_t> args;
        args.push_back(debugger.PageSize());
        SetArgs(args);
    }
    else
    {
        int64_t start = GetArg(0);
        debugger.Files(start);
        std::vector<int64_t> args;
        args.push_back(start + debugger.PageSize());
        SetArgs(args);
    }
}

FileCommand::FileCommand() : Command(CommandKind::fileCommand)
{
}

void FileCommand::Execute(Debugger& debugger)
{
    if (GetArgCount() == 0)
    {
        debugger.PrintFile();
    }
    else
    {
        debugger.SetFile(GetArg(0));
    }
}

ListCommand::ListCommand() : Command(CommandKind::listCommand)
{
}

void ListCommand::Execute(Debugger& debugger)
{
    if (GetArgCount() == 0)
    {
        int start = 1;
        debugger.List(debugger.File(), start);
        std::vector<int64_t> args;
        args.push_back(start + debugger.PageSize());
        SetArgs(args);
    }
    else if (GetArgCount() == 1)
    {
        int64_t start = GetArg(0);
        debugger.List(debugger.File(), start);
        std::vector<int64_t> args;
        args.push_back(start + debugger.PageSize());
        SetArgs(args);
    }
    else if (GetArgCount() == 2)
    {
        int64_t file = GetArg(0);
        int64_t start = GetArg(1);
        debugger.List(file, start);
        std::vector<int64_t> args;
        args.push_back(file);
        args.push_back(start + debugger.PageSize());
        SetArgs(args);
    }
}

FramesCommand::FramesCommand() : Command(CommandKind::framesCommand)
{
}

void FramesCommand::Execute(Debugger& debugger)
{
    if (GetArgCount() == 0)
    {
        debugger.PrintFrames(0);
        std::vector<int64_t> args;
        args.push_back(debugger.PageSize());
        SetArgs(args);
    }
    else
    {
        int64_t start = GetArg(0);
        debugger.PrintFrames(start);
        std::vector<int64_t> args;
        args.push_back(start + debugger.PageSize());
        SetArgs(args);
    }
}

FrameCommand::FrameCommand() : Command(CommandKind::frameCommand)
{
}

void FrameCommand::Execute(Debugger& debugger)
{
    if (GetArgCount() == 0)
    {
        debugger.PrintFrame();
    }
    else
    {
        debugger.SetFrame(GetArg(0));
    }
}

ModesCommand::ModesCommand() : Command(CommandKind::modesCommand)
{
}

void ModesCommand::Execute(Debugger& debugger)
{
    debugger.PrintModes();
}

ModeCommand::ModeCommand() : Command(CommandKind::modeCommand)
{
}

void ModeCommand::Execute(Debugger& debugger)
{
    if (GetArgCount() == 0)
    {
        debugger.PrintMode();
    }
    else
    {
        debugger.SetMode(GetArg(0));
    }
}

ProcsCommand::ProcsCommand() : Command(CommandKind::procsCommand), processes(cmajor::systemx::kernel::ProcessManager::Instance().GetProcesses())
{
}

void ProcsCommand::Execute(Debugger& debugger)
{
    if (GetArgCount() == 0)
    {
        debugger.PrintProcs(0, processes);
        std::vector<int64_t> args;
        args.push_back(debugger.PageSize());
        SetArgs(args);
    }
    else
    {
        int64_t start = GetArg(0);
        debugger.PrintProcs(start, processes);
        std::vector<int64_t> args;
        args.push_back(start + debugger.PageSize());
        SetArgs(args);
    }
}

BreakCommand::BreakCommand() : Command(CommandKind::breakCommand)
{
}

void BreakCommand::Execute(Debugger& debugger)
{
    if (GetArgCount() == 1)
    {
        int64_t file = debugger.File();
        int64_t line = GetArg(0);
        debugger.Break(file, line);
    }
    else if (GetArgCount() == 2)
    {
        int64_t file = GetArg(0);
        int64_t line = GetArg(1);
        debugger.Break(file, line);
    }
}

BreakPointsCommand::BreakPointsCommand() : Command(CommandKind::breakPointsCommand)
{
}

void BreakPointsCommand::Execute(Debugger& debugger)
{
    if (GetArgCount() == 0)
    {
        debugger.PrintBreakPoints(0);
        std::vector<int64_t> args;
        args.push_back(debugger.PageSize());
        SetArgs(args);
    }
    else
    {
        int64_t start = GetArg(0);
        debugger.PrintBreakPoints(start);
        std::vector<int64_t> args;
        args.push_back(start + debugger.PageSize());
        SetArgs(args);
    }
}

DeleteCommand::DeleteCommand() : Command(CommandKind::deleteCommand)
{
}

void DeleteCommand::Execute(Debugger& debugger)
{
    int64_t bpId = GetArg(0);
    debugger.DeleteBreakPoint(bpId);
}

ClearCommand::ClearCommand() : Command(CommandKind::clearCommand)
{
}

void ClearCommand::Execute(Debugger& debugger)
{
    debugger.ClearBreakPoints();
}

ContinueCommand::ContinueCommand() : Command(CommandKind::continueCommand)
{
}

void ContinueCommand::Execute(Debugger& debugger)
{
    debugger.Continue();
}

NextCommand::NextCommand() : Command(CommandKind::nextCommand)
{
}

void NextCommand::Execute(Debugger& debugger)
{
    debugger.Next();
}

StepCommand::StepCommand() : Command(CommandKind::stepCommand)
{
}

void StepCommand::Execute(Debugger& debugger)
{
    debugger.Step();
}

OutCommand::OutCommand() : Command(CommandKind::outCommand)
{
}

void OutCommand::Execute(Debugger& debugger)
{
    debugger.Out();
}

LocalsCommand::LocalsCommand() : Command(CommandKind::localsCommand)
{
}

void LocalsCommand::Execute(Debugger& debugger)
{
    debugger.PrintLocals();
}

PrintCommand::PrintCommand() : Command(CommandKind::printCommand)
{
}

void PrintCommand::SetExpr(const std::string& expr_)
{
    expr = expr_;
}

void PrintCommand::Execute(Debugger& debugger)
{
    int64_t start = 0;
    if (GetArgCount() == 1)
    {
        start = GetArg(0);
    }
    debugger.Print(expr, start);
}

} // namespace cmajor::systemx::sxcdb
