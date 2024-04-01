// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

import util;

import cmajor.build;
import cmajor.symbols;
import cmajor.ast;
import soul.lexer;
import soul.xml.dom;
import soul.xml.dom.parser;
import soul.xml.xpath;
import std.core;
import std.filesystem;

void InitApplication()
{
    util::Init();
}

void DoneApplication()
{
    util::Done();
}

const char* version = "5.0.0";

struct UnitTest
{
    UnitTest();
    bool IsDefault() const { return index == -1; }
    int index;
    int lineOffset;
    std::string solution;
    std::string project;
    std::string name;
    std::string projectFilePath;
    std::string executableFilePath;
    std::string xmlFilePath;
};

UnitTest::UnitTest() : index(-1), lineOffset(0)
{
}

struct UnitTestResult
{
    UnitTestResult();
    int index;
    int lineOffset;
    std::string solution;
    std::string project;
    std::string name;
    std::string xmlFilePath;
    bool compileError;
    std::string compileErrorMessage;
    bool executeError;
    int executeExitCode;
    std::string executeErrorMessage;
};

UnitTestResult::UnitTestResult() : index(-1), lineOffset(0), compileError(false), executeError(false), executeExitCode(0)
{
}

void DoUnitTest(UnitTest& unitTest, util::SynchronizedQueue<UnitTestResult>& resultQueue)
{
    UnitTestResult result;
    try
    {
        std::string compileCommand = "masmcmc";
        compileCommand.append(" --unit-test-file-path=\"" + unitTest.xmlFilePath + "\"");
        compileCommand.append(" --unit-test-name=" + unitTest.name);
        compileCommand.append(" --unit-test");
        compileCommand.append(" --config=" + cmajor::symbols::GetConfig());
        compileCommand.append(" ").append(unitTest.projectFilePath);
        result.index = unitTest.index;
        result.lineOffset = unitTest.lineOffset;
        result.solution = unitTest.solution;
        result.project = unitTest.project;
        result.name = unitTest.name;
        result.xmlFilePath = unitTest.xmlFilePath;
        {
            util::Process compileProcess(compileCommand, util::Process::Redirections::processStdErr);
            compileProcess.WaitForExit();
            int compileExitCode = compileProcess.ExitCode();
            if (compileExitCode != 0)
            {
                result.compileError = true;
                result.compileErrorMessage = compileProcess.ReadToEnd(util::Process::StdHandle::stdErr);
            }
        }
        std::string executeCommand = unitTest.executableFilePath;
        std::filesystem::remove(unitTest.xmlFilePath);
        util::Process executeProcess(executeCommand, util::Process::Redirections::processStdErr);
        executeProcess.WaitForExit();
        int executeExitCode = executeProcess.ExitCode();
        if (executeExitCode != 0)
        {
            result.executeError = true;
            result.executeExitCode = executeExitCode;
            result.executeErrorMessage = executeProcess.ReadToEnd(util::Process::StdHandle::stdErr);
        }
        resultQueue.Put(result);
    }
    catch (const std::exception& ex)
    {
        result.executeError = true;
        result.executeErrorMessage = ex.what();
        resultQueue.Put(result);
    }
}

void DoUnitTests(util::SynchronizedQueue<UnitTest>& unitTestQueue, util::SynchronizedQueue<UnitTestResult>& resultQueue)
{
    while (!unitTestQueue.IsEmpty())
    {
        UnitTest unitTest = unitTestQueue.Get();
        if (unitTest.IsDefault()) return;
        DoUnitTest(unitTest, resultQueue);
    }
}

