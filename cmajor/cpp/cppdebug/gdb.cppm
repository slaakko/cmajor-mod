// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.gdb;

import util;
import std.core;

export namespace cmajor::debug {

export namespace gdb {}

class GdbReplyRecord;

class GdbDriver
{
public:
    virtual void ProcessReplyRecord(GdbReplyRecord* record) = 0;
    virtual bool TargetRunning() const = 0;
    virtual bool TargetWasRunning() const = 0;
    virtual bool TargetOutput() const = 0;
    virtual void Proceed() = 0;
    virtual void ResetConsole() = 0;
    virtual bool Exiting() const = 0;
    virtual void Exit() = 0;
    virtual void Prompt() = 0;
    virtual void TargetInputPrompt() = 0;
    virtual void Error(const std::string& msg) = 0;
    virtual bool LatestCommandWasRunningCommand() = 0;
    virtual std::string& CurrentSourceFilePath() = 0;
};

enum class Format
{
    default_, binary, decimal, hexadecimal, octal, natural, zeroHexadecimal
};

class GdbCommand
{
public:
    enum class Kind
    {
        exit, breakInsert, breakDelete, breakCondition, execContinue, execFinish, execNext, execStep, execUntil, execRun, stackInfoDepth, stackListFrames,
        varCreate, varDelete, varSetFormat, varShowFormat, varInfoNumChildren, varListChildren, varInfoType, varInfoExpression,
        varInfoPathExpression, varShowAttributes, varEvaluateExpression, varAssign, varUpdate, varSetFrozen, varSetUpdateRange,
        varSetVisualizer, print, deleteAll, examineCommand
    };
    GdbCommand(Kind kind_, const std::string& str_);
    GdbCommand(const GdbCommand&) = delete;
    GdbCommand(GdbCommand&&) = delete;
    GdbCommand& operator=(const GdbCommand&) = delete;
    GdbCommand& operator=(GdbCommand&&) = delete;
    virtual ~GdbCommand();
    Kind GetKind() const { return kind; }
    const std::string& Str() const { return str; }
private:
    Kind kind;
    std::string str;
};

class GdbBreakInsertCommand : public GdbCommand
{
public:
    GdbBreakInsertCommand(const std::string& location);
};

class GdbBreakDeleteCommand : public GdbCommand
{
public:
    GdbBreakDeleteCommand(const std::string& breakpoints);
};

class GdbBreakConditionCommand : public GdbCommand
{
public:
    GdbBreakConditionCommand(int gdbBreakpointNumber, const std::string& condition);
};

class GdbDeleteAllBreakpointsCommand : public GdbCommand
{
public:
    GdbDeleteAllBreakpointsCommand();
};

class GdbExecContinueCommand : public GdbCommand
{
public:
    GdbExecContinueCommand();
};

class GdbExecFinishCommand : public GdbCommand
{
public:
    GdbExecFinishCommand();
};

class GdbExecNextCommand : public GdbCommand
{
public:
    GdbExecNextCommand();
};

class GdbExecStepCommand : public GdbCommand
{
public:
    GdbExecStepCommand();
};

class GdbExecUntilCommand : public GdbCommand
{
public:
    GdbExecUntilCommand(const std::string& location);
};

class GdbExecRunCommand : public GdbCommand
{
public:
    GdbExecRunCommand();
};

class GdbExitCommand : public GdbCommand
{
public:
    GdbExitCommand();
};

class GdbStackInfoDepthCommand : public GdbCommand
{
public:
    GdbStackInfoDepthCommand();
};

class GdbStackListFramesCommand : public GdbCommand
{
public:
    GdbStackListFramesCommand(int lowFrame, int highFrame);
};

class GdbVarCreateCommand : public GdbCommand
{
public:
    GdbVarCreateCommand(const std::string& name, const std::string& frame, const std::string& expression);
};

class GdbVarDeleteCommand : public GdbCommand
{
public:
    GdbVarDeleteCommand(const std::string& name, bool justChildren);
};

class GdbVarSetFormatCommand : public GdbCommand
{
public:
    GdbVarSetFormatCommand(const std::string& name, Format format);
};

class GdbVarShowFormatCommand : public GdbCommand
{
public:
    GdbVarShowFormatCommand(const std::string& name);
};

class GdbVarInfoNumChildrenCommand : public GdbCommand
{
public:
    GdbVarInfoNumChildrenCommand(const std::string& name);
};

class GdbVarListChildrenCommand : public GdbCommand
{
public:
    GdbVarListChildrenCommand(const std::string& name, bool printValues, int from, int to);
};

class GdbVarInfoTypeCommand : public GdbCommand
{
public:
    GdbVarInfoTypeCommand(const std::string& name);
};

class GdbVarInfoExpressionCommand : public GdbCommand
{
public:
    GdbVarInfoExpressionCommand(const std::string& name);
};

class GdbVarInfoPathExpressionCommand : public GdbCommand
{
public:
    GdbVarInfoPathExpressionCommand(const std::string& name);
};

class GdbVarShowAttributesCommand : public GdbCommand
{
public:
    GdbVarShowAttributesCommand(const std::string& name);
};

class GdbVarEvaluateExpressionCommand : public GdbCommand
{
public:
    GdbVarEvaluateExpressionCommand(const std::string& name, Format format);
};

class GdbVarAssignCommand : public GdbCommand
{
public:
    GdbVarAssignCommand(const std::string& name, const std::string& expression);
};

class GdbVarUpdateCommand : public GdbCommand
{
public:
    GdbVarUpdateCommand(const std::string& name);
};

class GdbVarSetFrozenCommand : public GdbCommand
{
public:
    GdbVarSetFrozenCommand(const std::string& name, bool frozen);
};

class GdbVarSetUpdateRangeCommand : public GdbCommand
{
public:
    GdbVarSetUpdateRangeCommand(const std::string& name, int from, int to);
};

class GdbVarSetVisualizerCommand : public GdbCommand
{
public:
    GdbVarSetVisualizerCommand(const std::string& name, const std::string& visualizer);
};

class GdbPrintCommand : public GdbCommand
{
public:
    GdbPrintCommand(const std::string& expression);
};

class GdbExamineCommand : public GdbCommand
{
public:
    GdbExamineCommand(int numBytes, char format, char unitSize, uint64_t address);
};

class GdbValue
{
public:
    enum class Kind
    {
        result, string, tuple, list
    };
    GdbValue(Kind kind_);
    GdbValue(const GdbValue&) = delete;
    GdbValue(GdbValue&&) = delete;
    GdbValue& operator=(const GdbValue&) = delete;
    GdbValue& operator=(GdbValue&&) = delete;
    virtual ~GdbValue();
    Kind GetKind() const { return kind; }
    virtual util::JsonValue* ToJson() const = 0;
private:
    Kind kind;
};

class GdbStringValue : public GdbValue
{
public:
    GdbStringValue(const std::string& value_);
    const std::string& Value() const { return value; }
    util::JsonValue* ToJson() const override;
private:
    std::string value;
};

class GdbResult;

class GdbTupleValue : public GdbValue
{
public:
    GdbTupleValue();
    GdbTupleValue(const GdbTupleValue&) = delete;
    GdbTupleValue(GdbTupleValue&&) = delete;
    GdbTupleValue& operator=(const GdbTupleValue&) = delete;
    GdbTupleValue& operator=(GdbTupleValue&&) = delete;
    void AddResult(GdbResult* result);
    const std::vector<std::unique_ptr<GdbResult>>& Results() const { return results; }
    GdbValue* GetField(const std::string& fieldName) const;
    util::JsonValue* ToJson() const override;
private:
    std::vector<std::unique_ptr<GdbResult>> results;
    std::unordered_map<std::string, GdbValue*> fieldMap;
};

class GdbListValue : public GdbValue
{
public:
    GdbListValue();
    GdbListValue(const GdbListValue&) = delete;
    GdbListValue(GdbListValue&&) = delete;
    GdbListValue& operator=(const GdbListValue&) = delete;
    GdbListValue& operator=(GdbListValue&&) = delete;
    void AddValue(GdbValue* value);
    const std::vector<std::unique_ptr<GdbValue>>& Values() const { return values; }
    int Count() const { return values.size(); }
    GdbValue* GetValue(int index) const;
    util::JsonValue* ToJson() const override;
private:
    std::vector<std::unique_ptr<GdbValue>> values;
};

class GdbResult : public GdbValue
{
public:
    GdbResult(const std::string& name_, GdbValue* value_);
    GdbResult(const GdbResult&) = delete;
    GdbResult(GdbResult&&) = delete;
    GdbResult& operator=(const GdbResult&) = delete;
    GdbResult& operator=(GdbResult&&) = delete;
    const std::string& Name() const { return name; }
    GdbValue* Value() const { return value.get(); }
    util::JsonValue* ToJson() const override;
    void AddJsonValueTo(util::JsonObject* jsonObject);
private:
    std::string name;
    std::unique_ptr<GdbValue> value;
};

class GdbResults
{
public:
    GdbResults();
    GdbResults(const GdbResults&) = delete;
    GdbResults(GdbResults&&) = delete;
    GdbResults& operator=(const GdbResults&) = delete;
    GdbResults& operator=(GdbResults&&) = delete;
    void Add(GdbResult* result);
    int Count() const { return results.size(); }
    GdbResult* operator[](int index) const { return results[index].get(); }
    GdbValue* GetField(const std::string& fieldName) const;
    std::unique_ptr<util::JsonValue> ToJson() const;
private:
    std::vector<std::unique_ptr<GdbResult>> results;
    std::unordered_map<std::string, GdbValue*> fieldMap;
};

class GdbReplyRecord
{
public:
    enum class Kind : uint8_t
    {
        result, execRunning, execStopped, notification, consoleOutput, targetOutput, logOutput, prompt, parsingError
    };
    const char* KindStr() const;
    GdbReplyRecord(Kind kind_, GdbResults* results_);
    GdbReplyRecord(const GdbReplyRecord&) = delete;
    GdbReplyRecord(GdbReplyRecord&&) = delete;
    GdbReplyRecord& operator=(const GdbReplyRecord&) = delete;
    GdbReplyRecord& operator=(GdbReplyRecord&&) = delete;
    virtual ~GdbReplyRecord();
    Kind GetKind() const { return kind; }
    virtual bool Stopped() const { return false; }
    virtual bool CommandSucceeded() const { return kind != Kind::parsingError; }
    GdbResults* Results() const { return results.get(); }
    virtual std::unique_ptr<util::JsonValue> ToJson() const;
private:
    Kind kind;
    std::unique_ptr<GdbResults> results;
};

class GdbResultRecord : public GdbReplyRecord
{
public:
    enum class Class : uint8_t
    {
        done, running, connected, error, exit
    };
    GdbResultRecord(Class cls, GdbResults* results);
    const char* ClassStr() const;
    bool CommandSucceeded() const override { return cls != Class::error; }
    Class GetClass() const { return cls; }
    std::unique_ptr<util::JsonValue> ToJson() const override;
private:
    Class cls;
};

class GdbDoneRecord : public GdbResultRecord
{
public:
    GdbDoneRecord(GdbResults* results);
    bool Stopped() const override { return true; }
};

class GdbRunningRecord : public GdbResultRecord
{
public:
    GdbRunningRecord(GdbResults* results);
};

class GdbConnectedRecord : public GdbResultRecord
{
public:
    GdbConnectedRecord(GdbResults* results);
};

class GdbErrorRecord : public GdbResultRecord
{
public:
    GdbErrorRecord(GdbResults* results);
    bool Stopped() const override { return true; }
};

class GdbExitRecord : public GdbResultRecord
{
public:
    GdbExitRecord(GdbResults* results);
    bool Stopped() const override { return true; }
};

class GdbAsyncRecord : public GdbReplyRecord
{
public:
    GdbAsyncRecord(Kind kind, GdbResults* results);
};

class GdbExecRecord : public GdbAsyncRecord
{
public:
    GdbExecRecord(Kind kind, GdbResults* results);
};

class GdbExecRunningRecord : public GdbExecRecord
{
public:
    GdbExecRunningRecord(GdbResults* results);
};

class GdbExecStoppedRecord : public GdbExecRecord
{
public:
    GdbExecStoppedRecord(GdbResults* results);
    bool Stopped() const override { return true; }
};

class GdbNotifyAsyncRecord : public GdbAsyncRecord
{
public:
    GdbNotifyAsyncRecord(const std::string& notification_, GdbResults* results);
    std::unique_ptr<util::JsonValue> ToJson() const override;
private:
    std::string notification;
};

class GdbStreamRecord : public GdbReplyRecord
{
public:
    GdbStreamRecord(Kind kind, const std::string& text_);
    const std::string& Text() const { return text; }
    std::unique_ptr<util::JsonValue> ToJson() const override;
private:
    std::string text;
};

class GdbConsoleOutputRecord : public GdbStreamRecord
{
public:
    GdbConsoleOutputRecord(const std::string& text);
};

class GdbTargetOutputRecord : public GdbStreamRecord
{
public:
    GdbTargetOutputRecord(const std::string& text);
};

class GdbLogOutputRecord : public GdbStreamRecord
{
public:
    GdbLogOutputRecord(const std::string& text);
};

class GdbPrompt : public GdbReplyRecord
{
public:
    GdbPrompt();
};

class GdbParsingError : public GdbReplyRecord
{
public:
    GdbParsingError(const std::string& parsingError_);
    const std::string& ParsingError() const { return parsingError; }
private:
    std::string parsingError;
};

class GdbReply
{
public:
    GdbReply();
    GdbReply(const GdbReply&) = delete;
    GdbReply(GdbReply&&) = delete;
    GdbReply& operator=(const GdbReply&) = delete;
    GdbReply& operator=(GdbReply&&) = delete;
    virtual ~GdbReply();
    void SetTextLines(const std::vector<std::string>& textLines_);
    const std::vector<std::string>& TextLines() const { return textLines; }
    void AddReplyRecord(std::unique_ptr<GdbReplyRecord>&& replyRecord);
    const std::vector<std::unique_ptr<GdbReplyRecord>>& ReplyRecords() const { return replyRecords; }
    GdbResultRecord* GetResultRecord() const { return resultRecord; }
    GdbReplyRecord* GetStoppedRecord() const { return stoppedRecord; }
    std::unique_ptr<util::JsonValue> ToJson() const;
private:
    std::vector<std::string> textLines;
    std::vector<std::unique_ptr<GdbReplyRecord>> replyRecords;
    GdbResultRecord* resultRecord;
    GdbReplyRecord* stoppedRecord;
};

void SetDebugFlag();
void StartGDB(const std::string& executable, const std::vector<std::string>& args, GdbDriver& driver);
GdbReply* GetGDBStartReply();
std::unique_ptr<GdbReply> ExecuteGDBCommand(const GdbCommand& command, GdbDriver& driver);
std::unique_ptr<GdbReply> ReadGDBReply(GdbDriver& driver);
void WriteTargetInputLine(const std::string& line);
void StopGDB(GdbDriver& driver);
void TerminateGDB();

} // namespace cmajor::debug
