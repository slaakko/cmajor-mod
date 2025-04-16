// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.kernel;

import cmajor.systemx.machine;
import std.core;

export namespace cmajor::systemx::kernel {

class MountTable;

class KernelProcess : public cmajor::systemx::machine::Process
{
public:
    int32_t Id() const { return 0; }
    std::string FilePath() const override { return "KERNEL"; }
    void Sleep(std::unique_lock<std::recursive_mutex>& lock) override;
    void Wakeup(cmajor::systemx::machine::Scheduler* scheduler) override;
    cmajor::systemx::machine::Processor* GetProcessor() const override;
    void ReleaseProcessor() override;
    void SetProcessor(cmajor::systemx::machine::Processor* processor) override;
    uint64_t GetINodeKeyOfWorkingDir() const override;
    void SetINodeKeyOfWorkingDir(uint64_t inodeKeyAsULong) override;
    int32_t UID() const override { return 0; }
    int32_t GID() const override { return 0; }
    void SetUID(int32_t uid) override;
    void SetGID(int32_t gid) override;
    int32_t EUID() const override { return 0; }
    int32_t EGID() const override { return 0; }
    void SetEUID(int32_t euid) override;
    void SetEGID(int32_t egid) override;
    int32_t UMask() const override { return 0; }
    void RemoveMessageQueue(int32_t md) override;
    cmajor::systemx::machine::Machine* GetMachine() override;
    bool InKernel() const override { return true; }
    void WaitNotInKernel(bool enter) override;
    void SetInKernel() override;
    void SetNotInKernel() override;
    bool DoSaveContext() const override { return false; }
    void SetSaveContext(bool saveContext_) override;
    void SetRegAX(uint64_t regAX_) override;
    void SetUseRegAX() override;
};

class Kernel
{
public:
    static void Init();
    static void Done();
    static Kernel& Instance() { return *instance; }
    void SetMachine(cmajor::systemx::machine::Machine* machine);
    cmajor::systemx::machine::Machine* GetMachine() const { return machine; }
    void Start();
    void Stop();
    void Sleep(std::unique_lock<std::recursive_mutex>& lock);
    void Wakeup();
    cmajor::systemx::machine::Process* GetKernelProcess() const;
    MountTable& GetMountTable() { return *mountTable; }
    uint64_t GetINodeKeyOfRootDir() const { return inodeKeyOfRootDir; }
    void SetINodeKeyOfRootDir(uint64_t inodeKeyOfRootDir_) { inodeKeyOfRootDir = inodeKeyOfRootDir_; }
private:
    Kernel();
    static std::unique_ptr<Kernel> instance;
    cmajor::systemx::machine::Machine* machine;
    std::unique_ptr<KernelProcess> kernelProcess;
    std::unique_ptr<MountTable> mountTable;
    std::condition_variable_any sleepVar;
    bool wakeup;
    uint64_t inodeKeyOfRootDir;
};

void InitKernel();
void DoneKernel();

} // namespace cmajor::systemx::kernel
