// JavaScript source code for drawing class inheritance diagrams and concept refinement diagrams

function pick(level, diagramNodes) {
    var levelNodes = [];
    var n = diagramNodes.length;
    for (var i = 0; i < n; ++i) {
        var c = diagramNodes[i];
        if (c.level == level) {
            levelNodes.push(c);
        }
    }
    return levelNodes;
}

function createDiagramNodeElements(levelNodes, maxTextWidth, maxTextHeight) {
    var textDimensions = { width: maxTextWidth, height: maxTextHeight };
    var n = levelNodes.length;
    for (var i = 0; i < n; ++i) {
        var levelNode = levelNodes[i];
        var svg = document.getElementById("inheritance_svg_diagram");
        var rectElement = document.createElementNS('http://www.w3.org/2000/svg', 'rect');
        rectElement.setAttribute("id", levelNode.id + "_rect");
        var linkElement = document.createElementNS('http://www.w3.org/2000/svg', 'a');
        linkElement.setAttribute("href", levelNode.link);
        var textElement = document.createElementNS('http://www.w3.org/2000/svg', 'text');
        linkElement.appendChild(textElement);
        textElement.setAttribute("id", levelNode.id + "_text");
        textElement.innerHTML = levelNode.name;
        svg.appendChild(rectElement);
        svg.appendChild(linkElement);
        var bb = textElement.getBBox();
        var textWidth = bb.width;
        var textHeight = bb.height;
        levelNode.textWidth = textWidth;
        levelNode.textHeight = textHeight;
        if (textWidth > textDimensions.width) {
            textDimensions.width = textWidth;
        }
        if (textHeight > textDimensions.height) {
            textDimensions.height = textHeight;
        }
    }
    return textDimensions;
}

