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

    property string resultText: "—"
    property bool   hasError:   false

    function treeDepth(node) {
        if (!node || !node.isOp) return 1
        return 1 + Math.max(treeDepth(node.left), treeDepth(node.right))
    }
    function treeWidth(node) {
        if (!node || !node.isOp) return 70
        return treeWidth(node.left) + treeWidth(node.right) + 20
    }
    function buildCodeText(node, indent) {
        if (!node) return ""
        if (!node.isOp) return indent + "new Number(" + node.label + ")"
        return indent + "new BinaryOperation(\n"
            + buildCodeText(node.left,  indent + "  ") + ",\n"
            + indent + "  '" + node.label + "',\n"
            + buildCodeText(node.right, indent + "  ") + ")"
    }
    function astLabelText(node) {
        if (!node) return ""
        if (!node.isOp) return node.label
        var ops = {"*":"×", "/":"÷"}
        return astLabelText(node.left) + " " + (ops[node.label] || node.label) + " " + astLabelText(node.right)
    }

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
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#7952b3'>double</span> l = left-><span style='color:#0097b2'>evaluate</span>();<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#7952b3'>double</span> r = right-><span style='color:#0097b2'>evaluate</span>();<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#7952b3'>if</span>(op=='/' &amp;&amp; r==0) <span style='color:#7952b3'>throw</span>;<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#7952b3'>if</span>(op=='+') <span style='color:#7952b3'>return</span> l+r;<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#7952b3'>if</span>(op=='-') <span style='color:#7952b3'>return</span> l-r;<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#7952b3'>if</span>(op=='*') <span style='color:#7952b3'>return</span> l*r;<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style='color:#7952b3'>return</span> l/r;<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;}<br>" +
        "&nbsp;&nbsp;&nbsp;&nbsp;~<span style='color:#0097b2'>BinaryOperation</span>() { <span style='color:#7952b3'>delete</span> left; <span style='color:#7952b3'>delete</span> right; }<br>};"

    AppErrorDialog { id: errDialog }
    Connections {
        target: typeof exprBE !== "undefined" ? exprBE : null
        function onError(msg) { errDialog.show(msg) }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 14
        spacing: 12

        // ── Left ──────────────────────────────────────────────
        ColumnLayout {
            Layout.fillWidth: true; Layout.fillHeight: true; spacing: 10

            Rectangle {
                Layout.fillWidth: true; Layout.fillHeight: true
                color: cWhite; border.color: cBorder; border.width: 1; radius: 6
                ColumnLayout {
                    anchors.fill: parent; spacing: 0
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
                                    Rectangle { anchors.bottom:parent.bottom; width:parent.width; height:index===0?2:0; color:cAccent }
                                    Text { id:ftl; anchors.centerIn:parent; text:modelData; color:index===0?cNavy:cDim; font.pixelSize:12; font.family:"Arial"; font.weight:index===0?Font.Bold:Font.Normal }
                                }
                            }
                        }
                    }
                    Flickable {
                        Layout.fillWidth: true; Layout.fillHeight: true
                        contentHeight: codeEdit.implicitHeight + 24; clip: true
                        ScrollBar.vertical: ScrollBar { policy:ScrollBar.AsNeeded; 
                        contentItem:Rectangle{radius:3;color:"#c8d4e8"}
                         background:Rectangle{color:"transparent"} }
                        Rectangle {
                            x:0;y:0;width:36; height:Math.max(parent.contentHeight,parent.height); color:"#f0f4fa"
                            Rectangle { anchors.right:parent.right; width:1; height:parent.height; color:cBorder }
                            Column { anchors.fill:parent; anchors.topMargin:14
                                Repeater { model:36; delegate:Text{width:30;text:index+1;color:cDim;font.pixelSize:11;font.family:"Consolas";horizontalAlignment:Text.AlignRight;height:19} }
                            }
                        }
                        TextEdit { id:codeEdit; x:44;y:12; width:parent.width-50; readOnly:true; selectByMouse:true; font.pixelSize:13; font.family:"Consolas"; color:cNavy; wrapMode:TextEdit.NoWrap; textFormat:TextEdit.RichText; text:codeHtml }
                    }
                }
            }

            AppToolBar {
                Layout.fillWidth: true
                Text { text:"Выражение:"; color:cDim; font.pixelSize:13; font.family:"Arial" }
                AppInput { id:exprInput; implicitWidth:220; text:"3 + 4 * 5"; Keys.onReturnPressed:evalBtn.clicked() }
                AppButton {
                    id:evalBtn; text:"\u25B6  Вычислить"; variant:"primary"
                    onClicked: {
    if (typeof exprBE === "undefined") return
    var r = exprBE.evaluate(exprInput.text)
    resultText = r
    hasError = r.startsWith("Ошибка")
    if (!hasError) {
        astCanvas.tree = exprBE.parseTree(exprInput.text)
        // onTreeChanged сам вызовет requestPaint()
    }
}
                }
                AppButton {
                    text:"\u2715 Очистить"
                    onClicked: {
                        exprInput.text=""; resultText="—"; hasError=false
                        astCanvas.tree = null
                        astCanvas.canvasSize = Qt.size(270, 220)
                        Qt.callLater(function() { astCanvas.requestPaint() })
                    }
                }
                ToolSep {}
                Text { text:"Результат:"; color:cDim; font.pixelSize:13; font.family:"Arial" }
                Rectangle {
                    height:30; width:resultLabel.implicitWidth+20; radius:15
                    color:hasError?"#fdecea":"#e8f5e9"; border.color:hasError?"#ef9a9a":"#a5d6a7"
                    Text { id:resultLabel; anchors.centerIn:parent; text:resultText; color:hasError?cRed:cGreen; font.pixelSize:15; font.family:"Arial"; font.weight:Font.Bold }
                }
            }
        }

        // ── Right: AST panel ──────────────────────────────────
        Rectangle {
            Layout.preferredWidth: 290; Layout.minimumWidth: 260; Layout.maximumWidth: 320
            Layout.fillHeight: true
            color: cWhite; border.color: cBorder; border.width: 1; radius: 6

            ColumnLayout {
                anchors.fill: parent; spacing: 0

                // Заголовок
                Rectangle {
                    Layout.fillWidth: true; height: 38
                    color: "#f0f4fa"; radius: 6
                    Rectangle { anchors.bottom:parent.bottom; width:parent.width; height:8; color:"#f0f4fa" }
                    Rectangle { anchors.bottom:parent.bottom; width:parent.width; height:1; color:cBorder }
                    Text {
                        anchors.fill: parent; anchors.leftMargin: 14
                        text: astCanvas.tree ? "AST: " + astLabelText(astCanvas.tree) : "AST"
                        color: cNavy; font.pixelSize: 13; font.family: "Arial"; font.weight: Font.Bold
                        verticalAlignment: Text.AlignVCenter; elide: Text.ElideRight
                    }
                }

                // Flickable + Canvas — буфер задаётся через canvasSize
                Flickable {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    contentWidth:  astCanvas.canvasSize.width
                    contentHeight: astCanvas.canvasSize.height

                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AsNeeded
                        contentItem: Rectangle { radius:3; color:"#c8d4e8" }
                        background: Rectangle { color:"transparent" }
                    }

                    Canvas {
    id: astCanvas
    // фиксированный большой буфер — Flickable обеспечит скролл
    width:  600
    height: 600

    property var tree: null

    onTreeChanged: requestPaint()

    onPaint: {
        var ctx = getContext("2d")
        ctx.clearRect(0, 0, width, height)

        if (!tree || !tree.label) {
            ctx.fillStyle = "#7a96b8"
            ctx.font = "13px Arial"
            ctx.textAlign = "center"; ctx.textBaseline = "middle"
            ctx.fillText("Введите выражение и нажмите Вычислить", width/2, height/2)
            return
        }

        function drawOp(x, y, lbl) {
            ctx.beginPath(); ctx.arc(x, y, 22, 0, Math.PI*2)
            ctx.fillStyle = "#1a2f5e"; ctx.fill()
            ctx.fillStyle = "#ffffff"; ctx.font = "bold 16px Arial"
            ctx.textAlign = "center"; ctx.textBaseline = "middle"
            var ops = {"*":"×", "/":"÷"}
            ctx.fillText(ops[lbl] || lbl, x, y+1)
        }
        function drawNum(x, y, lbl) {
    var w = Math.max(50, lbl.length * 9 + 16)
    var h = 28, r = 5
    var x0 = x - w/2, y0 = y - h/2

    // скруглённый прямоугольник без roundRect
    ctx.beginPath()
    ctx.moveTo(x0 + r, y0)
    ctx.lineTo(x0 + w - r, y0)
    ctx.arcTo(x0 + w, y0,     x0 + w, y0 + r,     r)
    ctx.lineTo(x0 + w, y0 + h - r)
    ctx.arcTo(x0 + w, y0 + h, x0 + w - r, y0 + h, r)
    ctx.lineTo(x0 + r, y0 + h)
    ctx.arcTo(x0,      y0 + h, x0,        y0 + h - r, r)
    ctx.lineTo(x0,     y0 + r)
    ctx.arcTo(x0,      y0,     x0 + r,    y0,     r)
    ctx.closePath()

    ctx.fillStyle = "#f0f4fa"; ctx.fill()
    ctx.strokeStyle = "#c8d4e8"; ctx.lineWidth = 1.5; ctx.stroke()

    ctx.fillStyle = "#1a2f5e"
    ctx.font = "bold 13px Arial"
    ctx.textAlign = "center"; ctx.textBaseline = "middle"
    ctx.fillText(lbl, x, y + 1)
}
        function nodeW(node) {
            if (!node || !node.isOp) return 70
            return nodeW(node.left) + nodeW(node.right) + 20
        }
        function draw(node, x, y) {
            if (!node) return
            if (!node.isOp) { drawNum(x, y, node.label); return }
            var lw = nodeW(node.left), rw = nodeW(node.right)
            var half = (lw + rw + 20) / 2
            var lx = x - half + lw/2, rx = x + half - rw/2, ny = y + 80
            ctx.strokeStyle = "#dce6f0"; ctx.lineWidth = 2
            ctx.beginPath(); ctx.moveTo(x, y+22); ctx.lineTo(lx, ny-22); ctx.stroke()
            ctx.beginPath(); ctx.moveTo(x, y+22); ctx.lineTo(rx, ny-22); ctx.stroke()
            drawOp(x, y, node.label)
            draw(node.left, lx, ny)
            draw(node.right, rx, ny)
        }

        draw(tree, width/2, 40)

        // легенда
        var ly = 580
        ctx.beginPath(); ctx.arc(16, ly, 7, 0, Math.PI*2)
        ctx.fillStyle = "#1a2f5e"; ctx.fill()
        ctx.fillStyle = "#7a96b8"; ctx.font = "10px Arial"
        ctx.textAlign = "left"; ctx.textBaseline = "middle"
        ctx.fillText("BinaryOperation", 28, ly)
        ctx.fillStyle = "#f0f4fa"; ctx.strokeStyle = "#c8d4e8"; ctx.lineWidth = 1
        ctx.beginPath(); var lx = 130, ly2 = ly-8, lw = 18, lh = 16, lr = 3
ctx.beginPath()
ctx.moveTo(lx+lr, ly2)
ctx.lineTo(lx+lw-lr, ly2)
ctx.arcTo(lx+lw, ly2,    lx+lw, ly2+lr,    lr)
ctx.lineTo(lx+lw, ly2+lh-lr)
ctx.arcTo(lx+lw, ly2+lh, lx+lw-lr, ly2+lh, lr)
ctx.lineTo(lx+lr, ly2+lh)
ctx.arcTo(lx,     ly2+lh, lx, ly2+lh-lr,   lr)
ctx.lineTo(lx,    ly2+lr)
ctx.arcTo(lx,     ly2,    lx+lr, ly2,       lr)
ctx.closePath()
ctx.fillStyle = "#f0f4fa"; ctx.fill()
ctx.strokeStyle = "#c8d4e8"; ctx.lineWidth = 1; ctx.stroke(); ctx.fill(); ctx.stroke()
        ctx.fillStyle = "#7a96b8"; ctx.fillText("Number", 154, ly)
    }
}
                }

                // Динамический код
                Rectangle {
                    Layout.fillWidth: true
                    height: codeText.implicitHeight + 16
                    color: "#f0f4fa"
                    Rectangle { anchors.top:parent.top; width:parent.width; height:1; color:cBorder }
                    Text {
                        id: codeText
                        anchors.fill: parent; anchors.margins: 10
                        text: astCanvas.tree ? buildCodeText(astCanvas.tree, "") : "new BinaryOperation(\n  new Number(3), '+',\n  new BinaryOperation(...))"
                        color: "#7952b3"; font.pixelSize:10; font.family:"Consolas"; wrapMode:Text.Wrap
                    }
                }
            }
        }
    }
}