void DoUnitTestsMultithreaded(util::SynchronizedQueue<UnitTest>& unitTestQueue, util::SynchronizedQueue<UnitTestResult>& resultQueue)
{
    std::vector<std::thread> threads;
    int numThreads = std::thread::hardware_concurrency();
    for (int i = 0; i < numThreads; ++i)
    {
        std::thread thread{ DoUnitTests, std::ref(unitTestQueue), std::ref(resultQueue) };
        threads.push_back(std::move(thread));
    }
    for (auto& thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}

void MakeUnitTestFiles(cmajor::ast::Solution* solution, cmajor::ast::Project* project, 
    const std::string& projectFilePath, int32_t fileIndex, soul::lexer::FileMap& fileMap, cmajor::symbols::Module* module,
    util::SynchronizedQueue<UnitTest>& unitTestQueue, int& unitTestIndex)
{
    std::unique_ptr<cmajor::ast::CompileUnitNode> compileUnit = cmajor::build::ParseSourceFile(fileIndex, fileMap, cmajor::build::Flags::none, module);
    cmajor::ast::CloneContext makeUnitTestUnitContext;
    makeUnitTestUnitContext.SetMakeTestUnits();
    std::unique_ptr<cmajor::ast::CompileUnitNode> environmentNode(static_cast<cmajor::ast::CompileUnitNode*>(compileUnit->Clone(makeUnitTestUnitContext)));
    int firstLine = -1;
    int lineOffset = 0;
    for (cmajor::ast::FunctionNode* unitTestFunction : makeUnitTestUnitContext.UnitTestFunctions())
    {
        soul::ast::Span span = unitTestFunction->GetSpan();
        const std::vector<int>* lineStarts = fileMap.LineStartIndeces(fileIndex);
        if (lineStarts)
        {
            soul::ast::LineColLen lineColLen = soul::ast::SpanToLineColLen(span, *lineStarts);
            int line = lineColLen.line;
            if (firstLine == -1)
            {
                firstLine = line;
            }
            lineOffset = line - firstLine;
        }
        std::string unitTestName = util::ToUtf8(unitTestFunction->GroupId());
        cmajor::ast::CloneContext testUnitContext;
        std::unique_ptr<cmajor::ast::CompileUnitNode> testUnitCompileUnit(static_cast<cmajor::ast::CompileUnitNode*>(environmentNode->Clone(testUnitContext)));
        cmajor::ast::NamespaceNode* ns = testUnitCompileUnit->GlobalNs();
        ns->AddMember(unitTestFunction);
        std::string projectDir = util::Path::Combine(util::Path::GetDirectoryName(projectFilePath), unitTestName);
        std::string sourceFilePath = util::Path::Combine(projectDir, unitTestName + ".cm");
        std::filesystem::create_directories(util::Path::GetDirectoryName(sourceFilePath));
        std::ofstream sourceFile(sourceFilePath);
        util::CodeFormatter sourceFileFormatter(sourceFile);
        sourceFileFormatter.SetIndentSize(4);
        cmajor::ast::SourceWriter sourceWriter(sourceFileFormatter);
        testUnitCompileUnit->Accept(sourceWriter);
        std::ofstream mainFile(util::Path::Combine(projectDir, "main.cm"));
        util::CodeFormatter mainFileFormatter(mainFile);
        mainFileFormatter.SetIndentSize(4);
        mainFileFormatter.WriteLine("using System;");
        mainFileFormatter.WriteLine();
        mainFileFormatter.WriteLine("int main()");
        mainFileFormatter.WriteLine("{");
        mainFileFormatter.IncIndent();
        mainFileFormatter.WriteLine(unitTestName + "();");
        mainFileFormatter.WriteLine("return 0;");
        mainFileFormatter.DecIndent();
        mainFileFormatter.WriteLine("}");
        std::string unitTestProjectFilePath = util::Path::Combine(projectDir, unitTestName + ".cmp");
        std::ofstream projectFile(unitTestProjectFilePath);
        util::CodeFormatter projectFileFormatter(projectFile);
        projectFileFormatter.WriteLine("project " + unitTestName + ";");
        projectFileFormatter.WriteLine("target=program;");
        projectFileFormatter.WriteLine("source <" + unitTestName + ".cm>;");
        projectFileFormatter.WriteLine("source <main.cm>;");
        UnitTest unitTest;
        unitTest.index = unitTestIndex++;
        unitTest.lineOffset = lineOffset;
        if (solution)
        {
            unitTest.solution = util::ToUtf8(solution->Name());
        }
        if (project)
        {
            unitTest.project = util::ToUtf8(project->Name());
        }
        unitTest.name = unitTestName;
        unitTest.projectFilePath = unitTestProjectFilePath;
        std::string executableDir = util::Path::Combine(util::Path::Combine(util::Path::Combine(
            projectDir, "lib"), "masm"), cmajor::symbols::GetConfig());
        std::string executableFilePath = util::Path::Combine(executableDir, unitTestName + ".exe");
        unitTest.executableFilePath = executableFilePath;
        std::string unitTextXmlFilePath = util::Path::Combine(executableDir, unitTestName + ".xml");
        unitTest.xmlFilePath = unitTextXmlFilePath;
        unitTestQueue.Put(unitTest);
    }
    makeUnitTestUnitContext.UnitTestFunctions().clear();
}

void MakeUnitTestProject(cmajor::ast::Solution* solution, const std::string& projectFilePath, util::SynchronizedQueue<UnitTest>& unitTestQueue, int& unitTestIndex)
{
    soul::lexer::FileMap fileMap;
    std::unique_ptr<cmajor::ast::Project> project = cmajor::build::ParseProjectFile(projectFilePath, cmajor::symbols::GetConfig(), cmajor::ast::BackEnd::masm);
    if (project->GetTarget() != cmajor::ast::Target::unitTest)
    {
        throw std::runtime_error("project '" + projectFilePath + "' is not a unit test project; please add 'target=unitTest;' project declaration");
    }
    cmajor::symbols::Module module(project->Name(), project->ModuleFilePath(), project->GetTarget());
    int32_t fileIndex = 0;
    for (const auto& sourceFilePath : project->SourceFilePaths())
    {
        fileIndex = fileMap.MapFile(sourceFilePath);
        MakeUnitTestFiles(solution, project.get(), util::GetFullPath(project->FilePath()), fileIndex, fileMap, &module, unitTestQueue, unitTestIndex);
    }
}

void MakeUnitTestSolution(const std::string& solutionFilePath, util::SynchronizedQueue<UnitTest>& unitTestQueue, int& unitTestIndex)
{
    std::unique_ptr<cmajor::ast::Solution> solution = cmajor::build::ParseSolutionFile(solutionFilePath);
    for (const auto& projectFilePath : solution->ProjectFilePaths())
    {
        MakeUnitTestProject(solution.get(), projectFilePath, unitTestQueue, unitTestIndex);
    }
}

struct SortByIndex
{
    bool operator()(const UnitTestResult& left, const UnitTestResult& right) const
    {
        return left.index < right.index;
    }
};

std::string MakeResultXml(const std::string& resultDir, util::SynchronizedQueue<UnitTestResult>& resultQueue)
{
    std::string resultXmlFilePath(util::Path::Combine(resultDir, "unit-test-results." + cmajor::symbols::GetConfig() + ".xml"));
    std::ofstream resultXmlFile(resultXmlFilePath);
    util::CodeFormatter formatter(resultXmlFile);
    std::vector<UnitTestResult> results;
    while (!resultQueue.IsEmpty())
    {
        results.push_back(resultQueue.Get());
    }
    std::sort(results.begin(), results.end(), SortByIndex());
    soul::xml::Document resultDoc;
    soul::xml::Element* resultsElement = soul::xml::MakeElement("results");
    resultDoc.AppendChild(resultsElement);
    for (const auto& result : results)
    {
        soul::xml::Element* resultElement = soul::xml::MakeElement(result.name);
        bool executed = true;
        if (result.compileError)
        {
            resultElement->SetAttribute("compileError", result.compileErrorMessage);
            executed = false;
        }
        if (result.executeError)
        {
            resultElement->SetAttribute("executeError", result.executeErrorMessage);
            executed = false;
        }
        if (executed)
        {
            if (std::filesystem::exists(result.xmlFilePath))
            {
                std::unique_ptr<soul::xml::Document> executeDoc(soul::xml::ParseXmlFile(result.xmlFilePath));
                resultElement->AppendChild(executeDoc->RemoveChild(executeDoc->DocumentElement()).release());
                int lineOffset = result.lineOffset;
                auto nodeSet = soul::xml::xpath::EvaluateToNodeSet("test/assertion", resultElement);
                int count = nodeSet->Count();
                for (int i = 0; i < count; ++i)
                {
                    soul::xml::Node* node = nodeSet->GetNode(i);
                    if (node->IsElementNode())
                    {
                        soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
                        int line = std::stoi(element->GetAttribute("line"));
                        line += lineOffset;
                        element->SetAttribute("line", std::to_string(line));
                    }
                }
            }
        }
        resultElement->SetAttribute("solution", result.solution);
        resultElement->SetAttribute("project", result.project);
        resultsElement->AppendChild(resultElement);
    }
    resultDoc.Write(formatter);
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        std::cout << "==> " << resultXmlFilePath << "\n";
    }
    return resultXmlFilePath;
}

