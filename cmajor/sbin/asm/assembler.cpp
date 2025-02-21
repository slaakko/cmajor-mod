// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.sbin.assembly.assembler;

import cmajor.sbin.assembly.asm_file_parser;

namespace cmajor::sbin::assembly {

Assembler::Assembler(const std::string& asmFilePath_, bool verbose_) : 
    verbose(verbose), fileMap(), asmFilePath(asmFilePath_), objFilePath(util::Path::ChangeExtension(asmFilePath, ".obj")), 
    objectFile(new cmajor::sbin::coff::CoffObjectFile()), codeSection(nullptr), dataSection(nullptr), 
    emitOperation(EmitOperation::emitCode), dataInstKind(DataInstKind::db), operandKind(NodeKind::none), 
    reg(cmajor::sbin::machine_x64::Register::none), contentReg0(cmajor::sbin::machine_x64::Register::none), 
    contentReg1(cmajor::sbin::machine_x64::Register::none), contentReg2(cmajor::sbin::machine_x64::Register::none),
    immediate(0), displacement(0), content(false), symbolOperand(nullptr), functionSymbol(nullptr)
{
}

void Assembler::Assemble()
{
    asmFile = ParseAsmFile(asmFilePath, verbose, fileMap);
    codeSection = cmajor::sbin::coff::MakeCodeSection(objectFile.get());
    objectFile->AddSection(codeSection);
    dataSection = cmajor::sbin::coff::MakeDataSection(objectFile.get());
    objectFile->AddSection(dataSection);
    MakeSymbols();
    EmitData();
    EmitCode();
    ResolveJumps();
    dataSection->SetData(dataStream.ReleaseContent());
    codeSection->SetData(codeStream.ReleaseContent());
    WriteObjectFile();
}

void Assembler::WriteObjectFile()
{
    objectFile->Write(objFilePath);
}

void Assembler::EmitData()
{
    emitOperation = EmitOperation::emitData;
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
    currentWriter.reset(new util::LittleEndianBinaryStreamWriter(codeStream));
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

void Assembler::EmitCallFunctionSymbol(Symbol* symbol)
{
    cmajor::sbin::machine_x64::EmitCallNear(*this);
    int64_t pos = currentWriter->Position();
    codeSection->GetRelocationTable()->AddRelocation(cmajor::sbin::coff::Relocation(static_cast<uint32_t>(pos), symbol->Entry()->Index(), 
        cmajor::sbin::coff::IMAGE_REL_AMD64_REL32));
    EmitDword(static_cast<uint32_t>(0u));
}

void Assembler::EmitLeaSymbol(cmajor::sbin::machine_x64::Register reg, Symbol* symbol, const soul::ast::Span& span)
{
    cmajor::sbin::machine_x64::EmitLeaNear(*this, reg, span);
    int64_t pos = currentWriter->Position();
    codeSection->GetRelocationTable()->AddRelocation(cmajor::sbin::coff::Relocation(static_cast<uint32_t>(pos), symbol->Entry()->Index(),
        cmajor::sbin::coff::IMAGE_REL_AMD64_REL32));
    EmitDword(static_cast<uint32_t>(0u));
}

void Assembler::EmitJmp(Symbol* symbol, const soul::ast::Span& span)
{
    cmajor::sbin::machine_x64::EmitJmpNear(*this);
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

void Assembler::AddJmpPos(int64_t pos, Symbol* symbol, const soul::ast::Span& span)
{
    jmpPositions.push_back(JmpPos(pos, symbol, span));
}

void Assembler::ResolveJumps()
{
    for (const auto& jmpPos : jmpPositions)
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
            util::LittleEndianMemoryWriter jmpOffsetWriter(codeStream.Data() + pos, 4);
            jmpOffsetWriter.Write(static_cast<int32_t>(offset));
        }
        else
        {
            ThrowError("value of symbol '" + symbol->Name() + "' not defined", jmpPos.span);
        }
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
        else if (symbol->IsKind(SymbolKind::external) && symbol->IsKind(SymbolKind::code))
        {
            cmajor::sbin::coff::SymbolTableEntry* entry = cmajor::sbin::coff::MakeExternalFunctionSymbolTableEntry(symbol->Name(), objectFile.get());
            objectFile->GetSymbolTable()->AddEntry(entry);
            symbol->SetEntry(entry);
        }
        else if (symbol->IsKind(SymbolKind::public_) && symbol->IsKind(SymbolKind::data))
        {
            cmajor::sbin::coff::SymbolTableEntry* entry = cmajor::sbin::coff::MakeInternalDataSymbolTableEntry(
                dataSection->GetSectionHeader()->Number(),
                symbol->Name(),
                objectFile.get());
            objectFile->GetSymbolTable()->AddEntry(entry);
            symbol->SetEntry(entry);
        }
        else if (symbol->IsKind(SymbolKind::external) && symbol->IsKind(SymbolKind::data))
        {
            cmajor::sbin::coff::SymbolTableEntry* entry = cmajor::sbin::coff::MakeExternalDataSymbolTableEntry(symbol->Name(), objectFile.get());
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
    }
    EmitOperation prevOperation = emitOperation;
    emitOperation = EmitOperation::emitData;
    dataInstKind = dataDefinitionNode.GetDataInstKind();
    for (const auto& operand : dataDefinitionNode.Operands())
    {
        operand->Accept(*this);
    }
    emitOperation = prevOperation;
}

void Assembler::Visit(BinaryExprNode& binaryExprNode)
{
    if (!content)
    {
        ThrowError("content expression expected", binaryExprNode.Span());
    }
    reg = sbin::machine_x64::Register::none;
    binaryExprNode.Left()->Accept(*this);
    if (operandKind == NodeKind::registerNode)
    {
        contentReg1 = reg;
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
    else if (operandKind == NodeKind::immediateNode)
    {
        displacement = static_cast<uint32_t>(immediate);
        immediate = 0;
    }
    reg = sbin::machine_x64::Register::none;
    operandKind = NodeKind::binaryExprNode;
}

void Assembler::Visit(UnaryExprNode& unaryExprNode)
{
    // todo
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
                EmitByte(static_cast<uint8_t>(value));
                break;
            }
            case DataInstKind::dw:
            {
                EmitWord(static_cast<uint16_t>(value));
                break;
            }
            case DataInstKind::dd:
            {
                EmitDword(static_cast<uint32_t>(value));
                break;
            }
            case DataInstKind::dq:
            {
                EmitQword(static_cast<uint64_t>(value));
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
        immediate = value;
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
    // todo
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
    std::cout << "SPAN pos=" + std::to_string(span.pos) << "\n";
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
    const std::vector<int>* lineStartIndeces = fileMap.LineStartIndeces(asmFile->FileIndex());
    if (lineStartIndeces)
    {
        soul::ast::LineColLen lineColLen = soul::ast::SpanToLineColLen(span, *lineStartIndeces);
        std::cout << "line " << lineColLen.line << "\n";
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
    uint64_t immediate0 = 0u;
    uint64_t immediate1 = 0u;
    uint32_t displacement0 = 0u;
    uint32_t displacement1 = 0u;
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
        displacement = 0u;
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
            else
            {
                ThrowError("JMP not implemented with these operands", instructionNode.Span());
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
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg)
            {
                cmajor::sbin::machine_x64::EmitMovSxReg64Reg(*this, reg0, reg1, instructionNode.Span());
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
            else
            {
                ThrowError("MOVSX not implemented with these operands", instructionNode.Span());
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
                cmajor::sbin::machine_x64::RegKind(reg0) == cmajor::sbin::machine_x64::RegisterKind::quadword_reg &&
                cmajor::sbin::machine_x64::RegKind(reg1) == cmajor::sbin::machine_x64::RegisterKind::byte_reg)
            {
                cmajor::sbin::machine_x64::EmitMovZxReg64Reg8(*this, reg0, reg1, instructionNode.Span());
            }
            else
            {
                ThrowError("MOVZX not implemented with these operands", instructionNode.Span());
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
                contentReg2 == cmajor::sbin::machine_x64::Register::none && displacement1 != 0u)
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
            else
            {
                ThrowError("CMP not implemented with these operands", instructionNode.Span());
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
                cmajor::sbin::machine_x64::EmitShrReg16Imm8(*this, reg0, static_cast<uint8_t>(immediate1), instructionNode.Span());
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
                cmajor::sbin::machine_x64::EmitXorReg8Imm8(*this, reg0, static_cast<uint8_t>(immediate1), instructionNode.Span());
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
        functionSymbol = nullptr;
    }
    for (const auto& inst : functionDefinitionNode.Instructions())
    {
        inst->Accept(*this);
    }
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
