// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debug.gdb;

import cmajor.debug.gdb.reply.lexer;
import cmajor.debug.gdb.reply.parser;
import util;

namespace cmajor::debug {

GdbCommand::GdbCommand(Kind kind_, const std::string& str_) : kind(kind_), str(str_)
{
}

GdbCommand::~GdbCommand()
{
}

GdbBreakInsertCommand::GdbBreakInsertCommand(const std::string& location) : GdbCommand(Kind::breakInsert, "-break-insert " + location)
{
}

GdbBreakDeleteCommand::GdbBreakDeleteCommand(const std::string& breakpoints) : GdbCommand(Kind::breakDelete, "-break-delete " + breakpoints)
{
}

GdbBreakConditionCommand::GdbBreakConditionCommand(int gdbBreakpointNumber, const std::string& condition) :
    GdbCommand(Kind::breakCondition, "-break-condition " + std::to_string(gdbBreakpointNumber) + " " + condition)
{
}

GdbDeleteAllBreakpointsCommand::GdbDeleteAllBreakpointsCommand() : GdbCommand(Kind::deleteAll, "delete")
{
}

GdbExecContinueCommand::GdbExecContinueCommand() : GdbCommand(Kind::execContinue, "-exec-continue")
{
}

GdbExecFinishCommand::GdbExecFinishCommand() : GdbCommand(Kind::execFinish, "-exec-finish")
{
}

GdbExecNextCommand::GdbExecNextCommand() : GdbCommand(Kind::execNext, "-exec-next")
{
}

GdbExecStepCommand::GdbExecStepCommand() : GdbCommand(Kind::execStep, "-exec-step")
{
}

GdbExecUntilCommand::GdbExecUntilCommand(const std::string& location) : GdbCommand(Kind::execUntil, "-exec-until " + location)
{
}

GdbExecRunCommand::GdbExecRunCommand() : GdbCommand(Kind::execRun, "-exec-run")
{
}

GdbExitCommand::GdbExitCommand() : GdbCommand(Kind::exit, "-gdb-exit")
{
}

GdbStackInfoDepthCommand::GdbStackInfoDepthCommand() : GdbCommand(Kind::stackInfoDepth, "-stack-info-depth")
{
}

std::string FrameStr(int lowFrame, int highFrame)
{
    if (lowFrame == -1 && highFrame == -1)
    {
        return std::string();
    }
    std::string s(1, ' ');
    s.append(std::to_string(lowFrame)).append(1, ' ').append(std::to_string(highFrame));
    return s;
}

GdbStackListFramesCommand::GdbStackListFramesCommand(int lowFrame, int highFrame) :
    GdbCommand(Kind::stackListFrames, "-stack-list-frames" + FrameStr(lowFrame, highFrame))
{
}

GdbVarCreateCommand::GdbVarCreateCommand(const std::string& name, const std::string& frame, const std::string& expression) :
    GdbCommand(Kind::varCreate, "-var-create " + name + " " + frame + " " + expression)
{
}

std::string ChildrenOpt(bool justChildren)
{
    if (justChildren)
    {
        return "-c ";
    }
    else
    {
        return "";
    }
}

GdbVarDeleteCommand::GdbVarDeleteCommand(const std::string& name, bool justChildren) :
    GdbCommand(Kind::varDelete, "-var-delete " + ChildrenOpt(justChildren) + name)
{
}

std::string FormatStr(Format format)
{
    switch (format)
    {
    case Format::binary: return "binary";
    case Format::decimal: return "decimal";
    case Format::hexadecimal: return "hexdecimal";
    case Format::octal: return "octal";
    case Format::natural: return "natural";
    case Format::zeroHexadecimal: return "zero-hexadecimal";
    }
    return "";
}

GdbVarSetFormatCommand::GdbVarSetFormatCommand(const std::string& name, Format format) :
    GdbCommand(Kind::varSetFormat, "-var-set-format " + name + " " + FormatStr(format))
{
}

GdbVarShowFormatCommand::GdbVarShowFormatCommand(const std::string& name) :
    GdbCommand(Kind::varShowFormat, "-var-show-format " + name)
{
}

GdbVarInfoNumChildrenCommand::GdbVarInfoNumChildrenCommand(const std::string& name) :
    GdbCommand(Kind::varInfoNumChildren, "-var-info-num-children " + name)
{
}

std::string PrintValuesStr(bool printValues)
{
    if (printValues)
    {
        return " 1";
    }
    else
    {
        return " 0";
    }
}

std::string FromToStr(int from, int to)
{
    if (from != -1 && to != -1)
    {
        return " " + std::to_string(from) + " " + std::to_string(to);
    }
    else
    {
        return "";
    }
}

GdbVarListChildrenCommand::GdbVarListChildrenCommand(const std::string& name, bool printValues, int from, int to) :
    GdbCommand(Kind::varListChildren, "-var-list-children " + name + FromToStr(from, to))
{
}

GdbVarInfoTypeCommand::GdbVarInfoTypeCommand(const std::string& name) :
    GdbCommand(Kind::varInfoType, "-var-info-type " + name)
{
}

GdbVarInfoExpressionCommand::GdbVarInfoExpressionCommand(const std::string& name) :
    GdbCommand(Kind::varInfoExpression, "-var-info-expression " + name)
{
}

GdbVarInfoPathExpressionCommand::GdbVarInfoPathExpressionCommand(const std::string& name) :
    GdbCommand(Kind::varInfoPathExpression, "-var-info-path-expression " + name)
{
}

GdbVarShowAttributesCommand::GdbVarShowAttributesCommand(const std::string& name) :
    GdbCommand(Kind::varShowAttributes, "-var-show-attributes " + name)
{
}

std::string FormatExprStr(Format format)
{
    switch (format)
    {
    case Format::binary: return "-f binary ";
    case Format::decimal: return "-f decimal ";
    case Format::hexadecimal: return "-f hexadecimal ";
    case Format::octal: return "-f octal ";
    case Format::natural: return "-f natural ";
    case Format::zeroHexadecimal: return "-f zero-hexadecimal ";
    }
    return "";
}

GdbVarEvaluateExpressionCommand::GdbVarEvaluateExpressionCommand(const std::string& name, Format format) :
    GdbCommand(Kind::varEvaluateExpression, "-var-evaluate-expression " + FormatExprStr(format) + name)
{
}

GdbVarAssignCommand::GdbVarAssignCommand(const std::string& name, const std::string& expression) :
    GdbCommand(Kind::varAssign, "-var-assign " + name + " " + expression)
{
}

GdbVarUpdateCommand::GdbVarUpdateCommand(const std::string& name) :
    GdbCommand(Kind::varUpdate, "-var-update " + name)
{
}

std::string FrozenStr(bool frozen)
{
    if (frozen)
    {
        return "1";
    }
    else
    {
        return "0";
    }
}

GdbVarSetFrozenCommand::GdbVarSetFrozenCommand(const std::string& name, bool frozen) :
    GdbCommand(Kind::varSetFrozen, "-var-set-frozen " + name + " " + FrozenStr(frozen))
{
}

GdbVarSetUpdateRangeCommand::GdbVarSetUpdateRangeCommand(const std::string& name, int from, int to) :
    GdbCommand(Kind::varSetUpdateRange, "-var-set-update-range " + name + FromToStr(from, to))
{
}

GdbVarSetVisualizerCommand::GdbVarSetVisualizerCommand(const std::string& name, const std::string& visualizer) :
    GdbCommand(Kind::varSetVisualizer, "-var-set-visualizer " + name + " " + visualizer)
{
}

GdbPrintCommand::GdbPrintCommand(const std::string& expression) : GdbCommand(Kind::print, "print " + expression)
{
}

GdbExamineCommand::GdbExamineCommand(int numBytes, char format, char unitSize, uint64_t address) :
    GdbCommand(Kind::examineCommand, "x/" + std::to_string(numBytes) + std::string(1, format) + std::string(1, unitSize) + " 0x" + util::ToHexString(address))
{
}

GdbValue::GdbValue(Kind kind_) : kind(kind_)
{
}

GdbValue::~GdbValue()
{
}

GdbStringValue::GdbStringValue(const std::string& value_) : GdbValue(Kind::string), value(value_)
{
}

util::JsonValue* GdbStringValue::ToJson() const
{
    return new util::JsonString(util::ToUtf32(value));
}

GdbTupleValue::GdbTupleValue() : GdbValue(Kind::tuple), results()
{
}

void GdbTupleValue::AddResult(GdbResult* result)
{
    fieldMap[result->Name()] = result->Value();
    results.push_back(std::unique_ptr<GdbResult>(result));
}

GdbValue* GdbTupleValue::GetField(const std::string& fieldName) const
{
    auto it = fieldMap.find(fieldName);
    if (it != fieldMap.cend())
    {
        GdbValue* value = it->second;
        return value;
    }
    else
    {
        return nullptr;
    }
}

util::JsonValue* GdbTupleValue::ToJson() const
{
    util::JsonObject* jsonObject = new util::JsonObject();
    for (const auto& result : results)
    {
        result->AddJsonValueTo(jsonObject);
    }
    return jsonObject;
}

GdbListValue::GdbListValue() : GdbValue(Kind::list)
{
}

void GdbListValue::AddValue(GdbValue* value)
{
    values.push_back(std::unique_ptr<GdbValue>(value));
}

util::JsonValue* GdbListValue::ToJson() const
{
    util::JsonArray* jsonArray = new util::JsonArray();
    for (const auto& result : values)
    {
        jsonArray->AddItem(std::unique_ptr<util::JsonValue>(result->ToJson()));
    }
    return jsonArray;
}

GdbValue* GdbListValue::GetValue(int index) const
{
    if (index >= 0 && index < Count())
    {
        return values[index].get();
    }
    else
    {
        return nullptr;
    }
}

GdbResult::GdbResult(const std::string& name_, GdbValue* value_) : GdbValue(Kind::result), name(name_), value(value_)
{
}

util::JsonValue* GdbResult::ToJson() const
{
    util::JsonObject* jsonObject = new util::JsonObject();
    jsonObject->AddField(U"name", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(name))));
    jsonObject->AddField(U"value", std::unique_ptr<util::JsonValue>(value->ToJson()));
    return jsonObject;
}

