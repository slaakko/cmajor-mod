// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.pass.manager;

import cmajor.systemx.intermediate.pass;
import soul.xml.dom;
import std.core;

export namespace cmajor::systemx::intermediate {

class PassManager
{
public:
    static void Init();
    static void Done();
    static PassManager& Instance() { return *instance; }
    PassManager(const PassManager&) = delete;
    PassManager& operator=(const PassManager&) = delete;
    void Initialize();
    void AddPass(Pass* pass);
    Pass* GetPass(const std::string& passName) const;
    void Run(int logStreamId, Context* context, const std::string& passName, bool verbose);
private:
    PassManager();
    void ReadPassesXml();
    void ReadPassContent(soul::xml::Element* passElement, Pass* pass);
    std::vector<Pass*> CollectPasses(const std::string& passName) const;
    std::vector<FunctionPass*> GetFunctionPasses(const std::vector<Pass*>& passes);
    std::vector<ModulePass*> GetModuleAndCompositePasses(const std::vector<Pass*>& passes);
    void RunFunctionPasses(Context* context, const std::vector<FunctionPass*>& functionPasses);
    void RunModuleAndCompositePasses(Context* context, const std::vector<ModulePass*>& moduleAndCompositePasses);
    static std::unique_ptr<PassManager> instance;
    std::vector<std::unique_ptr<Pass>> passes;
    std::map<std::string, Pass*> passMap;
};

void InitPassManager();
void DonePassManager();

} // cmajor::systemx::intermediate