void MakeResultHtml(const std::string& resultXmlFilePath)
{
    std::unique_ptr<soul::xml::Document> resultDoc = soul::xml::ParseXmlFile(resultXmlFilePath);
    std::string resultHtmlFilePath = util::Path::ChangeExtension(resultXmlFilePath, ".html");
    std::ofstream resultHtmlFile(resultHtmlFilePath);
    util::CodeFormatter formatter(resultHtmlFile);
    soul::xml::Document htmlDoc;

    std::unique_ptr<soul::xml::Element> htmlElement(soul::xml::MakeElement("html"));
    std::unique_ptr<soul::xml::Element> headElement(soul::xml::MakeElement("head"));
    std::unique_ptr<soul::xml::Element> titleElement(soul::xml::MakeElement("title"));
    titleElement->AppendChild(soul::xml::MakeText("Unit Test Report"));
    std::unique_ptr<soul::xml::Element> styleElement(soul::xml::MakeElement("style"));
    std::string style =
        "body { max-width: 800px; } h1, h2, h3, h4, h5, h6 { color: #005ab4; font-family: sans-serif; } table { boder-collapse: collapse; } table, th, td { text-align: left; border: 1px solid #dddddd; padding: 8px; }";
    styleElement->AppendChild(soul::xml::MakeText(style));
    std::unique_ptr<soul::xml::Element> bodyElement(soul::xml::MakeElement("body"));
    std::unique_ptr<soul::xml::Element> h1Element(soul::xml::MakeElement("h1"));
    h1Element->AppendChild(soul::xml::MakeText("Unit Test Report"));
    bodyElement->AppendChild(h1Element.release());
    soul::xml::Element* testRootElement = resultDoc->DocumentElement();

    std::unique_ptr<soul::xml::Element> paramTableElement(soul::xml::MakeElement("table"));

    std::string configuration = cmajor::symbols::GetConfig();
    std::unique_ptr<soul::xml::Element> trConfigElement(soul::xml::MakeElement("tr"));
    std::unique_ptr<soul::xml::Element> thConfigElement(soul::xml::MakeElement("th"));
    thConfigElement->AppendChild(soul::xml::MakeText("configuration"));
    trConfigElement->AppendChild(thConfigElement.release());
    std::unique_ptr<soul::xml::Element> tdConfigElement(soul::xml::MakeElement("td"));
    tdConfigElement->AppendChild(soul::xml::MakeText(configuration));
    trConfigElement->AppendChild(tdConfigElement.release());
    paramTableElement->AppendChild(trConfigElement.release());

    bodyElement->AppendChild(paramTableElement.release());

    std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
    h2Element->AppendChild(soul::xml::MakeText("Results"));
    bodyElement->AppendChild(h2Element.release());

    std::unique_ptr<soul::xml::Element> tableElement(soul::xml::MakeElement("table"));

    std::unique_ptr<soul::xml::Element> trHeaderElement(soul::xml::MakeElement("tr"));
    std::unique_ptr<soul::xml::Element> thNameElement(soul::xml::MakeElement("th"));
    thNameElement->AppendChild(soul::xml::MakeText("name"));
    trHeaderElement->AppendChild(thNameElement.release());

    std::unique_ptr<soul::xml::Element> thCountElement(soul::xml::MakeElement("th"));
    thCountElement->AppendChild(soul::xml::MakeText("count"));
    trHeaderElement->AppendChild(thCountElement.release());

    std::unique_ptr<soul::xml::Element> thPassedElement(soul::xml::MakeElement("th"));
    thPassedElement->AppendChild(soul::xml::MakeText("passed"));
    trHeaderElement->AppendChild(thPassedElement.release());

    std::unique_ptr<soul::xml::Element> thFailedElement(soul::xml::MakeElement("th"));
    thFailedElement->AppendChild(soul::xml::MakeText("failed"));
    trHeaderElement->AppendChild(thFailedElement.release());

    std::unique_ptr<soul::xml::Element> thEmptyElement(soul::xml::MakeElement("th"));
    thEmptyElement->AppendChild(soul::xml::MakeText("empty"));
    trHeaderElement->AppendChild(thEmptyElement.release());

    std::unique_ptr<soul::xml::Element> thExitCodeElement(soul::xml::MakeElement("th"));
    thExitCodeElement->AppendChild(soul::xml::MakeText("exit code"));
    trHeaderElement->AppendChild(thExitCodeElement.release());

    std::unique_ptr<soul::xml::Element> thCompileErrorElement(soul::xml::MakeElement("th"));
    thCompileErrorElement->AppendChild(soul::xml::MakeText("compile error"));
    trHeaderElement->AppendChild(thCompileErrorElement.release());

    std::unique_ptr<soul::xml::Element> thExecutionErrorElement(soul::xml::MakeElement("th"));
    thExecutionErrorElement->AppendChild(soul::xml::MakeText("execution error"));
    trHeaderElement->AppendChild(thExecutionErrorElement.release());

    std::vector<std::pair<std::string, soul::xml::Element*>> failedAssertions;

    tableElement->AppendChild(trHeaderElement.release());

    std::unique_ptr<soul::xml::xpath::NodeSet> tests = soul::xml::xpath::EvaluateToNodeSet("//test", resultDoc.get());
    int n = tests->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* testNode = tests->GetNode(i);
        if (testNode->IsElementNode())
        {
            soul::xml::Element* testElement = static_cast<soul::xml::Element*>(testNode);
            std::string name = testElement->GetAttribute("name");
            std::string fullTestName = name;
            soul::xml::Element* resultElement = static_cast<soul::xml::Element*>(testElement->Parent());
            std::string solution = resultElement->GetAttribute("solution");
            std::string project = resultElement->GetAttribute("project");
            if (!project.empty())
            {
                fullTestName = project + "." + fullTestName;
            }
            if (!solution.empty())
            {
                fullTestName = solution + "." + fullTestName;
            }
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            std::unique_ptr<soul::xml::Element> tdNameElement(soul::xml::MakeElement("td"));
            soul::xml::Element* nameElement = tdNameElement.get();
            tdNameElement->AppendChild(soul::xml::MakeText(fullTestName));
            trElement->AppendChild(tdNameElement.release());
            std::unique_ptr<soul::xml::Element> tdCountElement(soul::xml::MakeElement("td"));
            tdCountElement->AppendChild(soul::xml::MakeText(testElement->GetAttribute("count")));
            trElement->AppendChild(tdCountElement.release());
            std::unique_ptr<soul::xml::xpath::NodeSet> assertions = soul::xml::xpath::EvaluateToNodeSet("assertion", testElement);
            int passed = 0;
            int failed = 0;
            int empty = 0;
            int count = 0;
            count = assertions->Count();
            for (int i = 0; i < count; ++i)
            {
                soul::xml::Node* assertionNode = assertions->GetNode(i);
                if (assertionNode->IsElementNode())
                {
                    soul::xml::Element* assertionElement = static_cast<soul::xml::Element*>(assertionNode);
                    std::string result = assertionElement->GetAttribute("result");
                    if (result == "passed")
                    {
                        ++passed;
                    }
                    else if (result == "failed")
                    {
                        ++failed;
                        failedAssertions.push_back(std::make_pair(fullTestName, assertionElement));
                    }
                    else
                    {
                        ++empty;
                    }
                }
            }
            std::string fontAttr = "font-family: sans-serif; font-size: 13px; font-weight: bold;";
            if (count > 0)
            {
                if (passed == count)
                {
                    nameElement->SetAttribute("style", "background-color: #00e600; " + fontAttr);
                }
                else if (failed > 0)
                {
                    nameElement->SetAttribute("style", "background-color: #ff3300; " + fontAttr);
                }
                else
                {
                    nameElement->SetAttribute("style", "background-color: #ffff00; " + fontAttr);
                }
            }
            else
            {
                nameElement->SetAttribute("style", "background-color: #ffff00; " + fontAttr);
            }
            std::unique_ptr<soul::xml::Element> tdPassedElement(soul::xml::MakeElement("td"));
            tdPassedElement->AppendChild(soul::xml::MakeText(std::to_string(passed)));
            trElement->AppendChild(tdPassedElement.release());

            std::unique_ptr<soul::xml::Element> tdFailedElement(soul::xml::MakeElement("td"));
            tdFailedElement->AppendChild(soul::xml::MakeText(std::to_string(failed)));
            trElement->AppendChild(tdFailedElement.release());

            if (count == 0)
            {
                std::unique_ptr<soul::xml::Element> tdEmptyElement(soul::xml::MakeElement("td"));
                tdEmptyElement->AppendChild(soul::xml::MakeText(testElement->GetAttribute("count")));
                trElement->AppendChild(tdEmptyElement.release());
            }
            else
            {
                std::unique_ptr<soul::xml::Element> tdEmptyElement(soul::xml::MakeElement("td"));
                tdEmptyElement->AppendChild(soul::xml::MakeText(std::to_string(empty)));
                trElement->AppendChild(tdEmptyElement.release());
            }

            std::unique_ptr<soul::xml::Element> tdExitCodeElement(soul::xml::MakeElement("td"));
            tdExitCodeElement->AppendChild(soul::xml::MakeText(testElement->GetAttribute("exitCode")));
            trElement->AppendChild(tdExitCodeElement.release());
            std::unique_ptr<soul::xml::Element> tdCompileErrorElement(soul::xml::MakeElement("td"));
            tdCompileErrorElement->AppendChild(soul::xml::MakeText(testElement->GetAttribute("compileError")));
            trElement->AppendChild(tdCompileErrorElement.release());
            std::unique_ptr<soul::xml::Element> tdExceptionElement(soul::xml::MakeElement("td"));
            tdExceptionElement->AppendChild(soul::xml::MakeText(testElement->GetAttribute("executeError")));
            trElement->AppendChild(tdExceptionElement.release());
            tableElement->AppendChild(trElement.release());
        }
    }
    bodyElement->AppendChild(tableElement.release());

    if (!failedAssertions.empty())
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->AppendChild(soul::xml::MakeText("Failed Assertions"));
        bodyElement->AppendChild(h2Element.release());

        std::unique_ptr<soul::xml::Element> failedAssertionsTable(soul::xml::MakeElement("table"));

        std::unique_ptr<soul::xml::Element> trHeaderElement(soul::xml::MakeElement("tr"));

        std::unique_ptr<soul::xml::Element> thNameElement(soul::xml::MakeElement("th"));
        thNameElement->AppendChild(soul::xml::MakeText("name"));
        trHeaderElement->AppendChild(thNameElement.release());

        std::unique_ptr<soul::xml::Element> thIndexElement(soul::xml::MakeElement("th"));
        thIndexElement->AppendChild(soul::xml::MakeText("index"));
        trHeaderElement->AppendChild(thIndexElement.release());

        std::unique_ptr<soul::xml::Element> thLineElement(soul::xml::MakeElement("th"));
        thLineElement->AppendChild(soul::xml::MakeText("line"));
        trHeaderElement->AppendChild(thLineElement.release());

        failedAssertionsTable->AppendChild(trHeaderElement.release());

        int n = failedAssertions.size();
        for (int i = 0; i < n; ++i)
        {
            const std::pair<std::string, soul::xml::Element*>& p = failedAssertions[i];
            std::unique_ptr<soul::xml::Element> trAssertionElement(soul::xml::MakeElement("tr"));
            soul::xml::Element* assertionElement = p.second;

            std::unique_ptr<soul::xml::Element> tdNameElement(soul::xml::MakeElement("td"));
            std::string attr = "background-color: #ff3300; font-family: sans-serif; font-size: 13px; font-weight: bold;";
            tdNameElement->SetAttribute("style", attr);
            tdNameElement->AppendChild(soul::xml::MakeText(p.first));
            trAssertionElement->AppendChild(tdNameElement.release());

            std::unique_ptr<soul::xml::Element> tdIndexElement(soul::xml::MakeElement("td"));
            tdIndexElement->AppendChild(soul::xml::MakeText(assertionElement->GetAttribute("index")));
            trAssertionElement->AppendChild(tdIndexElement.release());

            std::unique_ptr<soul::xml::Element> tdLineElement(soul::xml::MakeElement("td"));
            tdLineElement->AppendChild(soul::xml::MakeText(assertionElement->GetAttribute("line")));
            trAssertionElement->AppendChild(tdLineElement.release());

            failedAssertionsTable->AppendChild(trAssertionElement.release());
        }
        bodyElement->AppendChild(failedAssertionsTable.release());
    }

    headElement->AppendChild(titleElement.release());
    headElement->AppendChild(styleElement.release());
    htmlElement->AppendChild(headElement.release());
    htmlElement->AppendChild(bodyElement.release());
    htmlDoc.AppendChild(htmlElement.release());
    htmlDoc.Write(formatter);
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        std::cout << "==> " << resultHtmlFilePath << "\n";
    }
}