void GdbResult::AddJsonValueTo(util::JsonObject* jsonObject)
{
    jsonObject->AddField(util::ToUtf32(name), std::unique_ptr<util::JsonValue>(value->ToJson()));
}

GdbResults::GdbResults()
{
}

void GdbResults::Add(GdbResult* result)
{
    results.push_back(std::unique_ptr<GdbResult>(result));
    fieldMap[result->Name()] = result->Value();
}

GdbValue* GdbResults::GetField(const std::string& fieldName) const
{
    auto it = fieldMap.find(fieldName);
    if (it != fieldMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

std::unique_ptr<util::JsonValue> GdbResults::ToJson() const
{
    util::JsonObject* jsonObject = new util::JsonObject();
    for (const auto& r : results)
    {
        r->AddJsonValueTo(jsonObject);
    }
    return std::unique_ptr<util::JsonValue>(jsonObject);
}

GdbReplyRecord::GdbReplyRecord(Kind kind_, GdbResults* results_) : kind(kind_), results(results_)
{
}

GdbReplyRecord::~GdbReplyRecord()
{
}

const char* GdbReplyRecord::KindStr() const
{
    switch (kind)
    {
    case Kind::result: return "result";
    case Kind::execRunning: return "execRunning";
    case Kind::execStopped: return "execStopped";
    case Kind::notification: return "notification";
    case Kind::consoleOutput: return "consoleOutput";
    case Kind::targetOutput: return "targetOutput";
    case Kind::logOutput: return "logOutput";
    case Kind::prompt: return "prompt";
    case Kind::parsingError: return "parsingError";
    }
    return "";
}

std::unique_ptr<util::JsonValue> GdbReplyRecord::ToJson() const
{
    std::unique_ptr<util::JsonValue> value = results->ToJson();
    if (value->Type() == util::JsonValueType::object)
    {
        util::JsonObject* jsonObject = static_cast<util::JsonObject*>(value.get());
        jsonObject->AddField(U"record", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(KindStr()))));
    }
    return value;
}

