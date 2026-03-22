// ExprPage.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    readonly property color cNavy:   "#1a2f5e"
    readonly property color cAccent: "#0097b2"
    readonly property color cRed:    "#ef5350"
    readonly property color cWhite:  "#ffffff"
    readonly property color cBorder: "#dce6f0"
    readonly property color cBg:     "#f8fafd"
    readonly property color cDim:    "#7a96b8"
    readonly property color cGreen:  "#2e7d32"

    property string resultText: "25.5"
    property bool   hasError:   false

    // Syntax-highlighted C++ code — no backticks, no lineHeight on TextEdit
    readonly property string codeHtml:
        "<span style='color:#7952b3'>struct</span> <span style='color:#1a2f5e'>Expression</span> {<br>" +
        "&nbsp;&nbsp;<span style='color:#7952b3'>virtual</span> <span style='color:#7952b3'>double</span> " +
        "<span style='color:#0097b2'>evaluate</span>() = <span style='color:#2e7d32'>0</span>;<br>" +
        "&nbsp;&nbsp;<span style='color:#7952b3'>virtual</span> ~<span style='color:#0097b2'>Expression</span>()" +
        " = <span style='color:#7952b3'>default</span>;<br>};<br><br>" +
        "<span style='color:#7952b3'>struct</span> <span style='color:#1a2f5e'>Number</span> : " +
        "<span style='color:#1a2f5e'>Expression</span> {<br>" +
        "&nbsp;&nbsp;<span style='color:#7952b3'>private:</span><br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#7952b3'>double</span> value;<br>" +
        "&nbsp;&nbsp;<span style='color:#7952b3'>public:</span><br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#0097b2'>Number</span>(<span style='color:#7952b3'>double</span> v) : value(v) {}<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#7952b3'>double</span> <span style='color:#0097b2'>evaluate</span>()" +
        " <span style='color:#7952b3'>override</span> { <span style='color:#7952b3'>return</span> value; }<br>};<br><br>" +
        "<span style='color:#7952b3'>struct</span> <span style='color:#1a2f5e'>BinaryOperation</span> : " +
        "<span style='color:#1a2f5e'>Expression</span> {<br>" +
        "&nbsp;&nbsp;<span style='color:#7952b3'>private:</span><br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#1a2f5e'>Expression</span> *left, *right;<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#7952b3'>char</span> op;<br>" +
        "&nbsp;&nbsp;<span style='color:#7952b3'>public:</span><br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#0097b2'>BinaryOperation</span>(<span style='color:#1a2f5e'>Expression</span>* l," +
        " <span style='color:#7952b3'>char</span> o, <span style='color:#1a2f5e'>Expression</span>* r)<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;: left(l), op(o), right(r) {}<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#7952b3'>double</span> <span style='color:#0097b2'>evaluate</span>()" +
        " <span style='color:#7952b3'>override</span> {<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#7952b3'>try</span> {<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#7952b3'>double</span> l = left-><span style='color:#0097b2'>evaluate</span>();<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#7952b3'>double</span> r = right-><span style='color:#0097b2'>evaluate</span>();<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#7952b3'>if</span>(op=='/' &amp;&amp; r==0) <span style='color:#7952b3'>throw</span>;<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#7952b3'>if</span>(op=='+') <span style='color:#7952b3'>return</span> l+r;<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#7952b3'>if</span>(op=='-') <span style='color:#7952b3'>return</span> l-r;<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#7952b3'>if</span>(op=='*') <span style='color:#7952b3'>return</span> l*r;<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#7952b3'>return</span> l/r;<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;} <span style='color:#7952b3'>catch</span>(...) { <span style='color:#7952b3'>throw</span>; }<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;}<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;~<span style='color:#0097b2'>BinaryOperation</span>() { <span style='color:#7952b3'>delete</span> left; <span style='color:#7952b3'>delete</span> right; }<br>};"

    RowLayout {
        anchors.fill: parent
        anchors.margins: 14
        spacing: 12

        // Left: code + eval
        ColumnLayout {
            Layout.fillWidth: true; Layout.fillHeight: true; spacing: 10

            Rectangle {
                Layout.fillWidth: true; Layout.fillHeight: true
                color: cWhite; border.color: cBorder; border.width: 1; radius: 6

                ColumnLayout {
                    anchors.fill: parent; spacing: 0

                    // File tabs bar
                    Rectangle {
                        Layout.fillWidth: true; height: 38
                        color: "#f0f4fa"; radius: 6
                        Rectangle { anchors.bottom:parent.bottom; width:parent.width; height:8; color:"#f0f4fa" }
                        Rectangle { anchors.bottom:parent.bottom; width:parent.width; height:1; color:cBorder }

                        Row {
                            anchors.fill: parent; anchors.leftMargin: 8; spacing: 0
                            Repeater {
                                model: ["expression.h", "expression.cpp"]
                                delegate: Rectangle {
                                    width: ftl.implicitWidth + 24; height: 38
                                    color: index === 0 ? cWhite : "transparent"
                                    border.color: index === 0 ? cBorder : "transparent"
                                    border.width: 1
                                    // Red bottom for active tab
                                    Rectangle {
                                        anchors.bottom: parent.bottom
                                        width: parent.width; height: index === 0 ? 2 : 0
                                        color: cAccent
                                    }
                                    Text {
                                        id: ftl; anchors.centerIn: parent
                                        text: modelData
                                        color: index === 0 ? cNavy : cDim
                                        font.pixelSize: 12; font.family: "Arial"
                                        font.weight: index === 0 ? Font.Bold : Font.Normal
                                    }
                                }
                            }
                        }
                    }

                    // Code area with line numbers
                    Flickable {
                        Layout.fillWidth: true; Layout.fillHeight: true
                        contentHeight: codeEdit.implicitHeight + 24; clip: true

                        ScrollBar.vertical: ScrollBar {
                            policy: ScrollBar.AsNeeded
                            contentItem: Rectangle { radius: 3; color: "#c8d4e8" }
                            background: Rectangle { color: "transparent" }
                        }

                        // Line numbers
                        Rectangle {
                            x:0; y:0; width:36
                            height: Math.max(parent.contentHeight, parent.height)
                            color: "#f0f4fa"
                            Rectangle { anchors.right:parent.right; width:1; height:parent.height; color:cBorder }

                            Column {
                                anchors.fill: parent; anchors.topMargin: 14
                                Repeater {
                                    model: 36
                                    delegate: Text {
                                        width: 30; text: index+1
                                        color: cDim; font.pixelSize: 11; font.family: "Consolas"
                                        horizontalAlignment: Text.AlignRight; height: 19
                                    }
                                }
                            }
                        }

                        TextEdit {
                            id: codeEdit; x:44; y:12
                            width: parent.width - 50
                            readOnly: true; selectByMouse: true
                            font.pixelSize: 13; font.family: "Consolas"
                            color: cNavy; wrapMode: TextEdit.NoWrap
                            textFormat: TextEdit.RichText; text: codeHtml
                        }
                    }
                }
            }

            // Eval bar
            AppToolBar {
                Layout.fillWidth: true
                Text { text: "Выражение:"; color: cDim; font.pixelSize: 13; font.family: "Arial" }
                AppInput { id: exprInput; implicitWidth: 220; text: "3 + 4.5 * 5"; Keys.onReturnPressed: evalBtn.clicked() }
                property var astTree: null

AppButton {
    id: evalBtn; text: "\u25B6  Вычислить"; variant: "primary"
    onClicked: {
        if (typeof exprBE === "undefined") return
        var r = exprBE.evaluate(exprInput.text)
        resultText = r
        hasError = r.startsWith("Ошибка")
        if (!hasError) {
            astTree = exprBE.parseTree(exprInput.text)  // ← C++ строит дерево
            astCanvas.requestPaint()
        }
    }
}
                AppButton { text: "\u2715 Очистить"; onClicked: { exprInput.text=""; resultText="\u2014"; hasError=false } }
                ToolSep {}
                Text { text: "Результат:"; color: cDim; font.pixelSize: 13; font.family: "Arial" }
                Rectangle {
                    height: 30; width: resultLabel.implicitWidth + 20; radius: 15
                    color: hasError ? "#fdecea" : "#e8f5e9"
                    border.color: hasError ? "#ef9a9a" : "#a5d6a7"
                    Text {
                        id: resultLabel; anchors.centerIn: parent
                        text: resultText
                        color: hasError ? cRed : cGreen
                        font.pixelSize: 15; font.family: "Arial"; font.weight: Font.Bold
                    }
                }
            }
        }

        // Right: AST tree
        Rectangle {
            Layout.preferredWidth: 290; Layout.minimumWidth: 260; Layout.maximumWidth: 310; Layout.fillHeight: true
            color: cWhite; border.color: cBorder; border.width: 1; radius: 6

            ColumnLayout {
                anchors.fill: parent; spacing: 0

                Rectangle {
                    Layout.fillWidth: true; height: 38
                    color: "#f0f4fa"; radius: 6
                    Rectangle { anchors.bottom:parent.bottom; width:parent.width; height:8; color:"#f0f4fa" }
                    Rectangle { anchors.bottom:parent.bottom; width:parent.width; height:1; color:cBorder }
                    Text {
                        anchors.fill: parent; anchors.leftMargin: 14
                        text: "AST: 3 + (4.5 \u00D7 5)"
                        color: cNavy; font.pixelSize: 13; font.family: "Arial"; font.weight: Font.Bold
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Canvas {
                    id: astCanvas; Layout.fillWidth: true; height: 240
                    onPaint: {
                        var ctx = getContext("2d")
                        ctx.clearRect(0,0,width,height)

                        function rr(x,y,w,h,r) {
                            ctx.beginPath()
                            ctx.moveTo(x+r,y); ctx.lineTo(x+w-r,y)
                            ctx.arcTo(x+w,y,  x+w,y+r,  r); ctx.lineTo(x+w,y+h-r)
                            ctx.arcTo(x+w,y+h,x+w-r,y+h,r); ctx.lineTo(x+r,y+h)
                            ctx.arcTo(x,y+h,  x,y+h-r,  r); ctx.lineTo(x,y+r)
                            ctx.arcTo(x,y,    x+r,y,    r); ctx.closePath()
                        }

                        ctx.strokeStyle="#dce6f0"; ctx.lineWidth=2
                        ctx.beginPath(); ctx.moveTo(145,50); ctx.lineTo(80,110);  ctx.stroke()
                        ctx.beginPath(); ctx.moveTo(145,50); ctx.lineTo(210,110); ctx.stroke()
                        ctx.beginPath(); ctx.moveTo(210,110); ctx.lineTo(165,175); ctx.stroke()
                        ctx.beginPath(); ctx.moveTo(210,110); ctx.lineTo(255,175); ctx.stroke()

                        function drawOp(x,y,lbl) {
                            ctx.beginPath(); ctx.arc(x,y,24,0,Math.PI*2)
                            ctx.fillStyle="#1a2f5e"; ctx.fill()
                            ctx.strokeStyle="#ffffff"; ctx.lineWidth=2; ctx.stroke()
                            ctx.fillStyle="#ffffff"; ctx.font="bold 17px Arial"
                            ctx.textAlign="center"; ctx.textBaseline="middle"
                            ctx.fillText(lbl,x,y+1)
                        }
                        function drawNum(x,y,lbl) {
                            rr(x-28,y-15,56,30,6)
                            ctx.fillStyle="#f0f4fa"; ctx.fill()
                            ctx.strokeStyle="#c8d4e8"; ctx.lineWidth=1.5; ctx.stroke()
                            ctx.fillStyle="#1a2f5e"; ctx.font="bold 13px Arial"
                            ctx.textAlign="center"; ctx.textBaseline="middle"
                            ctx.fillText(lbl,x,y+1)
                        }

                        drawOp(145,48,"+")
                        drawNum(80, 110,"3")
                        drawOp(210,110,"\u00D7")
                        drawNum(165,175,"4.5")
                        drawNum(255,175,"5")

                        // Legend
                        ctx.beginPath(); ctx.arc(20,222,8,0,Math.PI*2)
                        ctx.fillStyle="#1a2f5e"; ctx.fill()
                        ctx.fillStyle="#7a96b8"; ctx.font="10px Arial"
                        ctx.textAlign="left"; ctx.textBaseline="middle"
                        ctx.fillText("BinaryOperation", 34, 222)

                        rr(125,214,18,16,3)
                        ctx.fillStyle="#f0f4fa"; ctx.fill()
                        ctx.strokeStyle="#c8d4e8"; ctx.lineWidth=1; ctx.stroke()
                        ctx.fillStyle="#7a96b8"
                        ctx.fillText("Number", 150, 222)
                    }
                }

                Rectangle {
                    Layout.fillWidth: true; height: 52
                    color: "#f0f4fa"
                    border.color: cBorder; border.width: 0
                    Rectangle { anchors.top:parent.top; width:parent.width; height:1; color:cBorder }
                    Text {
                        anchors.fill: parent; anchors.margins: 10
                        text: "new BinaryOperation(\n  new Number(3), '+',\n  new BinaryOperation(...))"
                        color: "#7952b3"; font.pixelSize: 10; font.family: "Consolas"; wrapMode: Text.Wrap
                    }
                }

                Item { Layout.fillHeight: true }
            }
        }
    }
}
