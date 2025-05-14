// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.sxcdb.command;

import cmajor.systemx.kernel;
import std.core;

export namespace cmajor::systemx::sxcdb {

enum class CommandKind : int
{
    none, exitCommand, helpCommand, filesCommand, fileCommand, listCommand, framesCommand, frameCommand, modesCommand, modeCommand, procsCommand, 
    breakCommand, breakPointsCommand, deleteCommand, clearCommand, continueCommand, nextCommand, stepCommand, outCommand, localsCommand, printCommand
};

class Debugger;

class Command
{
public:
    Command(CommandKind kind_);
    virtual ~Command();
    CommandKind Kind() const { return kind; }
    bool IsNext() const { return kind == CommandKind::nextCommand; }
    bool IsStep() const { return kind == CommandKind::stepCommand; }
    bool IsPrint() const { return kind == CommandKind::printCommand; }
    virtual int64_t MinArgs() const { return 0; }
    virtual int64_t MaxArgs() const { return 0; }
    virtual void Execute(Debugger& debugger) = 0;
    void SetArgs(const std::vector<int64_t>& args_);
    int64_t GetArgCount() const { return static_cast<int64_t>(args.size()); }
    int64_t GetArg(int argIndex) const { return args[argIndex]; }
private:
    CommandKind kind;
    std::vector<int64_t> args;
};

class CommandMap
{
public:
    static CommandMap& Instance();
    Command* GetCommand(const std::string& commandName) const;
private:
    std::map<std::string, CommandKind> commandMap;
    CommandMap();
};

Command* GetCommand(const std::string& commandName);

class ExitCommand : public Command
{
public:
    ExitCommand();
    void Execute(Debugger& debugger) override;
};

class HelpCommand : public Command
{
public:
    HelpCommand();
    void Execute(Debugger& debugger) override;
};

class FilesCommand : public Command
{
public:
    FilesCommand();
    int64_t MaxArgs() const override { return 1; }
    void Execute(Debugger& debugger) override;
};

class FileCommand : public Command
{
public:
    FileCommand();
    int64_t MaxArgs() const override { return 1; }
    void Execute(Debugger& debugger) override;
};

class ListCommand : public Command
{
public:
    ListCommand();
    int64_t MaxArgs() const override { return 2; }
    void Execute(Debugger& debugger) override;
};

class FramesCommand : public Command
{
public:
    FramesCommand();
    int64_t MaxArgs() const override { return 1; }
    void Execute(Debugger& debugger) override;
};

class FrameCommand : public Command
{
public:
    FrameCommand();
    int64_t MaxArgs() const override { return 1; }
    void Execute(Debugger& debugger) override;
};

class ModesCommand : public Command
{
public:
    ModesCommand();
    void Execute(Debugger& debugger) override;
};

class ModeCommand : public Command
{
public:
    ModeCommand();
    int64_t MaxArgs() const override { return 1; }
    void Execute(Debugger& debugger) override;
};

class ProcsCommand : public Command
{
public:
    ProcsCommand();
    int64_t MaxArgs() const override { return 1; }
    void Execute(Debugger& debugger) override;
private:
    std::vector<cmajor::systemx::kernel::Process*> processes;
};

class BreakCommand : public Command
{
public:
    BreakCommand();
    int64_t MinArgs() const override { return 1; }
    int64_t MaxArgs() const override { return 2; }
    void Execute(Debugger& debugger) override;
};

class BreakPointsCommand : public Command
{
public:
    BreakPointsCommand();
    int64_t MaxArgs() const override { return 1; }
    void Execute(Debugger& debugger) override;
};

class DeleteCommand : public Command
{
public:
    DeleteCommand();
    int64_t MinArgs() const override { return 1; }
    int64_t MaxArgs() const override { return 1; }
    void Execute(Debugger& debugger) override;
};

class ClearCommand : public Command
{
public:
    ClearCommand();
    void Execute(Debugger& debugger) override;
};

class ContinueCommand : public Command
{
public:
    ContinueCommand();
    void Execute(Debugger& debugger) override;
};

class NextCommand : public Command
{
public:
    NextCommand();
    void Execute(Debugger& debugger) override;
};

class StepCommand : public Command
{
public:
    StepCommand();
    void Execute(Debugger& debugger) override;
};

class OutCommand : public Command
{
public:
    OutCommand();
    void Execute(Debugger& debugger) override;
};

class LocalsCommand : public Command
{
public:
    LocalsCommand();
    void Execute(Debugger& debugger) override;
};

class PrintCommand : public Command
{
public:
    PrintCommand();
    int64_t MaxArgs() const override { return 1; }
    void SetExpr(const std::string& expr_);
    void Execute(Debugger& debugger) override;
private:
    std::string expr;
};

} // namespace cmajor::systemx::sxcdb