void PrintHelp()
{
    std::cout << "Cmajor unit test engine version " << version << " with MASM backend for Windows x64" << std::endl;
    std::cout << "Usage: mcmunit [options] { project.cmp | solution.cms }" << std::endl;
    std::cout << "Compiles given Cmajor unit test solutions and projects to unit test executables and runs them." << std::endl;
    std::cout << "Generates unit test report 'unit-test-report.html' to the project or solution directory." << std::endl;
    std::cout << "Options:\n" <<
        "--help (-h)\n" <<
        "   print this help message\n" <<
        "--config=CONFIG (-c=CONFIG)\n" <<
        "   set configuration to CONFIG (debug | release)\n" <<
        "   default is debug\n" <<
        "--optimization-level=LEVEL (-O=LEVEL)\n" <<
        "   set optimization level to LEVEL=0-3\n" <<
        "   defaults: debug=0, release=2\n" <<
        "--verbose (-v)\n" <<
        "   print verbose messages\n" <<
        "--quiet (-q)\n" <<
        "   print no messages\n" <<
        "--define SYMBOL (-D SYMBOL)\n" <<
        "   define a conditional compilation symbol SYMBOL.\n" <<
        "--link-with-debug-runtime (-d)\n" <<
        "   link with debug runtime 'cmrtmasmd.lib'\n" <<
        "--single-threaded (-s)\n" <<
        "   compile using a single thread\n" <<
        "--time (-t)\n" <<
        "   print duration of compilation\n" <<
        std::endl;
}