GdbResultRecord::GdbResultRecord(Class cls_, GdbResults* results) : GdbReplyRecord(Kind::result, results), cls(cls_)
{
}

const char* GdbResultRecord::ClassStr() const
{
    switch (cls)
    {
    case Class::done: return "done";
    case Class::running: return "running";
    case Class::connected: return "connected";
    case Class::error: return "error";
    case Class::exit: return "exit";
    }
    return "";
}

std::unique_ptr<util::JsonValue> GdbResultRecord::ToJson() const
{
    std::unique_ptr<util::JsonValue> value = GdbReplyRecord::ToJson();
    if (value->Type() == util::JsonValueType::object)
    {
        util::JsonObject* jsonObject = static_cast<util::JsonObject*>(value.get());
        jsonObject->AddField(U"class", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(ClassStr()))));
    }
    return value;
}

GdbDoneRecord::GdbDoneRecord(GdbResults* results) : GdbResultRecord(Class::done, results)
{
}

GdbRunningRecord::GdbRunningRecord(GdbResults* results) : GdbResultRecord(Class::running, results)
{
}

GdbConnectedRecord::GdbConnectedRecord(GdbResults* results) : GdbResultRecord(Class::connected, results)
{
}

GdbErrorRecord::GdbErrorRecord(GdbResults* results) : GdbResultRecord(Class::error, results)
{
}

