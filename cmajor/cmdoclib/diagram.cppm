// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmdoclib.diagram;

import util;
import std.core;

export namespace cmdoclib {

struct DiagramNode
{
    DiagramNode(int index_, int level_, const std::string& name_, bool subject_, bool hasDerivedNodes_, const std::string& link_);
    void Write(util::CodeFormatter& formatter);
    int index;
    int level;
    std::string name;
    bool subject;
    bool hasDerivedNodes;
    std::string link;
};

struct DiagramNodeByLevelAndName
{
    bool operator()(const DiagramNode& left, const DiagramNode& right) const;
};

void GenerateDiagramCode(std::vector<DiagramNode>& diagramNodes, const std::string& drawFunctionName, const std::string& svgId, const std::string& scriptFilePath);

} // namespace cmdoclib