int main(int argc, const char** argv)
{
    try
    {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        InitApplication();
        bool prevWasDefine = false;
        std::vector<std::string> files;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.starts_with("--"))
            {
                if (arg == "--help")
                {
                    PrintHelp();
                    return 1;
                }
                else if (arg == "--verbose")
                {
                    cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::verbose);
                }
                else if (arg == "--quiet")
                {
                    cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::quiet);
                }
                else if (arg == "--define")
                {
                    prevWasDefine = true;
                }
                else if (arg == "--single-threaded")
                {
                    cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::singleThreadedCompile);
                }
                else if (arg == "--link-with-debug-runtime")
                {
                    cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::linkWithDebugRuntime);
                }
                else if (arg == "--time")
                {
                    cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::time);
                }
                else if (arg.find('=') != std::string::npos)
                {
                    std::vector<std::string> components = util::Split(arg, '=');
                    if (components.size() == 2)
                    {
                        if (components[0] == "--config")
                        {
                            if (components[1] == "release")
                            {
                                cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::release);
                            }
                            else if (components[1] != "debug")
                            {
                                throw std::runtime_error("unknown configuration '" + components[1] + "'");
                            }
                        }
                        else if (components[0] == "--optimization-level")
                        {
                            int optimizationLevel = std::stoi(components[1]);
                            if (optimizationLevel >= 0 && optimizationLevel <= 3)
                            {
                                cmajor::symbols::SetOptimizationLevel(optimizationLevel);
                            }
                            else
                            {
                                throw std::runtime_error("unknown optimization level '" + components[1] + "'");
                            }
                        }
                    }
                    else
                    {
                        throw std::runtime_error("unknown option '" + arg + "'");
                    }
                }
                else
                {
                    throw std::runtime_error("unknown option '" + arg + "'");
                }
            }
            else if (arg.starts_with("-"))
            {
                if (arg.find('=') != std::string::npos)
                {
                    std::vector<std::string> components = util::Split(arg, '=');
                    if (components[0] == "-c")
                    {
                        if (components[1] == "release")
                        {
                            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::release);
                        }
                        else if (components[1] != "debug")
                        {
                            throw std::runtime_error("unknown configuration '" + components[1] + "'");
                        }
                    }
                    else if (components[0] == "-O")
                    {
                        int optimizationLevel = std::stoi(components[1]);
                        if (optimizationLevel >= 0 && optimizationLevel <= 3)
                        {
                            cmajor::symbols::SetOptimizationLevel(optimizationLevel);
                        }
                        else
                        {
                            throw std::runtime_error("unknown optimization level '" + components[1] + "'");
                        }
                    }
                    else if (components[0] == "-p")
                    {
                        cmajor::symbols::SetPass(components[1]);
                    }
                    else
                    {
                        throw std::runtime_error("unknown option '" + arg + "'");
                    }
                }
                else
                {
                    std::string options = arg.substr(1);
                    for (char o : options)
                    {
                        switch (o)
                        {
                        case 'h':
                        {
                            PrintHelp();
                            return 1;
                        }
                        case 'v':
                        {
                            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::verbose);
                            break;
                        }
                        case 'q':
                        {
                            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::quiet);
                            break;
                        }
                        case 'D':
                        {
                            prevWasDefine = true;
                            break;
                        }
                        case 's':
                        {
                            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::singleThreadedCompile);
                            break;
                        }
                        case 'd':
                        {
                            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::linkWithDebugRuntime);
                            break;
                        }
                        case 't':
                        {
                            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::time);
                            break;
                        }
                        default:
                        {
                            throw std::runtime_error("unknown option -" + std::string(1, o) + "'");
                        }
                        }
                    }
                }
            }
            else if (prevWasDefine)
            {
                prevWasDefine = false;
                cmajor::symbols::DefineCommandLineConditionalSymbol(util::ToUtf32(arg));
            }
            else
            {
                files.push_back(arg);
            }
        }
        if (files.size() != 1)
        {
            throw std::runtime_error("single project or solution file expected");
        }
        int unitTestIndex = 0;
        std::string resultDir;
        util::SynchronizedQueue<UnitTest> unitTestQueue;
        util::SynchronizedQueue<UnitTestResult> resultQueue;
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
        {
            std::cout << "making tests for " + cmajor::symbols::GetConfig() + " mode..." << "\n";
        }
        for (const auto& file : files)
        {
            if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
            {
                std::cout << "> " << file << "\n";
            }
            if (file.ends_with(".cms"))
            {
                MakeUnitTestSolution(file, unitTestQueue, unitTestIndex);
                resultDir = util::Path::GetDirectoryName(util::GetFullPath(file));
            }
            else if (file.ends_with(".cmp"))
            {
                MakeUnitTestProject(nullptr, file, unitTestQueue, unitTestIndex);
                resultDir = util::Path::GetDirectoryName(util::GetFullPath(file));
            }
        }
        std::cout << "compiling and running tests for " + cmajor::symbols::GetConfig() + " mode..." << "\n";
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::singleThreadedCompile))
        {
            DoUnitTests(unitTestQueue, resultQueue);
        }
        else
        {
            DoUnitTestsMultithreaded(unitTestQueue, resultQueue);
        }
        std::cout << "making results for " + cmajor::symbols::GetConfig() + " mode..." << "\n";
        std::string resultXmlFilePath = MakeResultXml(resultDir, resultQueue);
        MakeResultHtml(resultXmlFilePath);
        std::cout << "done." << "\n";
        if (GetGlobalFlag(cmajor::symbols::GlobalFlags::time))
        {
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            auto dur = end - start;
            long long totalSecs = std::chrono::duration_cast<std::chrono::seconds>(dur).count() + 1;
            int hours = static_cast<int>(totalSecs / 3600);
            int mins = static_cast<int>((totalSecs / 60) % 60);
            int secs = static_cast<int>(totalSecs % 60);
            std::cout <<
                (hours > 0 ? std::to_string(hours) + " hour" + ((hours != 1) ? "s " : " ") : "") <<
                (mins > 0 ? std::to_string(mins) + " minute" + ((mins != 1) ? "s " : " ") : "") <<
                secs << " second" << ((secs != 1) ? "s" : "") << std::endl;
        }
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        if (!cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::quiet))
        {
            std::cerr << ex.What() << std::endl;
        }
        return 1;
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
        return 1;
    }
    DoneApplication();
    return 0;
}
