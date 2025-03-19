// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.sbin.assembly.assembler;

import cmajor.sbin.assembly.asm_file_parser;

namespace cmajor::sbin::assembly {

Assembler::Assembler(const std::string& asmFilePath_, int logStreamId_, bool verbose_, bool printLines_) :
    verbose(verbose_), printLines(printLines_), logStreamId(logStreamId_), fileMap(), 
    asmFilePath(asmFilePath_), objFilePath(util::Path::ChangeExtension(asmFilePath, ".obj")),
    objectFile(new cmajor::sbin::coff::CoffObjectFile()), codeSection(nullptr), dataSection(nullptr), currentCodeStream(nullptr),
    currentSection(nullptr), emitOperation(EmitOperation::emitCode), dataInstKind(DataInstKind::db), operandKind(NodeKind::none), 
    reg(cmajor::sbin::machine_x64::Register::none), contentReg0(cmajor::sbin::machine_x64::Register::none), 
    contentReg1(cmajor::sbin::machine_x64::Register::none), contentReg2(cmajor::sbin::machine_x64::Register::none),
    indexReg(cmajor::sbin::machine_x64::Register::none), baseReg(cmajor::sbin::machine_x64::Register::none), 
    negate(false), immediate(0), displacement(0), scale(0u), content(false), symbolOperand(nullptr), functionSymbol(nullptr),
    symbol0(nullptr), symbol1(nullptr)
{
}

void Assembler::Assemble()
{
    asmFile = ParseAsmFile(logStreamId, asmFilePath, verbose, fileMap);
    codeSection = cmajor::sbin::coff::MakeCodeSection(objectFile.get());
    objectFile->AddSection(codeSection);
    dataSection = cmajor::sbin::coff::MakeDataSection(objectFile.get());
    objectFile->AddSection(dataSection);
    MakeSymbols();
    EmitData();
    EmitCode();
    ResolveJumps();
    ResolveSymbolDifferences();
    dataSection->SetData(dataStream.ReleaseContent());
    codeSection->SetData(codeStream.ReleaseContent());
    WriteObjectFile();
    if (verbose)
    {
        util::LogMessage(logStreamId, "==> " + objFilePath);
    }
}

void Assembler::WriteObjectFile()
{
    objectFile->Write(objFilePath);
}

void Assembler::EmitData()
{
    emitOperation = EmitOperation::emitData;
    currentSection = dataSection;
    currentWriter.reset(new util::LittleEndianBinaryStreamWriter(dataStream));
    for (const auto& dataDefinition : asmFile->DataDefinitions())
    {
        dataDefinition->Accept(*this);
    }
    dataStream.SetFromContent();
}

void Assembler::EmitCode()
{
    emitOperation = EmitOperation::emitCode;
    currentSection = codeSection;
    currentWriter.reset(new util::LittleEndianBinaryStreamWriter(codeStream));
    currentCodeStream = &codeStream;
    currentJmpPositions = &jmpPositions;
    currentSymbolDifferences = &symbolDifferences;
    for (const auto& definition : asmFile->Definitions())
    {
        definition->Accept(*this);
    }
    codeStream.SetFromContent();
}

Symbol* Assembler::GetSymbol(const std::string& symbolName) const
{
    auto it = symbolMap.find(symbolName);
    if (it != symbolMap.end())
    {
        return it->second;
    }
    return nullptr;
}

void Assembler::AddSymbol(Symbol* symbol)
{
    symbolMap[symbol->Name()] = symbol;
    symbols.push_back(std::unique_ptr<Symbol>(symbol));
}

void Assembler::EmitByte(uint8_t x)
{
    currentWriter->Write(static_cast<uint8_t>(x));
}

void Assembler::EmitWord(uint16_t x)
{
    currentWriter->Write(static_cast<uint16_t>(x));
}

void Assembler::EmitDword(uint32_t x)
{
    currentWriter->Write(static_cast<uint32_t>(x));
}

void Assembler::EmitQword(uint64_t x)
{
    currentWriter->Write(static_cast<uint64_t>(x));
}

void Assembler::EmitFloat(float x)
{
    currentWriter->Write(x);
}

void Assembler::EmitDouble(double x)
{
    currentWriter->Write(x);
}

void Assembler::EmitCallFunctionSymbol(Symbol* symbol)
{
    cmajor::sbin::machine_x64::EmitCallNear(*this);
    int64_t pos = currentWriter->Position();
    currentSection->GetRelocationTable()->AddRelocation(cmajor::sbin::coff::Relocation(static_cast<uint32_t>(pos), symbol->Entry()->Index(), 
        cmajor::sbin::coff::IMAGE_REL_AMD64_REL32));
    EmitDword(static_cast<uint32_t>(0u));
}

void Assembler::EmitMovSdSymbol(cmajor::sbin::machine_x64::Register reg, Symbol* symbol, const soul::ast::Span& span)
{
    cmajor::sbin::machine_x64::EmitMovSdXmmRegNear(*this, reg, span);
    int64_t pos = currentWriter->Position();
    currentSection->GetRelocationTable()->AddRelocation(cmajor::sbin::coff::Relocation(static_cast<uint32_t>(pos), symbol->Entry()->Index(),
        cmajor::sbin::coff::IMAGE_REL_AMD64_REL32));
    EmitDword(static_cast<uint64_t>(0u));
}

void Assembler::EmitMovSsSymbol(cmajor::sbin::machine_x64::Register reg, Symbol* symbol, const soul::ast::Span& span)
{
    cmajor::sbin::machine_x64::EmitMovSsXmmRegNear(*this, reg, span);
    int64_t pos = currentWriter->Position();
    currentSection->GetRelocationTable()->AddRelocation(cmajor::sbin::coff::Relocation(static_cast<uint32_t>(pos), symbol->Entry()->Index(),
        cmajor::sbin::coff::IMAGE_REL_AMD64_REL32));
    EmitDword(static_cast<uint64_t>(0u));
}

void Assembler::EmitLeaSymbol(cmajor::sbin::machine_x64::Register reg, Symbol* symbol, const soul::ast::Span& span)
{
    if (symbol->Entry())
    {
        cmajor::sbin::machine_x64::EmitLeaNear(*this, reg, span);
        int64_t pos = currentWriter->Position();
        currentSection->GetRelocationTable()->AddRelocation(cmajor::sbin::coff::Relocation(static_cast<uint32_t>(pos), symbol->Entry()->Index(),
            cmajor::sbin::coff::IMAGE_REL_AMD64_REL32));
        EmitDword(static_cast<uint32_t>(0u));
    }
    else
    {
        cmajor::sbin::machine_x64::EmitLeaNear(*this, reg, span);
        int64_t pos = currentWriter->Position();
        AddJmpPos(pos, symbol, span);
        EmitDword(static_cast<uint32_t>(0u));
    }
}

void Assembler::EmitMovReg64Symbol(cmajor::sbin::machine_x64::Register reg, Symbol* symbol, const soul::ast::Span& span)
{
    cmajor::sbin::machine_x64::EmitMovReg64Offset(*this, reg, span);
    int64_t pos = currentWriter->Position();
    if (symbol->Entry())
    {
        currentSection->GetRelocationTable()->AddRelocation(cmajor::sbin::coff::Relocation(static_cast<uint32_t>(pos), symbol->Entry()->Index(),
            cmajor::sbin::coff::IMAGE_REL_AMD64_ADDR64));
    }
    else
    {
        throw std::runtime_error("entry required");
    }
    EmitQword(static_cast<uint64_t>(0u));
}

void Assembler::EmitJmp(Symbol* symbol, const soul::ast::Span& span)
{
    cmajor::sbin::machine_x64::EmitJmpNear(*this);
    int64_t pos = currentWriter->Position();
    AddJmpPos(pos, symbol, span);
    EmitDword(static_cast<uint32_t>(0u));
}

void Assembler::EmitJe(Symbol* symbol, const soul::ast::Span& span)
{
    cmajor::sbin::machine_x64::EmitJeNear(*this);
    int64_t pos = currentWriter->Position();
    AddJmpPos(pos, symbol, span);
    EmitDword(static_cast<uint32_t>(0u));
}

void Assembler::EmitJne(Symbol* symbol, const soul::ast::Span& span)
{
    cmajor::sbin::machine_x64::EmitJneNear(*this);
    int64_t pos = currentWriter->Position();
    AddJmpPos(pos, symbol, span);
    EmitDword(static_cast<uint32_t>(0u));
}

void Assembler::EmitJae(Symbol* symbol, const soul::ast::Span& span)
{
    cmajor::sbin::machine_x64::EmitJaeNear(*this);
    int64_t pos = currentWriter->Position();
    AddJmpPos(pos, symbol, span);
    EmitDword(static_cast<uint32_t>(0u));
}

void Assembler::AddJmpPos(int64_t pos, Symbol* symbol, const soul::ast::Span& span)
{
    currentJmpPositions->push_back(JmpPos(pos, symbol, span));
}

void Assembler::ResolveJumps()
{
    for (const auto& jmpPos : *currentJmpPositions)
    {
        int64_t pos = jmpPos.pos;
        Symbol* symbol = jmpPos.symbol;
        if (symbol->HasValue())
        {
            int64_t spos = static_cast<int64_t>(symbol->Value());
            int64_t offset64 = spos - (pos + 4);
            if (std::abs(offset64) > std::numeric_limits<int32_t>::max())
            {
                ThrowError("jump too far", jmpPos.span);
            }
            int32_t offset = static_cast<int32_t>(offset64);
            util::LittleEndianMemoryWriter jmpOffsetWriter(currentCodeStream->Data() + pos, 4);
            jmpOffsetWriter.Write(static_cast<int32_t>(offset));
        }
        else
        {
            ThrowError("value of symbol '" + symbol->Name() + "' not defined", jmpPos.span);
        }
    }
}

void Assembler::ResolveSymbolDifferences()
{
    for (const auto& symbolDifference : *currentSymbolDifferences)
    {
        int64_t pos = symbolDifference.pos;
        int64_t s0pos = 0;
        int64_t s1pos = 0;
        Symbol* symbol0 = symbolDifference.symbol0;
        if (symbol0->HasValue())
        {
            s0pos = static_cast<int64_t>(symbol0->Value());
        }
        else
        {
            ThrowError("symbol 0 has no value", symbolDifference.span);
        }
        Symbol* symbol1 = symbolDifference.symbol1;
        if (symbol1->HasValue())
        {
            s1pos = static_cast<int64_t>(symbol1->Value());
        }
        else
        {
            ThrowError("symbol 1 has no value", symbolDifference.span);
        }
        uint32_t difference = static_cast<uint32_t>(s0pos - s1pos);
        util::LittleEndianMemoryWriter differenceWriter(currentCodeStream->Data() + pos, 4);
        differenceWriter.Write(static_cast<uint32_t>(difference));
    }
}

void Assembler::MakeSymbols()
{
    for (const auto& declaration : asmFile->Declarations())
    {
        Symbol* symbol = GetSymbol(declaration->Symbol()->Name());
        if (symbol)
        {
            ThrowError("symbol already declared", declaration->Span());
        }
        else
        {
            symbol = new Symbol(declaration->GetSymbolKind(), declaration->GetType(), declaration->Symbol()->Name());
            switch (declaration->GetType())
            {
                case Type::proc:
                {
                    symbol->SetKindFlag(SymbolKind::code);
                    break;
                }
                case Type::byte_:
                case Type::word:
                case Type::dword:
                case Type::qword:
                case Type::oword:
                {
                    symbol->SetKindFlag(SymbolKind::data);
                    break;
                }
            }
            AddSymbol(symbol);
        }
    }
    for (const auto& dataDefinition : asmFile->DataDefinitions())
    {
        LabelNode* label = dataDefinition->Label();
        if (label)
        {
            Symbol* symbol = GetSymbol(label->Label());
            if (symbol)
            {
                symbol->SetKindFlag(SymbolKind::data);
            }
            else
            {
                symbol = new Symbol(SymbolKind::data, Type::none, label->Label());
                AddSymbol(symbol);
            }
        }
    }
    for (const auto& definition : asmFile->Definitions())
    {
        Symbol* symbol = GetSymbol(definition->Symbol()->Name());
        if (definition->IsFunctionDefinitionNode())
        {
            functionSymbol = symbol;
            FunctionDefinitionNode* functionDefinitionNode = static_cast<FunctionDefinitionNode*>(definition.get());
            if (functionDefinitionNode->Symbol()->Name() != functionDefinitionNode->EndpSymbol()->Name())
            {
                ThrowError("ENDP symbol name does not match PROC symbol name", definition->Span());
            }
            if (symbol)
            {
                if (symbol->IsKind(SymbolKind::data) || symbol->IsKind(SymbolKind::macro))
                {
                    ThrowError("Code symbol expected. Note: previous definition was data or macro", definition->Span());
                }
                symbol->SetKindFlag(SymbolKind::code);
            }
            else
            {
                symbol = new Symbol(SymbolKind::code, Type::proc, definition->Symbol()->Name());
                AddSymbol(symbol);
                functionSymbol = symbol;
            }
            for (const auto& inst : functionDefinitionNode->Instructions())
            {
                if (inst->Label())
                {
                    Symbol* symbol = functionSymbol->GetSubSymbol(inst->Label()->Label());
                    if (symbol)
                    {
                        ThrowError("symbol '" + symbol->Name() + "' already defined", inst->Span());
                    }
                    symbol = new Symbol(SymbolKind::label, Type::none, inst->Label()->Label());
                    functionSymbol->AddSubSymbol(symbol);
                }
            }
        }
        else if (definition->IsMacroDefinitionNode())
        {
            if (!symbol)
            {
                symbol = new Symbol(SymbolKind::macro, Type::none, definition->Symbol()->Name());
                AddSymbol(symbol);
            }
            else if (!symbol->IsKind(SymbolKind::macro))
            {
                ThrowError("Nacro symbol expected. Note: previous definition was not a macro", definition->Span());
            }
        }
    }
    cmajor::sbin::coff::SymbolTableEntry* codeSectionEntry = new cmajor::sbin::coff::SymbolTableEntry();
    codeSectionEntry->SetName(".text", objectFile.get());
    codeSectionEntry->SetSectioNumber(codeSection->GetSectionHeader()->Number());
    codeSectionEntry->SetType(cmajor::sbin::coff::MakeSymbolTypeNull());
    codeSectionEntry->SetStorageClass(cmajor::sbin::coff::IMAGE_SYM_CLASS_STATIC);
    objectFile->GetSymbolTable()->AddEntry(codeSectionEntry);
    cmajor::sbin::coff::SymbolTableEntry* dataSectionEntry = new cmajor::sbin::coff::SymbolTableEntry();
    dataSectionEntry->SetName(".data", objectFile.get());
    dataSectionEntry->SetSectioNumber(dataSection->GetSectionHeader()->Number());
    dataSectionEntry->SetType(cmajor::sbin::coff::MakeSymbolTypeNull());
    dataSectionEntry->SetStorageClass(cmajor::sbin::coff::IMAGE_SYM_CLASS_STATIC);
    objectFile->GetSymbolTable()->AddEntry(dataSectionEntry);
    for (const auto& symbol : symbols)
    {
        if (symbol->IsKind(SymbolKind::public_) && symbol->IsKind(SymbolKind::code))
        {
            cmajor::sbin::coff::SymbolTableEntry* entry = cmajor::sbin::coff::MakeInternalFunctionSymbolTableEntry(
                codeSection->GetSectionHeader()->Number(), 
                symbol->Name(), 
                objectFile.get());
            objectFile->GetSymbolTable()->AddEntry(entry);
            symbol->SetEntry(entry);
        }
        else if (symbol->IsKind(SymbolKind::linkOnce) && symbol->IsKind(SymbolKind::code))
        {
            cmajor::sbin::coff::Section* comdatSection = cmajor::sbin::coff::MakeComdatSection(objectFile.get());
            objectFile->AddSection(comdatSection);
            cmajor::sbin::coff::SymbolTableEntry* sectionDefinitionEntry = cmajor::sbin::coff::MakeSectionDefinitionSymbolTableEntry(
                comdatSection->GetSectionHeader()->Number(), objectFile.get());
            objectFile->GetSymbolTable()->AddEntry(sectionDefinitionEntry);
            cmajor::sbin::coff::SymbolTableEntry* entry = cmajor::sbin::coff::MakeInternalFunctionSymbolTableEntry(
                comdatSection->GetSectionHeader()->Number(),
                symbol->Name(),
                objectFile.get());
            objectFile->GetSymbolTable()->AddEntry(entry);
            symbol->SetEntry(entry);
            symbol->SetSectionDefinitionEntry(sectionDefinitionEntry);
            symbol->SetSection(comdatSection);
        }
        else if (symbol->IsKind(SymbolKind::external) && symbol->IsKind(SymbolKind::code))
        {
            cmajor::sbin::coff::SymbolTableEntry* entry = cmajor::sbin::coff::MakeExternalFunctionSymbolTableEntry(symbol->Name(), objectFile.get());
            objectFile->GetSymbolTable()->AddEntry(entry);
            symbol->SetEntry(entry);
        }
        else if (symbol->IsKind(SymbolKind::external) && symbol->IsKind(SymbolKind::data))
        {
            cmajor::sbin::coff::SymbolTableEntry* entry = cmajor::sbin::coff::MakeExternalDataSymbolTableEntry(symbol->Name(), objectFile.get());
            objectFile->GetSymbolTable()->AddEntry(entry);
            symbol->SetEntry(entry);
        }
        else if (symbol->IsKind(SymbolKind::data))
        {
            cmajor::sbin::coff::SymbolTableEntry* entry = cmajor::sbin::coff::MakeInternalDataSymbolTableEntry(
                dataSection->GetSectionHeader()->Number(),
                symbol->Name(),
                objectFile.get());
            objectFile->GetSymbolTable()->AddEntry(entry);
            symbol->SetEntry(entry);
        }
    }
}

void Assembler::Visit(SymbolNode& symbolNode)
{
    Symbol* symbol = GetSymbol(symbolNode.Name());
    if (symbol)
    {
        if (symbol->IsKind(SymbolKind::macro))
        {
            Node* node = symbol->GetNode();
            if (node)
            {
                node->Accept(*this);
            }
            else
            {
                ThrowError("node not set for macro symbol '" + symbolNode.Name() + "'", symbolNode.Span());
            }
        }
        else
        {
            operandKind = NodeKind::symbolNode;
            symbolOperand = symbol;
            Node* node = symbol->GetNode();
            if (!node)
            {
                symbol->SetNode(&symbolNode);
            }
        }
    }
    else if (functionSymbol)
    {
        symbol = functionSymbol->GetSubSymbol(symbolNode.Name());
        if (symbol)
        {
            operandKind = NodeKind::symbolNode;
            symbolOperand = symbol;
            Node* node = symbol->GetNode();
            if (!node)
            {
                symbol->SetNode(&symbolNode);
            }
        }
        else
        {
            ThrowError("symbol '" + symbolNode.Name() + "' not found", symbolNode.Span());
        }
    }
    else
    {
        ThrowError("symbol '" + symbolNode.Name() + "' not found", symbolNode.Span());
    }
}

void Assembler::Visit(DataDefinitionNode& dataDefinitionNode)
{
    if (dataDefinitionNode.Label())
    {
        LabelNode* label = dataDefinitionNode.Label();
        Symbol* symbol = GetSymbol(label->Label());
        if (symbol)
        {
            symbol->SetValue(currentWriter->Position());
        }
        else if (functionSymbol)
        {
            Symbol* symbol = functionSymbol->GetSubSymbol(label->Label());
            if (symbol)
            {
                symbol->SetValue(currentWriter->Position());
            }
            else
            {
                ThrowError("Label symbol '" + label->Label() + "' not defined", dataDefinitionNode.Span());
            }

        }
    }
    EmitOperation prevOperation = emitOperation;
    emitOperation = EmitOperation::emitData;
    dataInstKind = dataDefinitionNode.GetDataInstKind();
    for (const auto& operand : dataDefinitionNode.Operands())
    {
        operandKind = NodeKind::none;
        operand->Accept(*this);
        if (operandKind == NodeKind::symbolDifferenceNode)
        {
            currentSymbolDifferences->push_back(SymbolDifference(currentWriter->Position(), symbol0, symbol1, dataDefinitionNode.Span()));
            switch (dataInstKind)
            {
                case DataInstKind::dq:
                {
                    EmitQword(0);
                    break;
                }
                case DataInstKind::dd:
                {
                    EmitDword(0);
                    break;
                }
                case DataInstKind::dw:
                {
                    EmitWord(0);
                    break;
                }
                case DataInstKind::db:
                {
                    EmitByte(0);
                    break;
                }
            }
        }
        else if (operandKind == NodeKind::symbolNode)
        {
            if (dataInstKind == DataInstKind::dq)
            {
                Symbol* symbol = symbolOperand;
                cmajor::sbin::coff::SymbolTableEntry* entry = symbol->Entry();
                uint32_t symbolTableIndex = entry->Index();
                uint32_t address = static_cast<uint32_t>(currentWriter->Position());
                cmajor::sbin::coff::Relocation relocation(address, symbolTableIndex, cmajor::sbin::coff::IMAGE_REL_AMD64_ADDR64);
                dataSection->GetRelocationTable()->AddRelocation(relocation);
            }
            else
            {
                ThrowError("DQ expected", dataDefinitionNode.Span());
            }
            switch (dataInstKind)
            {
                case DataInstKind::dq:
                {
                    EmitQword(0);
                    break;
                }
                case DataInstKind::dd:
                {
                    EmitDword(0);
                    break;
                }
                case DataInstKind::dw:
                {
                    EmitWord(0);
                    break;
                }
                case DataInstKind::db:
                {
                    EmitByte(0);
                    break;
                }
            }
        }
    }
    emitOperation = prevOperation;
}

void Assembler::Visit(BinaryExprNode& binaryExprNode)
{
    if (emitOperation == EmitOperation::emitCode)
    {
        if (!content)
        {
            ThrowError("content expression expected", binaryExprNode.Span());
        }
        switch (binaryExprNode.Op())
        {
            case Operator::plus:
            {
                reg = sbin::machine_x64::Register::none;
                binaryExprNode.Left()->Accept(*this);
                if (operandKind == NodeKind::registerNode)
                {
                    contentReg1 = reg;
                }
                else if (operandKind == NodeKind::sibNode)
                {
                    baseReg = reg;
                    reg = sbin::machine_x64::Register::none;
                    operandKind = NodeKind::sibNode;
                    return;
                }
                else
                {
                    ThrowError("register operand expected", binaryExprNode.Left()->Span());
                }
                reg = sbin::machine_x64::Register::none;
                binaryExprNode.Right()->Accept(*this);
                if (operandKind == NodeKind::registerNode)
                {
                    contentReg2 = reg;
                }
                else if (operandKind == NodeKind::sibNode)
                {
                    baseReg = contentReg1;
                    reg = sbin::machine_x64::Register::none;
                    operandKind = NodeKind::sibNode;
                    return;
                }
                else if (operandKind == NodeKind::immediateNode)
                {
                    displacement = static_cast<int32_t>(immediate);
                    immediate = 0;
                }
                reg = sbin::machine_x64::Register::none;
                operandKind = NodeKind::binaryExprNode;
                break;
            }
            case Operator::minus:
            {
                break;
            }
            case Operator::times:
            {
                scale = 0u;
                indexReg = cmajor::sbin::machine_x64::Register::none;
                binaryExprNode.Left()->Accept(*this);
                if (operandKind == NodeKind::immediateNode)
                {
                    scale = static_cast<uint8_t>(immediate);
                }
                else if (operandKind == NodeKind::registerNode)
                {
                    indexReg = reg;
                }
                binaryExprNode.Right()->Accept(*this);
                if (operandKind == NodeKind::immediateNode)
                {
                    scale = static_cast<uint8_t>(immediate);
                }
                else if (operandKind == NodeKind::registerNode)
                {
                    indexReg = reg;
                }
                operandKind = NodeKind::sibNode;
                break;
            }
        }
    }
    else if (emitOperation == EmitOperation::emitData)
    {
        switch (binaryExprNode.Op())
        {
            case Operator::minus:
            {
                binaryExprNode.Left()->Accept(*this);
                if (operandKind == NodeKind::symbolNode)
                {
                    symbol0 = symbolOperand;
                }
                else
                {
                    ThrowError("symbol opereand expected", binaryExprNode.Left()->Span());
                }
                binaryExprNode.Right()->Accept(*this);
                if (operandKind == NodeKind::symbolNode)
                {
                    symbol1 = symbolOperand;
                }
                else
                {
                    ThrowError("symbol opereand expected", binaryExprNode.Left()->Span());
                }
                operandKind = NodeKind::symbolDifferenceNode;
                break;
            }
            default:
            {
                ThrowError("difference expression expected", binaryExprNode.Left()->Span());
            }
        }
    }
}

void Assembler::Visit(UnaryExprNode& unaryExprNode)
{
    if (unaryExprNode.Op() == Operator::minus)
    {
        negate = true;
    }
    unaryExprNode.Operand()->Accept(*this);
    negate = false;
}

void Assembler::Visit(RegisterNode& registerNode)
{
    operandKind = NodeKind::registerNode;
    reg = registerNode.Reg();
}

void Assembler::Visit(ContentExprNode& contentExprNode)
{
    content = true;
    contentExprNode.Operand()->Accept(*this);
}

void Assembler::Visit(SizeExprNode& sizeExprNode)
{
    sizeExprNode.Operand()->Accept(*this);
}

void Assembler::Visit(ParenthesizedExprNode& parenthesizedExprNode)
{
    parenthesizedExprNode.Operand()->Accept(*this);
}

void Assembler::ProcessIntegerValue(uint64_t value, const soul::ast::Span& span)
{
    if (emitOperation == EmitOperation::emitData)
    {
        switch (dataInstKind)
        {
            case DataInstKind::db:
            {
                if (negate)
                {
                    EmitByte(static_cast<uint8_t>(-static_cast<int64_t>(value)));
                }
                else
                {
                    EmitByte(static_cast<uint8_t>(value));
                }
                break;
            }
            case DataInstKind::dw:
            {
                if (negate)
                {
                    EmitWord(static_cast<uint16_t>(-static_cast<int64_t>(value)));
                }
                else
                {
                    EmitWord(static_cast<uint16_t>(value));
                }
                break;
            }
            case DataInstKind::dd:
            {
                if (negate)
                {
                    EmitDword(static_cast<uint32_t>(-static_cast<int64_t>(value)));
                }
                else
                {
                    EmitDword(static_cast<uint32_t>(value));
                }
                break;
            }
            case DataInstKind::dq:
            {
                if (negate)
                {
                    EmitQword(static_cast<uint64_t>(-static_cast<int64_t>(value)));
                }
                else
                {
                    EmitQword(static_cast<uint64_t>(value));
                }
                break;
            }
            default:
            {
                ThrowError("DB, DW, DD or DQ instruction expected for value", span);
                break;
            }
        }
    }
    else if (emitOperation == EmitOperation::emitCode)
    {
        operandKind = NodeKind::immediateNode;
        if (negate)
        {
            immediate = -static_cast<int64_t>(value);
        }
        else
        {
            immediate = static_cast<int64_t>(value);
        }
    }
}

void Assembler::Visit(HexNumberNode& hexNumberNode)
{
    ProcessIntegerValue(hexNumberNode.Value(), hexNumberNode.Span());
}

void Assembler::Visit(IntegerNode& integerNode)
{
    ProcessIntegerValue(integerNode.Value(), integerNode.Span());
}

void Assembler::Visit(RealNode& realNode)
{
    double value = realNode.Value();
    if (negate)
    {
        value = -value;
    }
    switch (dataInstKind)
    {
        case DataInstKind::real8:
        {
            EmitDouble(value);
            break;
        }
        case DataInstKind::real4:
        {
            float floatValue = static_cast<float>(value);
            EmitFloat(floatValue);
            break;
        }
    }
}

void Assembler::Visit(StringNode& stringNode)
{
    if (dataInstKind == DataInstKind::db)
    {
        for (auto c : stringNode.Value())
        {
            EmitByte(static_cast<uint8_t>(c));
        }
    }
    else
    {
        ThrowError("DB expected for string operands", stringNode.Span());
    }
}

void Assembler::ThrowError(const std::string& errorMessage, const soul::ast::Span& span)
{
    //std::cout << "SPAN pos=" + std::to_string(span.pos) << "\n";
    std::string msg;
    msg.append(errorMessage);
    msg.append(": file=" + asmFilePath);
    if (asmFile)
    {
        const std::vector<int>* lineStartIndeces = fileMap.LineStartIndeces(asmFile->FileIndex());
        if (lineStartIndeces)
        {
            soul::ast::LineColLen lineColLen = soul::ast::SpanToLineColLen(span, *lineStartIndeces);
            msg.append(", line=" + std::to_string(lineColLen.line));
            std::u32string line = fileMap.GetFileLine(asmFile->FileIndex(), lineColLen.line);
            msg.append("\n").append(util::ToUtf8(line)).append("\n");
            msg.append(lineColLen.col - 1, ' ').append(lineColLen.len, '^').append("\n");
        }
    }
    throw std::runtime_error(msg);
}

void Assembler::Visit(InstructionNode& instructionNode)
{
    soul::ast::Span span = instructionNode.Span();
    if (printLines)
    {
        const std::vector<int>* lineStartIndeces = fileMap.LineStartIndeces(asmFile->FileIndex());
        if (lineStartIndeces)
        {
            soul::ast::LineColLen lineColLen = soul::ast::SpanToLineColLen(span, *lineStartIndeces);
            std::cout << "line " << lineColLen.line << "\n";
        }
    }
    LabelNode* label = instructionNode.Label();
    if (label)
    {
        if (functionSymbol)
        {
            Symbol* symbol = functionSymbol->GetSubSymbol(label->Label());
            if (symbol)
            {
                symbol->SetValue(currentWriter->Position());
            }
            else
            {
                ThrowError("Label symbol '" + label->Label() + "' not defined", instructionNode.Span());
            }
        }
    }
    operandKind = NodeKind::none;
    NodeKind operand0Kind = NodeKind::none;
    NodeKind operand1Kind = NodeKind::none;
    cmajor::sbin::machine_x64::Register reg0 = cmajor::sbin::machine_x64::Register::none;
    cmajor::sbin::machine_x64::Register reg1 = cmajor::sbin::machine_x64::Register::none;
    int64_t immediate0 = 0;
    int64_t immediate1 = 0;
    int32_t displacement0 = 0;
    int32_t displacement1 = 0;
    contentReg0 = cmajor::sbin::machine_x64::Register::none;
    contentReg1 = cmajor::sbin::machine_x64::Register::none;
    contentReg2 = cmajor::sbin::machine_x64::Register::none;
    symbolOperand = nullptr;
    Symbol* symbol0 = nullptr;
    Symbol* symbol1 = nullptr;
    int n = static_cast<int>(instructionNode.Operands().size());
    for (int i = 0; i < n; ++i)
    {
        Node* operandNode = instructionNode.Operands()[i].get();
        operandKind = NodeKind::none;
        content = false;
        displacement = 0;
        operandNode->Accept(*this);
        switch (operandKind)
        {
            case NodeKind::symbolNode:
            {
                switch (i)
                {
                    case 0:
                    {
                        operand0Kind = operandKind;
                        symbol0 = symbolOperand;
                        break;
                    }
                    case 1:
                    {
                        operand1Kind = operandKind;
                        symbol1 = symbolOperand;
                        break;
                    }
                    default:
                    {
                        ThrowError("invalid number of operands", instructionNode.Span());
                        break;
                    }
                }
                break;
            }
            case NodeKind::binaryExprNode:
            {
                switch (i)
                {
                    case 0:
                    {
                        operand0Kind = operandKind;
                        displacement0 = displacement;
                        break;
                    }
                    case 1:
                    {
                        operand1Kind = operandKind;
                        displacement1 = displacement;
                        break;
                    }
                    default:
                    {
                        ThrowError("invalid number of operands", instructionNode.Span());
                        break;
                    }
                }
                break;
            }
            case NodeKind::sibNode:
            {
                switch (i)
                {
                    case 0:
                    {
                        operand0Kind = operandKind;
                        break;
                    }
                    case 1:
                    {
                        operand1Kind = operandKind;
                        break;
                    }
                    default:
                    {
                        ThrowError("invalid number of operands", instructionNode.Span());
                        break;
                    }
                }
                break;
            }
            case NodeKind::registerNode:
            {
                switch (i)
                {
                    case 0:
                    {
                        if (content)
                        {
                            operand0Kind = NodeKind::contentExprNode;
                            contentReg0 = reg;
                        }
                        else
                        {
                            operand0Kind = operandKind;
                            reg0 = reg;
                        }
                        break;
                    }
                    case 1:
                    {
                        if (content)
                        {
                            operand1Kind = NodeKind::contentExprNode;
                            contentReg1 = reg;
                        }
                        else
                        {
                            operand1Kind = operandKind;
                            reg1 = reg;
                        }
                        break;
                    }
                    default:
                    {
                        ThrowError("invalid number of operands", instructionNode.Span());
                        break;
                    }
                }
                break;
            }
            case NodeKind::immediateNode:
            {
                switch (i)
                {
                    case 0:
                    {
                        operand0Kind = operandKind;
                        immediate0 = immediate;
                        break;
                    }
                    case 1:
                    {
                        operand1Kind = operandKind;
                        immediate1 = immediate;
                        break;
                    }
                    default:
                    {
                        ThrowError("invalid number of operands", instructionNode.Span());
                        break;
                    }
                }
                break;
            }
        }
    }
    switch (instructionNode.OpCode())
    {
        case cmajor::sbin::machine_x64::OpCode::CALL:
        {
            if (n != 1)
            {
                ThrowError("one operand expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::symbolNode)
            {
                EmitCallFunctionSymbol(symbol0);
            }
            else if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                EmitCallReg64(*this, reg0, instructionNode.Span());
            }
            else
            {
                ThrowError("CALL not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::JMP:
        {
            if (n != 1)
            {
                ThrowError("one operand expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::symbolNode)
            {
                EmitJmp(symbol0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                EmitJmpReg64(*this, reg0, instructionNode.Span());
            }
            else
            {
                ThrowError("JMP not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::JZ:
        case cmajor::sbin::machine_x64::OpCode::JE:
        {
            if (n != 1)
            {
                ThrowError("one operand expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::symbolNode)
            {
                EmitJe(symbol0, instructionNode.Span());
            }
            else
            {
                ThrowError("JE not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::JNZ:
        case cmajor::sbin::machine_x64::OpCode::JNE:
        {
            if (n != 1)
            {
                ThrowError("one operand expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::symbolNode)
            {
                EmitJne(symbol0, instructionNode.Span());
            }
            else
            {
                ThrowError("JNE not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::JAE:
        {
            if (n != 1)
            {
                ThrowError("one operand expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::symbolNode)
            {
                EmitJae(symbol0, instructionNode.Span());
            }
            else
            {
                ThrowError("JAE not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::NOP:
        {
            if (n != 0)
            {
                ThrowError("no operands expected", instructionNode.Span());
            }
            cmajor::sbin::machine_x64::EmitNop(*this, instructionNode.Span());
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::MOV:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::immediateNode)
            {
                cmajor::sbin::machine_x64::EmitMovRegImmediate(*this, reg0, immediate1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitMovReg64Reg64(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg)
            {
                cmajor::sbin::machine_x64::EmitMovReg32Reg32(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::binaryExprNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg)
            {
                cmajor::sbin::machine_x64::EmitMovReg32ContentDisp(*this, reg0, contentReg1, displacement1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::contentExprNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg && 
                cmajor::sbin::machine_x64::RegKind(contentReg1) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitMovReg32Content(*this, reg0, contentReg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitMovReg8Reg8(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::binaryExprNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg)
            {
                cmajor::sbin::machine_x64::EmitMovContentDispReg32(*this, contentReg1, displacement0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::contentExprNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg)
            {
                cmajor::sbin::machine_x64::EmitMovContentReg32(*this, contentReg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::contentExprNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitMovContentReg64(*this, contentReg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::binaryExprNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitMovContentDispReg64(*this, contentReg1, displacement0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::binaryExprNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitMovReg64ContentDisp(*this, reg0, contentReg1, displacement1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::binaryExprNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitMovContentDispReg8(*this, contentReg1, displacement0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::binaryExprNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
            {
                cmajor::sbin::machine_x64::EmitMovContentDispReg16(*this, contentReg1, displacement0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::contentExprNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
            {
                cmajor::sbin::machine_x64::EmitMovContentReg16(*this, contentReg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::contentExprNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitMovContentReg8(*this, contentReg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::contentExprNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitMovReg8Content(*this, reg0, contentReg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::binaryExprNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitMovReg8ContentDisp(*this, reg0, contentReg1, displacement1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::binaryExprNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
            {
                cmajor::sbin::machine_x64::EmitMovReg16ContentDisp(*this, reg0, contentReg1, displacement1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::contentExprNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitMovReg64Content(*this, reg0, contentReg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
            {
                cmajor::sbin::machine_x64::EmitMovReg16Reg16(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::contentExprNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg &&
                cmajor::sbin::machine_x64::RegKind(contentReg1) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitMovReg16Content(*this, reg0, contentReg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::symbolNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                EmitMovReg64Symbol(reg0, symbol1, instructionNode.Span());
            }
            else
            {
                ThrowError("MOV not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::MOVSX:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode && 
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitMovSxReg64Reg(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
            {
                cmajor::sbin::machine_x64::EmitMovSxReg64Reg16(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitMovSxReg16Reg8(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitMovSxReg32Reg8(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
            {
                cmajor::sbin::machine_x64::EmitMovSxReg32Reg16(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("MOVSX not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::MOVSXD:
        {
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg)
            {
                cmajor::sbin::machine_x64::EmitMovSxdReg64Reg32(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::sibNode && 
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg && 
                cmajor::sbin::machine_x64::RegKind(indexReg) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg &&
                cmajor::sbin::machine_x64::RegKind(baseReg) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitMovSxdReg64Sib(*this, reg0, scale, indexReg, baseReg, instructionNode.Span());
            }
            else
            {
                ThrowError("MOVSXD not implemented with these operands", instructionNode.Span());
            }
            break;

        }
        case cmajor::sbin::machine_x64::OpCode::MOVZX:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitMovZxReg16Reg8(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitMovZxReg32Reg8(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
            {
                cmajor::sbin::machine_x64::EmitMovZxReg32Reg16(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitMovZxReg64Reg8(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
            {
                cmajor::sbin::machine_x64::EmitMovZxReg64Reg16(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("MOVZX not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::MOVSD:
        {
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::symbolNode)
            {
                EmitMovSdSymbol(reg0, symbol1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::binaryExprNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitMovSdContentDispXmmReg(*this, contentReg1, displacement0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::contentExprNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitMovSdContentXmmReg(*this, contentReg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitMovSdXmmRegXmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::binaryExprNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitMovSdXmmRegContentDisp(*this, reg0, contentReg1, displacement1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::contentExprNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(contentReg1) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitMovSdXmmRegContent(*this, reg0, contentReg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::contentExprNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitMovSdContentXmmReg(*this, contentReg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("MOVSD not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::MOVSS:
        {
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::symbolNode)
            {
                EmitMovSsSymbol(reg0, symbol1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode && 
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitMovSsXmmRegXmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::binaryExprNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitMovSsContentDispXmmReg(*this, contentReg1, displacement0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::binaryExprNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitMovSsXmmRegContentDisp(*this, reg0, contentReg1, displacement1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::contentExprNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(contentReg1) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitMovSsXmmRegContent(*this, reg0, contentReg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::contentExprNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitMovSsContentXmmReg(*this, contentReg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("MOVSS not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::LEA:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::binaryExprNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg &&
                contentReg1 != cmajor::sbin::machine_x64::Register::none &&
                contentReg2 == cmajor::sbin::machine_x64::Register::none)
            {
                cmajor::sbin::machine_x64::EmitLeaReg64ContentDisp(*this, reg0, contentReg1, displacement1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::binaryExprNode && 
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg &&
                contentReg1 != cmajor::sbin::machine_x64::Register::none && 
                contentReg2 != cmajor::sbin::machine_x64::Register::none)
            {
                cmajor::sbin::machine_x64::EmitLeaReg64ContentRegs(*this, reg0, contentReg1, contentReg2, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::symbolNode)
            {
                EmitLeaSymbol(reg0, symbol1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::contentExprNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitLeaReg64ContentReg(*this, reg0, contentReg1, instructionNode.Span());
            }
            else
            {
                ThrowError("LEA not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::ADD:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode && 
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitAddReg64Reg64(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg)
            {
                cmajor::sbin::machine_x64::EmitAddReg32Reg32(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
            {
                cmajor::sbin::machine_x64::EmitAddReg16Reg16(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitAddReg8Reg8(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("ADD not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::ADDSS:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitAddSsXmmRegXmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("ADDSS not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::ADDSD:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitAddSdXmmRegXmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("ADDSD not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::SUB:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode)
            {
                switch (operand1Kind)
                {
                    case NodeKind::registerNode:
                    {
                        if (cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg &&
                            cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
                        {
                            cmajor::sbin::machine_x64::EmitSubReg64Reg64(*this, reg0, reg1, instructionNode.Span());
                        }
                        else if (cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg &&
                            cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg)
                        {
                            cmajor::sbin::machine_x64::EmitSubReg32Reg32(*this, reg0, reg1, instructionNode.Span());
                        }
                        else if (cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg &&
                            cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
                        {
                            cmajor::sbin::machine_x64::EmitSubReg16Reg16(*this, reg0, reg1, instructionNode.Span());
                        }
                        else if (cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg &&
                            cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
                        {
                            cmajor::sbin::machine_x64::EmitSubReg8Reg8(*this, reg0, reg1, instructionNode.Span());
                        }
                        else
                        {
                            ThrowError("SUB not implemented with these operands", instructionNode.Span());
                        }
                        break;
                    }
                    case NodeKind::immediateNode:
                    {
                        if (cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
                        {
                            cmajor::sbin::machine_x64::EmitSubReg64Immediate(*this, reg0, immediate1, instructionNode.Span());
                        }
                        else if (cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg)
                        {
                            cmajor::sbin::machine_x64::EmitSubReg32Immediate(*this, reg0, static_cast<int32_t>(immediate1), instructionNode.Span());
                        }
                        else if (cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
                        {
                            cmajor::sbin::machine_x64::EmitSubReg16Immediate(*this, reg0, static_cast<int16_t>(immediate1), instructionNode.Span());
                        }
                        else if (cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
                        {
                            cmajor::sbin::machine_x64::EmitSubReg8Immediate(*this, reg0, static_cast<int8_t>(immediate1), instructionNode.Span());
                        }
                        else
                        {
                            ThrowError("SUB not implemented with these operands", instructionNode.Span());
                        }
                        break;
                    }
                    default:
                    {
                        ThrowError("SUB not implemented with these operands", instructionNode.Span());
                        break;
                    }
                }
                break;
            }
            else
            {
                ThrowError("SUB not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::SUBSS:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitSubSsXmmRegXmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("SUBSS not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::SUBSD:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitSubSdXmmRegXmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("ADDSD not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::MUL:
        {
            if (n != 1)
            {
                ThrowError("one operand expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitMulReg64(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg)
            {
                cmajor::sbin::machine_x64::EmitMulReg32(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
            {
                cmajor::sbin::machine_x64::EmitMulReg16(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitMulReg8(*this, reg0, instructionNode.Span());
            }
            else
            {
                ThrowError("MUL not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::IMUL:
        {
            if (n != 1)
            {
                ThrowError("one operand expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitIMulReg64(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg)
            {
                cmajor::sbin::machine_x64::EmitIMulReg32(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
            {
                cmajor::sbin::machine_x64::EmitIMulReg16(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitIMulReg8(*this, reg0, instructionNode.Span());
            }
            else
            {
                ThrowError("IMUL not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::MULSS:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitMulSsXmmRegXmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("MULSS not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::MULSD:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitMulSdXmmRegXmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("MULSD not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::DIV:
        {
            if (n != 1)
            {
                ThrowError("one operand expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitDivReg64(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg)
            {
                cmajor::sbin::machine_x64::EmitDivReg32(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
            {
                cmajor::sbin::machine_x64::EmitDivReg16(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitDivReg8(*this, reg0, instructionNode.Span());
            }
            else
            {
                ThrowError("MUL not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::IDIV:
        {
            if (n != 1)
            {
                ThrowError("one operand expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitIDivReg8(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
            {
                cmajor::sbin::machine_x64::EmitIDivReg16(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg)
            {
                cmajor::sbin::machine_x64::EmitIDivReg32(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitIDivReg64(*this, reg0, instructionNode.Span());
            }
            else
            {
                ThrowError("IDIV not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::DIVSS:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitDivSsXmmRegXmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("DIVSS not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::DIVSD:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitDivSdXmmRegXmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("DIVSD not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::PUSH:
        case cmajor::sbin::machine_x64::OpCode::POP:
        {
            if (n != 1)
            {
                ThrowError("one operand expected", instructionNode.Span());
            }
            switch (operand0Kind)
            {
                case NodeKind::registerNode:
                {
                    switch (instructionNode.OpCode())
                    {
                        case cmajor::sbin::machine_x64::OpCode::PUSH:
                        {
                            cmajor::sbin::machine_x64::EmitPush(*this, reg0, instructionNode.Span());
                            break;
                        }
                        case cmajor::sbin::machine_x64::OpCode::POP:
                        {
                            cmajor::sbin::machine_x64::EmitPop(*this, reg0, instructionNode.Span());
                            break;
                        }
                    }
                    break;
                }
                default:
                {
                    ThrowError("register operand expected", instructionNode.Span());
                    break;
                }
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::RET:
        {
            if (n != 0 && n != 1)
            {
                ThrowError("zero or one operands expected", instructionNode.Span());
            }
            if (n == 0)
            {
                cmajor::sbin::machine_x64::EmitRet(*this, instructionNode.Span());
            }
            else if (n == 1)
            {
                if (immediate0 == 0)
                {
                    cmajor::sbin::machine_x64::EmitRet(*this, instructionNode.Span());
                }
                else if (operand0Kind == NodeKind::immediateNode)
                {
                    cmajor::sbin::machine_x64::EmitRet(*this, immediate0, instructionNode.Span());
                }
                else
                {
                    ThrowError("RET not implemented with these operands", instructionNode.Span());
                }
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::CMP:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg && 
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitCmpReg64Reg64(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg)
            {
                cmajor::sbin::machine_x64::EmitCmpReg32Reg32(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
            {
                cmajor::sbin::machine_x64::EmitCmpReg16Reg16(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitCmpReg8Reg8(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::immediateNode && 
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitCmpReg64Imm32(*this, reg0, static_cast<int32_t>(immediate1), instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::immediateNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg)
            {
                cmajor::sbin::machine_x64::EmitCmpReg32Imm32(*this, reg0, static_cast<int32_t>(immediate1), instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::immediateNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
            {
                cmajor::sbin::machine_x64::EmitCmpReg16Imm16(*this, reg0, static_cast<int16_t>(immediate1), instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::immediateNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitCmpReg8Imm8(*this, reg0, static_cast<int8_t>(immediate1), instructionNode.Span());
            }
            else
            {
                ThrowError("CMP not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::COMISD:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitComiSdXmmRegXmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("COMISD not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::UCOMISD:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitUComiSdXmmRegXmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("UCOMISD not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::COMISS:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitComiSsXmmRegXmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("COMISS not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::UCOMISS:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitUComiSsXmmRegXmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("UCOMISS not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::SETE:
        case cmajor::sbin::machine_x64::OpCode::SETC:
        case cmajor::sbin::machine_x64::OpCode::SETL:
        {
            if (n != 1)
            {
                ThrowError("one operand expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                switch (instructionNode.OpCode())
                {
                    case cmajor::sbin::machine_x64::OpCode::SETE:
                    {
                        cmajor::sbin::machine_x64::EmitSetEReg8(*this, reg0, instructionNode.Span());
                        break;
                    }
                    case cmajor::sbin::machine_x64::OpCode::SETC:
                    {
                        cmajor::sbin::machine_x64::EmitSetCReg8(*this, reg0, instructionNode.Span());
                        break;
                    }
                    case cmajor::sbin::machine_x64::OpCode::SETL:
                    {
                        cmajor::sbin::machine_x64::EmitSetLReg8(*this, reg0, instructionNode.Span());
                        break;
                    }
                }
            }
            else
            {
                ThrowError("SETx not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::SHL:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && 
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg && reg1 == cmajor::sbin::machine_x64::Register::cl)
            {
                cmajor::sbin::machine_x64::EmitShlReg8Cl(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg && reg1 == cmajor::sbin::machine_x64::Register::cl)
            {
                cmajor::sbin::machine_x64::EmitShlReg16Cl(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg && reg1 == cmajor::sbin::machine_x64::Register::cl)
            {
                cmajor::sbin::machine_x64::EmitShlReg32Cl(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg && reg1 == cmajor::sbin::machine_x64::Register::cl)
            {
                cmajor::sbin::machine_x64::EmitShlReg64Cl(*this, reg0, instructionNode.Span());
            }
            else
            {
                ThrowError("SHL not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::SHR:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg && reg1 == cmajor::sbin::machine_x64::Register::cl)
            {
                cmajor::sbin::machine_x64::EmitShrReg8Cl(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg && reg1 == cmajor::sbin::machine_x64::Register::cl)
            {
                cmajor::sbin::machine_x64::EmitShrReg16Cl(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg && reg1 == cmajor::sbin::machine_x64::Register::cl)
            {
                cmajor::sbin::machine_x64::EmitShrReg32Cl(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg && reg1 == cmajor::sbin::machine_x64::Register::cl)
            {
                cmajor::sbin::machine_x64::EmitShrReg64Cl(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::immediateNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
            {
                cmajor::sbin::machine_x64::EmitShrReg16Imm8(*this, reg0, static_cast<int8_t>(immediate1), instructionNode.Span());
            }
            else
            {
                ThrowError("SHR not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::SAR:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg && reg1 == cmajor::sbin::machine_x64::Register::cl)
            {
                cmajor::sbin::machine_x64::EmitSarReg8Cl(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg && reg1 == cmajor::sbin::machine_x64::Register::cl)
            {
                cmajor::sbin::machine_x64::EmitSarReg16Cl(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg && reg1 == cmajor::sbin::machine_x64::Register::cl)
            {
                cmajor::sbin::machine_x64::EmitSarReg32Cl(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg && reg1 == cmajor::sbin::machine_x64::Register::cl)
            {
                cmajor::sbin::machine_x64::EmitSarReg64Cl(*this, reg0, instructionNode.Span());
            }
            else
            {
                ThrowError("SAR not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::AND:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitAndReg8Reg8(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
            {
                cmajor::sbin::machine_x64::EmitAndReg16Reg16(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg)
            {
                cmajor::sbin::machine_x64::EmitAndReg32Reg32(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitAndReg64Reg64(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("AND not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::OR:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitOrReg8Reg8(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
            {
                cmajor::sbin::machine_x64::EmitOrReg16Reg16(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg)
            {
                cmajor::sbin::machine_x64::EmitOrReg32Reg32(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitOrReg64Reg64(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("OR not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::XOR:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitXorReg64Reg64(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg)
            {
                cmajor::sbin::machine_x64::EmitXorReg32Reg32(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
            {
                cmajor::sbin::machine_x64::EmitXorReg16Reg16(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitXorReg8Reg8(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::high_byte_reg && 
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::high_byte_reg)
            {
                cmajor::sbin::machine_x64::EmitXorHighByteRegHighByteReg(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::immediateNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitXorReg8Imm8(*this, reg0, static_cast<int8_t>(immediate1), instructionNode.Span());
            }
            else
            {
                ThrowError("XOR not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::NEG:
        {
            if (n != 1)
            {
                ThrowError("one operand expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitNegReg8(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
            {
                cmajor::sbin::machine_x64::EmitNegReg16(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg)
            {
                cmajor::sbin::machine_x64::EmitNegReg32(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitNegReg64(*this, reg0, instructionNode.Span());
            }
            else
            {
                ThrowError("NEG not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::NOT:
        {
            if (n != 1)
            {
                ThrowError("one operand expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitNotReg8(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::word_reg)
            {
                cmajor::sbin::machine_x64::EmitNotReg16(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg)
            {
                cmajor::sbin::machine_x64::EmitNotReg32(*this, reg0, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitNotReg64(*this, reg0, instructionNode.Span());
            }
            else
            {
                ThrowError("NOT not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::CBW:
        {
            if (n != 0)
            {
                ThrowError("no operands expected", instructionNode.Span());
            }
            cmajor::sbin::machine_x64::EmitCbw(*this, instructionNode.Span());
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::CWD:
        {
            if (n != 0)
            {
                ThrowError("no operands expected", instructionNode.Span());
            }
            cmajor::sbin::machine_x64::EmitCwd(*this, instructionNode.Span());
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::CDQ:
        {
            if (n != 0)
            {
                ThrowError("no operands expected", instructionNode.Span());
            }
            cmajor::sbin::machine_x64::EmitCdq(*this, instructionNode.Span());
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::CQO:
        {
            if (n != 0)
            {
                ThrowError("no operands expected", instructionNode.Span());
            }
            cmajor::sbin::machine_x64::EmitCqo(*this, instructionNode.Span());
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::CVTSI2SD:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitCvtSi2SdReg64XmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg)
            {
                cmajor::sbin::machine_x64::EmitCvtSi2SdReg32XmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("CVTSI2SD not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::CVTSI2SS:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitCvtSi2SsReg64XmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg)
            {
                cmajor::sbin::machine_x64::EmitCvtSi2SsReg32XmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("CVTSI2SS not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::CVTTSD2SI:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitCvtTSd2SiReg64XmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitCvtTSd2SiReg32XmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("CVTTSD2SI not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::CVTTSS2SI:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitCvtTSs2SiReg64XmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::doubleword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitCvtTSs2SiReg32XmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("CVTTSS2SI not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::CVTSS2SD:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitCvtSs2SdXmmRegXmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("CVTSS2SD not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::CVTSD2SS:
        {
            if (n != 2)
            {
                ThrowError("two operands expected", instructionNode.Span());
            }
            if (operand0Kind == NodeKind::registerNode && operand1Kind == NodeKind::registerNode &&
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::xmm_reg)
            {
                cmajor::sbin::machine_x64::EmitCvtSd2SsXmmRegXmmReg(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("CVTSD2SS not implemented with these operands", instructionNode.Span());
            }
            break;
        }
        default:
        {
            ThrowError("unimplemented opcode", instructionNode.Span());
            break;
        }
    }
}

void Assembler::Visit(FunctionDefinitionNode& functionDefinitionNode)
{
    SymbolNode* symbolNode = functionDefinitionNode.Symbol();
    if (symbolNode)
    {
        functionSymbol = GetSymbol(symbolNode->Name());
        if (!functionSymbol)
        {
            ThrowError("function symbol '" + symbolNode->Name() + "' not found", functionDefinitionNode.Span());
        }
        functionSymbol->SetValue(currentWriter->Position());
    }
    else
    {
        ThrowError("symbol node expected ", functionDefinitionNode.Span());
    }
    cmajor::sbin::coff::Section* prevSection = currentSection;
    std::unique_ptr<util::LittleEndianBinaryStreamWriter> prevWriter;
    util::MemoryStream linkOnceCodeStream;
    util::MemoryStream* prevCodeStream = currentCodeStream;
    std::vector<JmpPos> linkOnceJmpPositions;
    std::vector<SymbolDifference> linkonceSymbolDifferences;
    std::vector<JmpPos>* prevJmpPositions = currentJmpPositions;
    std::vector<SymbolDifference>* prevSymbolDifferences = currentSymbolDifferences;
    if (functionSymbol->IsKind(SymbolKind::linkOnce))
    {
        currentSection = functionSymbol->Section();
        prevWriter.reset(currentWriter.release());
        currentWriter.reset(new util::LittleEndianBinaryStreamWriter(linkOnceCodeStream));
        functionSymbol->SetValue(currentWriter->Position());
        currentCodeStream = &linkOnceCodeStream;
        currentJmpPositions = &linkOnceJmpPositions;
        currentSymbolDifferences = &linkonceSymbolDifferences;
    }
    for (const auto& inst : functionDefinitionNode.Instructions())
    {
        inst->Accept(*this);
    }
    if (functionSymbol->IsKind(SymbolKind::linkOnce))
    {
        linkOnceCodeStream.SetFromContent();
        ResolveJumps();
        ResolveSymbolDifferences();
        functionSymbol->Section()->SetData(linkOnceCodeStream.ReleaseContent());
        functionSymbol->Section()->ComputeCheckSum();
        functionSymbol->SectionDefinitionEntry()->SetAuxSectionDefinitionEntry(cmajor::sbin::coff::MakeAuxSectionDefinitionSymbolTableEntry(
            functionSymbol->Section()->Data().size(), functionSymbol->Section()->GetRelocationTable()->Relocations().size(), functionSymbol->Section()->CheckSum(),
            functionSymbol->Section()->GetSectionHeader()->Number(), cmajor::sbin::coff::IMAGE_COMDAT_SELECT_ANY));
        currentWriter.reset(prevWriter.release());
    }
    currentSection = prevSection;
    currentCodeStream = prevCodeStream;
    currentJmpPositions = prevJmpPositions;
    currentSymbolDifferences = prevSymbolDifferences;
}

void Assembler::Visit(MacroDefinitionNode& macroDefinitionNode)
{
    Symbol* symbol = GetSymbol(macroDefinitionNode.Symbol()->Name());
    if (symbol)
    {
        if (symbol->IsKind(SymbolKind::macro))
        {
            symbol->SetNode(macroDefinitionNode.Expr());
        }
        else
        {
            ThrowError("macro symbol '" + macroDefinitionNode.Symbol()->Name() + "' expected", macroDefinitionNode.Span());
        }
    }
    else
    {
        ThrowError("symbol '" + macroDefinitionNode.Symbol()->Name() + "' not found", macroDefinitionNode.Span());
    }
}

} // namespace cmajor::sbin::assembly