function drawDiagram(diagramNodes) {
    var cont = true;
    var level = 0;
    var yspace = 8;
    var xspace = 8;
    var minRectWidth = 100;
    var minRectHeight = 40;
    var maxTextWidth = 0;
    var maxTextHeight = 0;
    var triangleHeight = 20;
    var triangleWidth = 20;
    var targetHandleHeight = 20;
    var sourceHandleHeight = 40;
    var rectXSpace = 20;
    var horizontalRectYSpace = triangleHeight + targetHandleHeight + sourceHandleHeight;
    var verticalRectYSpace = 20;
    var derivedTriangleHeight = 8;
    var derivedTriangleWidth = 8;
    var widthThreshold = 1800;
    var allLevelNodes = [];
    while (cont) {
        var levelNodes = pick(level, diagramNodes);
        var n = levelNodes.length;
        cont = n > 0;
        if (cont) {
            var textDimensions = createDiagramNodeElements(levelNodes, maxTextWidth, maxTextHeight);
            if (textDimensions.width > maxTextWidth) {
                maxTextWidth = textDimensions.width;
            }
            if (textDimensions.height > maxTextHeight) {
                maxTextHeight = textDimensions.height;
            }
            allLevelNodes.push(levelNodes);
            ++level;
        }
    }
    var maxLevel = level;
    var rectWidth = Math.max(minRectWidth, maxTextWidth + 2 * xspace);
    var rectHeight = Math.max(minRectHeight, maxTextHeight + 2 * yspace);
    var totalWidth = 0;
    var totalHeight = 0;
    var horizontalDirection = 0;
    var verticalDirection = 1;
    var levelDirection = [];
    var levelHeight = [];
    var prevW = 0;
    for (level = 0; level < maxLevel; ++level) {
        var levelNodes = allLevelNodes[level];
        var n = levelNodes.length;
        var w = n * (rectWidth + rectXSpace);
        var h = rectHeight + horizontalRectYSpace;
        if (w < widthThreshold) {
            levelDirection.push(horizontalDirection);
            if (w > totalWidth) {
                totalWidth = w;
            }
        }
        else {
            w = prevW + rectWidth + rectXSpace;
            h = n * (rectHeight + verticalRectYSpace);
            levelDirection.push(verticalDirection);
            totalWidth += w;
        }
        totalHeight += h;
        levelHeight.push(h);
        prevW = w;
    }
    var svg = document.getElementById("inheritance_svg_diagram");
    svg.setAttribute("width", totalWidth.toString());
    svg.setAttribute("height", totalHeight.toString());
    var prevRectY = 0;
    var prevRectX = 0;
    var prevHandleX2 = -1;
    var prevHandleY2 = -1;
    var prevY = 0;
    for (level = 0; level < maxLevel; ++level) {
        var direction = levelDirection[level];
        var levelNodes = allLevelNodes[level];
        var n = levelNodes.length;
        var rectY = prevY;
        prevY += levelHeight[level];
        var rectX = (totalWidth / n - rectWidth) / 2;
        var minHandleX = Number.MAX_SAFE_INTEGER;
        var maxHandleX = 0;
        var handleY = 0;
        for (var i = 0; i < n; ++i) {
            var levelNode = levelNodes[i];
            var textWidth = levelNode.textWidth;
            var textHeight = levelNode.textHeight;
            if (direction == horizontalDirection) {
                rectX = (totalWidth / n - rectWidth) / 2 + i * (rectWidth + rectXSpace);
            }
            else if (direction == verticalDirection) {
                rectX = prevRectX + (rectWidth + rectXSpace);
                rectY = prevRectY + horizontalRectYSpace + i * (rectHeight + verticalRectYSpace);
            }
            var textX = rectX + (rectWidth - textWidth) / 2;
            var textY = (rectY + rectHeight - yspace / 2) - (rectHeight - textHeight) / 2;
            var rectElement = document.getElementById(levelNode.id + "_rect");
            rectElement.setAttribute("x", rectX.toString());
            rectElement.setAttribute("y", rectY.toString());
            rectElement.setAttribute("width", rectWidth.toString());
            rectElement.setAttribute("height", rectHeight.toString());
            var fillColor = "white";
            if (levelNode.subject) {
                fillColor = "floralWhite";
            }
            rectElement.setAttribute("fill", fillColor);
            rectElement.setAttribute("stroke", "black");
            var textElement = document.getElementById(levelNode.id + "_text");
            textElement.setAttribute("x", textX.toString());
            textElement.setAttribute("y", textY.toString());
            if (level < maxLevel - 1) {
                var triangleElement = document.createElementNS('http://www.w3.org/2000/svg', 'polygon');
                var tipX = rectX + rectWidth / 2;
                var tipY = rectY + rectHeight;
                var leftX = tipX - triangleWidth / 2;
                var leftY = rectY + rectHeight + triangleHeight;
                var rightX = tipX + triangleWidth / 2;
                var rightY = rectY + rectHeight + triangleHeight;
                triangleElement.setAttribute("points",
                    tipX.toString() + "," + tipY.toString() + " " +
                    leftX.toString() + "," + leftY.toString() + " " +
                    rightX.toString() + "," + rightY.toString());
                triangleElement.setAttribute("fill", "white");
                triangleElement.setAttribute("stroke", "black");
                svg.appendChild(triangleElement);
                var targetHandleElement = document.createElementNS('http://www.w3.org/2000/svg', 'line');
                var handleX1 = tipX;
                var handleY1 = tipY + triangleHeight;
                var handleX2 = tipX;
                var handleY2 = tipY + triangleHeight + targetHandleHeight;
                targetHandleElement.setAttribute("x1", handleX1.toString());
                targetHandleElement.setAttribute("y1", handleY1.toString());
                targetHandleElement.setAttribute("x2", handleX2.toString());
                targetHandleElement.setAttribute("y2", handleY2.toString());
                targetHandleElement.setAttribute("stroke", "black");
                svg.appendChild(targetHandleElement);
                prevHandleX2 = handleX1;
                prevHandleY2 = handleY1;
                if (handleX1 < minHandleX) {
                    minHandleX = handleX1;
                    handleY = handleY2;
                }
                if (handleX1 > maxHandleX) {
                    maxHandleX = handleX1;
                    handleY = handleY2;
                }
            } else if (level == maxLevel - 1 && levelNode.hasDerivedNodes) {
                var derivedTriangleElement = document.createElementNS('http://www.w3.org/2000/svg', 'polygon');
                var cornerX = rectX + rectWidth;
                var cornerY = rectY + rectHeight;
                var verticalX = rectX + rectWidth;
                var verticalY = rectY + rectHeight - derivedTriangleHeight;
                var horizontalX = rectX + rectWidth - derivedTriangleWidth;
                var horizontalY = rectY + rectHeight;
                derivedTriangleElement.setAttribute("points",
                    cornerX.toString() + "," + cornerY.toString() + " " +
                    verticalX.toString() + "," + verticalY.toString() + " " +
                    horizontalX.toString() + "," + horizontalY.toString());
                derivedTriangleElement.setAttribute("fill", "black");
                svg.appendChild(derivedTriangleElement);
            }
            if (level > 0 && direction == horizontalDirection) {
                var sourceHandleElement = document.createElementNS('http://www.w3.org/2000/svg', 'line');
                var handleX1 = rectX + rectWidth / 2;
                var handleY1 = rectY;
                var handleX2 = rectX + rectWidth / 2;
                var handleY2 = rectY - sourceHandleHeight;
                sourceHandleElement.setAttribute("x1", handleX1.toString());
                sourceHandleElement.setAttribute("y1", handleY1.toString());
                sourceHandleElement.setAttribute("x2", handleX2.toString());
                sourceHandleElement.setAttribute("y2", handleY2.toString());
                sourceHandleElement.setAttribute("stroke", "black");
                svg.appendChild(sourceHandleElement);
                if (handleX1 < minHandleX) {
                    minHandleX = handleX1;
                    handleY = handleY2;
                }
                if (handleX1 > maxHandleX) {
                    maxHandleX = handleX1;
                    handleY = handleY2;
                }
            }
            else if (level > 0 && direction == verticalDirection) {
                var sourceHandleElement = document.createElementNS('http://www.w3.org/2000/svg', 'line');
                var handleX1 = rectX;
                var handleY1 = rectY + rectHeight / 2;
                var handleX2 = rectX - rectWidth / 2 - rectXSpace;
                var handleY2 = rectY + rectHeight / 2;
                sourceHandleElement.setAttribute("x1", handleX1.toString());
                sourceHandleElement.setAttribute("y1", handleY1.toString());
                sourceHandleElement.setAttribute("x2", handleX2.toString());
                sourceHandleElement.setAttribute("y2", handleY2.toString());
                sourceHandleElement.setAttribute("stroke", "black");
                svg.appendChild(sourceHandleElement);
                if (prevHandleX2 != -1 && prevHandleY2 != -1) {
                    var connectorHandleElement = document.createElementNS('http://www.w3.org/2000/svg', 'line');
                    connectorHandleElement.setAttribute("x1", handleX2.toString());
                    connectorHandleElement.setAttribute("y1", handleY2.toString());
                    connectorHandleElement.setAttribute("x2", prevHandleX2.toString());
                    connectorHandleElement.setAttribute("y2", prevHandleY2.toString());
                    connectorHandleElement.setAttribute("stroke", "black");
                    svg.appendChild(connectorHandleElement);
                }
                prevHandleX2 = handleX2
                prevHandleY2 = handleY2;
            }
        }
        if (minHandleX < maxHandleX && direction == horizontalDirection) {
            var hlineElement = document.createElementNS('http://www.w3.org/2000/svg', 'line');
            hlineElement.setAttribute("x1", minHandleX.toString());
            hlineElement.setAttribute("y1", handleY.toString());
            hlineElement.setAttribute("x2", maxHandleX.toString());
            hlineElement.setAttribute("y2", handleY.toString());
            hlineElement.setAttribute("stroke", "black");
            svg.appendChild(hlineElement);
        }
        prevRectY = rectY;
        prevRectX = rectX;
    }
}