GdbExitRecord::GdbExitRecord(GdbResults* results) : GdbResultRecord(Class::exit, results)
{
}

GdbAsyncRecord::GdbAsyncRecord(Kind kind, GdbResults* results) : GdbReplyRecord(kind, results)
{
}

GdbExecRecord::GdbExecRecord(Kind kind, GdbResults* results) : GdbAsyncRecord(kind, results)
{
}

GdbExecRunningRecord::GdbExecRunningRecord(GdbResults* results) : GdbExecRecord(Kind::execRunning, results)
{
}

GdbExecStoppedRecord::GdbExecStoppedRecord(GdbResults* results) : GdbExecRecord(Kind::execStopped, results)
{
}

GdbNotifyAsyncRecord::GdbNotifyAsyncRecord(const std::string& notification_, GdbResults* results) : GdbAsyncRecord(Kind::notification, results)
{
}

std::unique_ptr<util::JsonValue> GdbNotifyAsyncRecord::ToJson() const
{
    std::unique_ptr<util::JsonValue> value = GdbAsyncRecord::ToJson();
    if (value->Type() == util::JsonValueType::object)
    {
        util::JsonObject* jsonObject = static_cast<util::JsonObject*>(value.get());
        jsonObject->AddField(U"notification", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(notification))));
    }
    return value;
}

GdbStreamRecord::GdbStreamRecord(Kind kind, const std::string& text_) : GdbReplyRecord(kind, new GdbResults()), text(text_)
{
}

std::unique_ptr<util::JsonValue> GdbStreamRecord::ToJson() const
{
    std::unique_ptr<util::JsonValue> value = GdbReplyRecord::ToJson();
    if (value->Type() == util::JsonValueType::object)
    {
        util::JsonObject* jsonObject = static_cast<util::JsonObject*>(value.get());
        jsonObject->AddField(U"text", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(text))));
    }
    return value;
}

GdbConsoleOutputRecord::GdbConsoleOutputRecord(const std::string& text) : GdbStreamRecord(Kind::consoleOutput, text)
{
}

GdbTargetOutputRecord::GdbTargetOutputRecord(const std::string& text) : GdbStreamRecord(Kind::targetOutput, text)
{
}

GdbLogOutputRecord::GdbLogOutputRecord(const std::string& text) : GdbStreamRecord(Kind::logOutput, text)
{
}

GdbPrompt::GdbPrompt() : GdbReplyRecord(Kind::prompt, new GdbResults())
{
}

GdbParsingError::GdbParsingError(const std::string& parsingError_) : GdbReplyRecord(Kind::parsingError, new GdbResults()), parsingError(parsingError_)
{
}

GdbReply::GdbReply() : resultRecord(nullptr), stoppedRecord(nullptr)
{
}

void GdbReply::SetTextLines(const std::vector<std::string>& textLines_)
{
    textLines = textLines_;
}

void GdbReply::AddReplyRecord(std::unique_ptr<GdbReplyRecord>&& replyRecord)
{
    if (replyRecord->GetKind() == GdbReplyRecord::Kind::result)
    {
        resultRecord = static_cast<GdbResultRecord*>(replyRecord.get());
    }
    if (replyRecord->Stopped())
    {
        stoppedRecord = replyRecord.get();
    }
    replyRecords.push_back(std::move(replyRecord));
}

GdbReply::~GdbReply()
{
}

