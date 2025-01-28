// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.reg.allocator;

import std.core;

export namespace cmajor::systemx::assembler {

class HexadecimalConstant;

}  // namespace cmajor::systemx::assembler

export namespace cmajor::systemx::intermediate {

class Instruction;
class Context;

enum class Locations : int
{
    none = 0, reg = 1 << 0, frame = 1 << 1
};

inline Locations operator|(Locations left, Locations right)
{
    return Locations(int(left) | int(right));
}

inline Locations operator&(Locations left, Locations right)
{
    return Locations(int(left) & int(right));
}

inline Locations operator~(Locations locs)
{
    return Locations(~int(locs));
}

struct FrameLocation
{
    FrameLocation() : index(-1), offset(0), size(0) {}
    FrameLocation(int index_, int offset_, int size_) : index(index_), offset(offset_), size(size_) {}
    bool Valid() const { return index != -1; }
    bool IsWithinImmediateRange() const { return offset >= 0 && offset < 256; }
    int index;
    int64_t offset;
    int64_t size;
};

class ArgLocation
{
public:
    ArgLocation(int index_);
private:
    friend class CallFrame;
    void SetConstant(cmajor::systemx::assembler::HexadecimalConstant* constant_);
    void SetValue(uint64_t start);
    int index;
    cmajor::systemx::assembler::HexadecimalConstant* constant;
};

class CallFrame
{
public:
    CallFrame();
    CallFrame(const CallFrame&) = delete;
    CallFrame& operator=(const CallFrame&) = delete;
    void NextArgLocation(cmajor::systemx::assembler::HexadecimalConstant* constant);
    int SaveNumLocals() const { return saveNumLocals; }
    void SetSaveNumLocals(int saveNumLocals_) { saveNumLocals = saveNumLocals_; }
    void Resolve(int frameSize);
private:
    int saveNumLocals;
    std::vector<std::unique_ptr<ArgLocation>> argLocations;
};

class Frame
{
public:
    Frame();
    FrameLocation GetFrameLocation(int64_t size);
    int64_t Size() const;
    bool IsWithinWydeRange() const { return Size() < 65536; }
    CallFrame* CurrentCallFrame() { return currentCallFrame.get(); }
    void ResetCallFrame();
    void AddCallFrame();
    void ResolveCallFrames();
private:
    std::vector<FrameLocation> frameLocations;
    std::unique_ptr<CallFrame> currentCallFrame;
    std::vector<std::unique_ptr<CallFrame>> callFrames;
};

enum class RegisterKind : int
{
    none, local, global
};

struct Register
{
    Register() : kind(RegisterKind::none), number(0) {}
    Register(RegisterKind kind_, uint8_t number_) : kind(kind_), number(number_) {}
    bool Valid() const { return kind != RegisterKind::none; }
    bool IsLocal() const { return kind == RegisterKind::local; }
    bool IsGlobal() const { return kind == RegisterKind::global; }
    RegisterKind kind;
    uint8_t number;
};

inline bool operator<(const Register& left, const Register& right)
{
    if (left.kind == RegisterKind::local && right.kind == RegisterKind::global) return true;
    if (left.kind == RegisterKind::global && right.kind == RegisterKind::local) return false;
    return left.number < right.number;
}

class RegisterPool
{
public:
    RegisterPool();
    void AddLocalRegister(const Register& reg);
    Register GetLocalRegister();
    Register GetGlobalRegister(uint8_t number);
    int LocalRegisterCount() const { return localRegisterCount; }
    int NumFreeLocalRegisters() const { return localRegisterPool.size(); }
private:
    int localRegisterCount;
    std::set<Register> localRegisterPool;
    std::map<uint8_t, Register> globalRegisterMap;
};

struct SpillData
{
    Register registerToSpill;;
    FrameLocation spillToFrameLocation;
};

enum class RegisterAllocationAction
{
    none, spill, registerAllocated
};

class RegisterAllocator
{
public:
    virtual ~RegisterAllocator();
    virtual RegisterAllocationAction Run(Instruction* inst) = 0;
    virtual const std::vector<SpillData>& GetSpillData() const = 0;
    virtual Locations GetLocations(Instruction* inst) const = 0;
    virtual Register GetRegister(Instruction* inst) const = 0;
    virtual void AddRegisterLocation(Instruction* inst, const Register& reg) = 0;
    virtual Frame& GetFrame() = 0;
    virtual FrameLocation GetFrameLocation(Instruction* inst) const = 0;
    virtual int LastActiveLocalReg() const = 0;
};

Register GetGlobalRegister(Context* context, uint8_t registerNumber);
Register GetLocalRegister(Context* context);

} // cmajor::systemx::intermediate