function drawInheritanceDiagram() {
    var diagramNodes = [
        { name: "System.IO.IOBase", id: "diagram_node_0", level: 0, subject: true, hasDerivedNodes: true, link: "../../System.Base/doc/class_IOBase_325A635E11759F146842FF3D54DFAE621782FC86.html" },
        { name: "System.AnsiEngine", id: "diagram_node_1", level: 1, subject: false, hasDerivedNodes: false, link: "../../System.Base/doc/class_AnsiEngine_E020B478179ED33FC5043514986AD3C67A975E2B.html" },
        { name: "System.IO.BinaryReader", id: "diagram_node_5", level: 1, subject: false, hasDerivedNodes: false, link: "../../System.Base/doc/class_BinaryReader_7D0A41A9C666814155001B1AF5B066E926C8F3AC.html" },
        { name: "System.IO.BinaryWriter", id: "diagram_node_6", level: 1, subject: false, hasDerivedNodes: false, link: "../../System.Base/doc/class_BinaryWriter_6D47712E9CF8C098A17FFF3729140EE727BD87E2.html" },
        { name: "System.IO.LogWriter", id: "diagram_node_11", level: 1, subject: false, hasDerivedNodes: false, link: "../../System.Base/doc/class_LogWriter_B4272A03687DE3F3A77648E66827E88F61C72BFC.html" },
        { name: "System.IO.MemoryReader", id: "diagram_node_12", level: 1, subject: false, hasDerivedNodes: false, link: "../../System.Base/doc/class_MemoryReader_7C2B5BA19453DDDA4428C8201D93E07311596E12.html" },
        { name: "System.IO.MemoryWriter", id: "diagram_node_13", level: 1, subject: false, hasDerivedNodes: false, link: "../../System.Base/doc/class_MemoryWriter_BC3E557B84654B5DB52F9A019B7E85AF4A0D7C48.html" },
        { name: "System.IO.Stream", id: "diagram_node_17", level: 1, subject: false, hasDerivedNodes: false, link: "../../System.Base/doc/class_Stream_5B86812CFF4EC38459E63BA53D90E1A7AEE57A5B.html" },
        { name: "System.IO.StreamReader", id: "diagram_node_16", level: 1, subject: false, hasDerivedNodes: false, link: "../../System.Base/doc/class_StreamReader_8A8267CD81BA8B198AF295474A8CB0B2DFBB7B57.html" },
        { name: "System.IO.TextWriter", id: "diagram_node_20", level: 1, subject: false, hasDerivedNodes: false, link: "../../System.Base/doc/class_TextWriter_95D72C37DDCBB941FE8EC41FF28270B9FFB782C1.html" },
        { name: "System.Lex.Lexer", id: "diagram_node_10", level: 1, subject: false, hasDerivedNodes: false, link: "../../System.Lex/doc/class_Lexer_156A4FBAF81A8279F7218814A16ED516A1B97F23.html" },
        { name: "System.Net.Sockets.TcpListener", id: "diagram_node_18", level: 1, subject: false, hasDerivedNodes: false, link: "../../System.Net.Sockets/doc/class_TcpListener_113AF33A61527FBDFBFD58155CAD89A443C91B0B.html" },
        { name: "System.Net.Sockets.TcpSocket", id: "diagram_node_19", level: 1, subject: false, hasDerivedNodes: false, link: "../../System.Net.Sockets/doc/class_TcpSocket_B38B3FFF6145E508D0B49BC2B9D97F255711ED1C.html" },
        { name: "System.Text.CodeFormatter", id: "diagram_node_8", level: 1, subject: false, hasDerivedNodes: false, link: "../../System.Base/doc/class_CodeFormatter_38898A0914E7D27753F0D42E65A18EC272984F99.html" },
        { name: "System.Unicode.BidiClassTable", id: "diagram_node_2", level: 1, subject: false, hasDerivedNodes: false, link: "../../System.Base/doc/class_BidiClassTable_C2D5046C361B79F9A690C84020C0EA764E7E73C5.html" },
        { name: "System.Unicode.BidiPairedBracketTypeTable", id: "diagram_node_3", level: 1, subject: false, hasDerivedNodes: false, link: "../../System.Base/doc/class_BidiPairedBracketTypeTable_0064D862A1F33AB5C9E09D707B6EC98CAC6E1BA7.html" },
        { name: "System.Unicode.BinaryPropertyTable", id: "diagram_node_4", level: 1, subject: false, hasDerivedNodes: false, link: "../../System.Base/doc/class_BinaryPropertyTable_D9F900B43A497D516547526966A7747D0329EEEA.html" },
        { name: "System.Unicode.BlockTable", id: "diagram_node_7", level: 1, subject: false, hasDerivedNodes: false, link: "../../System.Base/doc/class_BlockTable_6443121076F39E567B3BAACE0620D0ABC989F5D7.html" },
        { name: "System.Unicode.GeneralCategoryTable", id: "diagram_node_9", level: 1, subject: false, hasDerivedNodes: false, link: "../../System.Base/doc/class_GeneralCategoryTable_3109B631555A0B27CBC86AC6578E1E5835FD0ADE.html" },
        { name: "System.Unicode.NumericTypeTable", id: "diagram_node_14", level: 1, subject: false, hasDerivedNodes: false, link: "../../System.Base/doc/class_NumericTypeTable_D5D406F33CCAC2C03087278A817836F0F3A3D92A.html" },
        { name: "System.Unicode.ScriptTable", id: "diagram_node_15", level: 1, subject: false, hasDerivedNodes: false, link: "../../System.Base/doc/class_ScriptTable_CEB3F0C6961FADA195D3CEC50AC41CB1955D92CF.html" },
        { name: "System.Unicode.UnicodeEngine", id: "diagram_node_21", level: 1, subject: false, hasDerivedNodes: false, link: "../../System.Base/doc/class_UnicodeEngine_4BA56D0BC89EA412C5D44E2EAC43EF3D52AA83DE.html" }];
    drawDiagram(diagramNodes);
}