std::unique_ptr<util::JsonValue> GdbReply::ToJson() const
{
    util::JsonObject* object = new util::JsonObject();
    util::JsonArray* array = new util::JsonArray();
    int resultIndex = -1;
    int stoppedIndex = -1;
    int n = replyRecords.size();
    for (int i = 0; i < n; ++i)
    {
        const std::unique_ptr<GdbReplyRecord>& record = replyRecords[i];
        if (record->GetKind() == GdbReplyRecord::Kind::result)
        {
            resultIndex = i;
        }
        if (record->Stopped())
        {
            stoppedIndex = i;
        }
        array->AddItem(record->ToJson());
    }
    object->AddField(U"records", std::unique_ptr<util::JsonValue>(array));
    if (resultIndex != -1)
    {
        object->AddField(U"resultIndex", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(std::to_string(resultIndex)))));
    }
    if (stoppedIndex != -1)
    {
        object->AddField(U"stoppedIndex", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(std::to_string(stoppedIndex)))));
    }
    return std::unique_ptr<util::JsonValue>(object);
}

class Gdb
{
public:
    static Gdb& Instance();
    void SetDebugFlag() { debug = true; }
    void Start(const std::string& executable, const std::vector<std::string>& args, GdbDriver& driver);
    void Run(const std::string& startCommand);
    void Stop(GdbDriver& driver);
    void Terminate();
    std::unique_ptr<GdbReply> Execute(const GdbCommand& commmand, GdbDriver& driver);
    std::unique_ptr<GdbReply> ReadReply(GdbDriver& driver);
    void WriteTargetInputLine(const std::string& line);
    GdbReply* GetStartReply() { return startReply.get(); }
private:
    Gdb();
    bool debug;
    static std::unique_ptr<Gdb> instance;
    std::unique_ptr<util::Process> gdb;
    std::unique_ptr<GdbReply> startReply;
    std::thread gdbThread;
    std::exception_ptr gdbException;
    int gdbExitCode;
    bool exited;
    std::condition_variable gdbStarted;
    std::mutex mtx;
};

void RunGDB(Gdb* gdb, const std::string& startCommand)
{
    gdb->Run(startCommand);
}

Gdb& Gdb::Instance()
{
    static Gdb instance;
    return instance;
}

Gdb::Gdb() : debug(false), gdbExitCode(-1), exited(false)
{
}

void Gdb::Start(const std::string& executable, const std::vector<std::string>& args, GdbDriver& driver)
{
    std::string startCommand;
    startCommand.append("gdb");
    startCommand.append(" --interpreter=mi");
    if (!args.empty())
    {
        startCommand.append(" --args");
        startCommand.append(" \"").append(util::Path::MakeCanonical(executable)).append("\"");
        for (const std::string& arg : args)
        {
            startCommand.append(" \"").append(arg).append("\"");
        }
    }
    else
    {
        startCommand.append(" \"").append(executable).append("\"");
    }
    gdbThread = std::thread{ RunGDB, this, startCommand };
    std::unique_lock<std::mutex> lock(mtx);
    gdbStarted.wait(lock);
    startReply = ReadReply(driver);
}

void Gdb::Run(const std::string& startCommand)
{
    try
    {
        gdb.reset(new util::Process(startCommand,
            util::Process::Redirections::processStdIn |
            util::Process::Redirections::processStdOut |
            util::Process::Redirections::processStdErr));
        gdbStarted.notify_one();
    }
    catch (...)
    {
        gdbException = std::current_exception();
        gdbStarted.notify_one();
    }
}

void Gdb::Stop(GdbDriver& driver)
{
    if (exited) return;
    exited = true;
    GdbExitCommand exitCommand;
    Execute(exitCommand, driver);
    gdb->WaitForExit();
    gdbThread.join();
    gdbExitCode = gdb->ExitCode();
}

void Gdb::Terminate()
{
    try
    {
        gdb->Terminate();
    }
    catch (const std::exception&)
    {
    }
    try
    {
        gdbThread.join();
    }
    catch (const std::exception&)
    {
    }
}

