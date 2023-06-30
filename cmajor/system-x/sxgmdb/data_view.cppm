// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module sxgmdb.data.view;

import sxgmdb.debug.view;
import cmajor.systemx.machine;
import cmajor.systemx.kernel;
import wing;
import std.core;

export namespace sxgmdb {

class DataRange
{
public:
    DataRange();
    virtual ~DataRange();
    cmajor::systemx::machine::Machine* GetMachine() const { return machine; }
    void SetMachine(cmajor::systemx::machine::Machine* machine_);
    cmajor::systemx::kernel::Process* GetProcess() const { return process; }
    void SetProcess(cmajor::systemx::kernel::Process* process_);
    virtual std::string Name() const = 0;
    virtual int64_t Start() const = 0;
    virtual int64_t Length() const = 0;
private:
    cmajor::systemx::machine::Machine* machine;
    cmajor::systemx::kernel::Process* process;
};

class DataSegmentDataRange : public DataRange
{
public:
    std::string Name() const override { return "Data"; }
    int64_t Start() const override;
    int64_t Length() const override;
};

class ArgumentsDataRange : public DataRange
{
public:
    std::string Name() const override { return "Arguments"; }
    int64_t Start() const override;
    int64_t Length() const override;
};

class EnvironmentDataRange : public DataRange
{
public:
    std::string Name() const override { return "Environment"; }
    int64_t Start() const override;
    int64_t Length() const override;
};

class HeapDataRange : public DataRange
{
public:
    std::string Name() const override { return "Heap"; }
    int64_t Start() const override;
    int64_t Length() const override;
};

class StackDataRange : public DataRange
{
public:
    std::string Name() const override { return "Stack"; }
    int64_t Start() const override;
    int64_t Length() const override;
};

class DataRanges
{
public:
    DataRanges();
    const std::vector<std::unique_ptr<DataRange>>& Ranges() const { return ranges; }
    DataRange* GetDataRange(const std::string& rangeName) const;
    void SetMachine(cmajor::systemx::machine::Machine* machine);
    void SetProcess(cmajor::systemx::kernel::Process* process);
private:
    std::vector<std::unique_ptr<DataRange>> ranges;
    std::map<std::string, DataRange*> rangeMap;
};

std::string DefaultDataViewFontFamilyName();
float DefaultDataViewFontSize();
wing::Color DefaultDataViewAddressColor();
wing::Color DefaultDataViewOctaColor();
wing::Color DefaultDataViewByteColor();
wing::Color DefaultDataViewCharColor();

struct DataViewCreateParams
{
    DataViewCreateParams();
    DataViewCreateParams& Defaults();
    wing::ControlCreateParams controlCreateParams;
    std::string fontFamilyName;
    float fontSize;
    wing::Color addressColor;
    wing::Color octaColor;
    wing::Color byteColor;
    wing::Color charColor;
};

class DataView : public wing::Control, public DebugView
{
public:
    DataView(DataViewCreateParams& createParams, DataRange* dataRange_);
    void SetMachine(cmajor::systemx::machine::Machine* machine_) override;
    void SetProcess(cmajor::systemx::kernel::Process* process_) override;
    void UpdateView(bool updateCurrentAddress) override;
    void NextLine() override;
    void PrevLine() override;
    void NextQuarter() override;
    void PrevQuarter() override;
    void NextPage() override;
    void PrevPage() override;
    void ToStart() override;
    void ToEnd() override;
protected:
    void OnPaint(wing::PaintEventArgs& args) override;
private:
    void Measure(wing::Graphics& graphics);
    void Paint(wing::Graphics& graphics);
    void PrintValue(const wing::PointF& origin, wing::Graphics& graphics, uint64_t address, uint64_t value,
        const wing::SolidBrush& addressBrush, const wing::SolidBrush& octaBrush, const wing::SolidBrush& byteBrush, const wing::SolidBrush& charBrush);
    DataRange* dataRange;
    wing::StringFormat stringFormat;
    wing::SolidBrush addressBrush;
    wing::SolidBrush octaBrush;
    wing::SolidBrush byteBrush;
    wing::SolidBrush charBrush;
    float lineHeight;
    float charWidth;
    int numLines;
    int64_t currentAddress;
};

} // namespace sxgmdb
