// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing.metrics;

import soul.xml.dom;
import soul.xml.dom.parser;
import soul.xml.xpath;
import util;
import std.filesystem;

namespace wing {

std::string GetDefaultMetricsFilePath()
{
    std::string metricsFilePath;
    const char* cmajorRoot = getenv("CMAJOR_ROOT");
    if (cmajorRoot && *cmajorRoot)
    {
        metricsFilePath = util::Path::Combine(util::Path::Combine(cmajorRoot, "config"), "metrics.xml");
    }
    else
    {
        throw std::runtime_error("error: 'CMAJOR_ROOT' environment variable not set, please set it to contain /path/to/cmajor directory");
    }
    return metricsFilePath;
}

SizeElement::SizeElement(const std::string& name_) : name(name_)
{
}

void SizeElement::Read(soul::xml::Element* parentElement)
{
    std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet(name, parentElement);
    int n = nodeSet->Count();
    if (n == 1)
    {
        soul::xml::Node* node = nodeSet->GetNode(0);
        if (node->IsElementNode())
        {
            soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
            size.Width = std::stof(element->GetAttribute("width"));
            size.Height = std::stof(element->GetAttribute("height"));
        }
    }
}

void SizeElement::Write(soul::xml::Element* parentElement)
{
    soul::xml::Element* element = soul::xml::MakeElement(name);
    element->SetAttribute("width", std::to_string(size.Width));
    element->SetAttribute("height", std::to_string(size.Height));
    parentElement->AppendChild(element);
}

Metrics::Metrics() :
    dpiX(96), dpiY(96),
    defaultButtonSize(88, 25), defaultButtonSizeElement("button"),
    defaultLabelSize(71, 19), defaultLabelSizeElement("label"),
    defaultTextBoxSize(94, 14), defaultTextBoxSizeElement("textBox"),
    defaultListBoxSize(120, 96), defaultListBoxSizeElement("listBox"),
    defaultComboBoxSize(121, 21), defaultComboBoxSizeElement("comboBox"),
    defaultCheckBoxSize(80, 17), defaultCheckBoxSizeElement("checkBox"),
    defaultControlSpacing(10, 10), defaultControlSpacingElement("controlSpacing")
{
    SetElementValues();
}

void Metrics::SetElementValues()
{
    defaultButtonSizeElement.Set(SizeF(HorizontalPixelsToMM(defaultButtonSize.Width), VerticalPixelsToMM(defaultButtonSize.Height)));
    defaultLabelSizeElement.Set(SizeF(HorizontalPixelsToMM(defaultLabelSize.Width), VerticalPixelsToMM(defaultLabelSize.Height)));
    defaultTextBoxSizeElement.Set(SizeF(HorizontalPixelsToMM(defaultTextBoxSize.Width), VerticalPixelsToMM(defaultTextBoxSize.Height)));
    defaultListBoxSizeElement.Set(SizeF(HorizontalPixelsToMM(defaultListBoxSize.Width), VerticalPixelsToMM(defaultListBoxSize.Height)));
    defaultComboBoxSizeElement.Set(SizeF(HorizontalPixelsToMM(defaultComboBoxSize.Width), VerticalPixelsToMM(defaultComboBoxSize.Height)));
    defaultCheckBoxSizeElement.Set(SizeF(HorizontalPixelsToMM(defaultCheckBoxSize.Width), VerticalPixelsToMM(defaultCheckBoxSize.Height)));
    defaultControlSpacingElement.Set(SizeF(HorizontalPixelsToMM(defaultControlSpacing.Width), VerticalPixelsToMM(defaultControlSpacing.Height)));
}

void Metrics::LoadFromFile(const std::string& fileName)
{
    if (std::filesystem::exists(fileName))
    {
        std::unique_ptr<soul::xml::Document> metricsDoc = soul::xml::ParseXmlFile(fileName);
        Read(metricsDoc->DocumentElement());
    }
}

void Metrics::SaveToFile(const std::string& fileName, bool setElementValues)
{
    if (setElementValues)
    {
        SetElementValues();
    }
    soul::xml::Document metricsDoc;
    metricsDoc.AppendChild(soul::xml::MakeElement("metrics"));
    metricsDoc.DocumentElement()->AppendChild(soul::xml::MakeComment("metrics are in millimeters"));
    metricsDoc.DocumentElement()->AppendChild(soul::xml::MakeText("\n"));
    Write(metricsDoc.DocumentElement());
    std::ofstream metricsFile(fileName);
    util::CodeFormatter formatter(metricsFile);
    formatter.SetIndentSize(1);
    metricsDoc.Write(formatter);
}

void Metrics::Calculate(Graphics& graphics)
{
    dpiX = graphics.GetDpiX();
    dpiY = graphics.GetDpiY();
    defaultButtonSize.Width = MMToHorizontalPixels(defaultButtonSizeElement.Get().Width);
    defaultButtonSize.Height = MMToVerticalPixels(defaultButtonSizeElement.Get().Height);
    defaultLabelSize.Width = MMToHorizontalPixels(defaultLabelSizeElement.Get().Width);
    defaultLabelSize.Height = MMToVerticalPixels(defaultLabelSizeElement.Get().Height);
    defaultTextBoxSize.Width = MMToHorizontalPixels(defaultTextBoxSizeElement.Get().Width);
    defaultTextBoxSize.Height = MMToVerticalPixels(defaultTextBoxSizeElement.Get().Height);
    defaultListBoxSize.Width = MMToHorizontalPixels(defaultListBoxSizeElement.Get().Width);
    defaultListBoxSize.Height = MMToVerticalPixels(defaultListBoxSizeElement.Get().Height);
    defaultComboBoxSize.Width = MMToHorizontalPixels(defaultComboBoxSizeElement.Get().Width);
    defaultComboBoxSize.Height = MMToHorizontalPixels(defaultComboBoxSizeElement.Get().Height);
    defaultCheckBoxSize.Width = MMToHorizontalPixels(defaultCheckBoxSizeElement.Get().Width);
    defaultCheckBoxSize.Height = MMToVerticalPixels(defaultCheckBoxSizeElement.Get().Height);
    defaultControlSpacing.Width = MMToHorizontalPixels(defaultControlSpacingElement.Get().Width);
    defaultControlSpacing.Height = MMToVerticalPixels(defaultControlSpacingElement.Get().Height);
}

void Metrics::Read(soul::xml::Element* parentElement)
{
    defaultButtonSizeElement.Read(parentElement);
    defaultLabelSizeElement.Read(parentElement);
    defaultTextBoxSizeElement.Read(parentElement);
    defaultListBoxSizeElement.Read(parentElement);
    defaultComboBoxSizeElement.Read(parentElement);
    defaultCheckBoxSizeElement.Read(parentElement);
    defaultControlSpacingElement.Read(parentElement);
}

void Metrics::Write(soul::xml::Element* parentElement)
{
    defaultButtonSizeElement.Write(parentElement);
    defaultLabelSizeElement.Write(parentElement);
    defaultTextBoxSizeElement.Write(parentElement);
    defaultListBoxSizeElement.Write(parentElement);
    defaultComboBoxSizeElement.Write(parentElement);
    defaultCheckBoxSizeElement.Write(parentElement);
    defaultControlSpacingElement.Write(parentElement);
}

Metrics ScreenMetrics::metrics;

void ScreenMetrics::Load()
{
    metrics.LoadFromFile(GetDefaultMetricsFilePath());
}

void ScreenMetrics::Save(bool setElementValues)
{
    metrics.SaveToFile(GetDefaultMetricsFilePath(), setElementValues);
}

void ScreenMetrics::Calculate(Graphics& graphics)
{
    Load();
    metrics.Calculate(graphics);
}

void ScreenMetrics::Set(Metrics& metrics_)
{
    metrics = metrics_;
}

void LoadMetrics()
{
    ScreenMetrics::Load();
}

bool ScreenMetrics::calculated = false;

} // wing