std::unique_ptr<GdbReply> Gdb::Execute(const GdbCommand& command, GdbDriver& driver)
{
    if (gdbException)
    {
        std::rethrow_exception(gdbException);
    }
    if (debug)
    {
        util::LogMessage(-1, "-> " + command.Str());
    }
    gdb->WriteLine(command.Str());
    std::unique_ptr<GdbReply> reply;
    if (command.GetKind() != GdbCommand::Kind::exit)
    {
        reply = ReadReply(driver);
    }
    if (gdbException)
    {
        std::rethrow_exception(gdbException);
    }
    return reply;
}

std::unique_ptr<GdbReplyRecord> ParseGdbReplyRecord(const std::string& line)
{
    std::u32string lineNL = util::ToUtf32(line);
    lineNL.append(U"\r\n");
    auto lexer = cmajor::debug::gdb::reply::lexer::MakeLexer(lineNL.c_str(), lineNL.c_str() + line.length(), "");
    using LexerType = decltype(lexer);
    try
    {
        std::unique_ptr<GdbReplyRecord> replyRecord = cmajor::debug::gdb::reply::parser::GdbReplyParser<LexerType>::Parse(lexer);
        return replyRecord;
    }
    catch (const std::exception& ex)
    {
        return std::unique_ptr<GdbParsingError>(new GdbParsingError(ex.what()));
    }
}

std::string PreparedLine(const std::string& line)
{
    if (!line.empty())
    {
        switch (line[0])
        {
        case '=': case '~': case '^': case '*': case '&':
        {
            return line;
        }
        default:
        {
            if (line.starts_with("(gdb)"))
            {
                return line;
            }
            return "@\"" + util::StringStr(line) + "\"";
        }
        }
    }
    return line;
}

std::unique_ptr<GdbReply> Gdb::ReadReply(GdbDriver& driver)
{
    std::unique_ptr<GdbReply> reply(new GdbReply());
    std::vector<std::string> textLines;
    std::string line = gdb->ReadLine(util::Process::StdHandle::stdOut);
    if (debug)
    {
        util::LogMessage(-1, "<- " + line);
    }
    std::string preparedLine = PreparedLine(line);
    if (preparedLine.empty())
    {
        return std::unique_ptr<GdbReply>();
    }
    std::unique_ptr<GdbReplyRecord> replyRecord = ParseGdbReplyRecord(preparedLine);
    driver.ProcessReplyRecord(replyRecord.get());
    while (replyRecord->GetKind() != GdbReplyRecord::Kind::prompt)
    {
        if (replyRecord->GetKind() == GdbReplyRecord::Kind::parsingError)
        {
            GdbParsingError* parsingError = static_cast<GdbParsingError*>(replyRecord.get());
            util::LogMessage(-1, "error parsing GDB reply '" + line + "': " + parsingError->ParsingError());
        }
        textLines.push_back(line);
        reply->AddReplyRecord(std::move(replyRecord));
        line = gdb->ReadLine(util::Process::StdHandle::stdOut);
        if (debug)
        {
            util::LogMessage(-1, "<- " + line);
        }
        std::string preparedLine = PreparedLine(line);
        if (preparedLine.empty())
        {
            break;
        }
        replyRecord = ParseGdbReplyRecord(preparedLine);
        driver.ProcessReplyRecord(replyRecord.get());
    }
    reply->SetTextLines(textLines);
    return reply;
}

void Gdb::WriteTargetInputLine(const std::string& line)
{
    gdb->WriteLine(line);
}

void SetDebugFlag()
{
    Gdb::Instance().SetDebugFlag();
}

void StartGDB(const std::string& executable, const std::vector<std::string>& args, GdbDriver& driver)
{
    Gdb::Instance().Start(executable, args, driver);
}

GdbReply* GetGDBStartReply()
{
    return Gdb::Instance().GetStartReply();
}

std::unique_ptr<GdbReply> ExecuteGDBCommand(const GdbCommand& command, GdbDriver& driver)
{
    return Gdb::Instance().Execute(command, driver);
}

std::unique_ptr<GdbReply> ReadGDBReply(GdbDriver& driver)
{
    return Gdb::Instance().ReadReply(driver);
}

void StopGDB(GdbDriver& driver)
{
    Gdb::Instance().Stop(driver);
}

void TerminateGDB()
{
    Gdb::Instance().Terminate();
}

void WriteTargetInputLine(const std::string& line)
{
    Gdb::Instance().WriteTargetInputLine(line);
}

} // namespace cmajor::debug
