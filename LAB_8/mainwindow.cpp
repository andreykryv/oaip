#include "mainwindow.h"
#include <QStyleFactory>
#include <QScrollArea>

// ════════════════════════════════════════════════════════════════
//  Глобальный stylesheet в дизайнерском стиле
// ════════════════════════════════════════════════════════════════
static QString globalStyle() {
    return R"(
QWidget {
    background: #080812;
    color: #e6e6f5;
    font-family: 'JetBrains Mono', 'Consolas', monospace;
    font-size: 13px;
}
QTabWidget::pane {
    border: 1px solid #28284a;
    background: #0b0b1a;
    border-radius: 8px;
}
QTabBar::tab {
    background: #111120;
    color: #8888aa;
    border: none;
    padding: 12px 22px;
    font-weight: 600;
    font-size: 13px;
    border-right: 1px solid #1e1e38;
}
QTabBar::tab:selected {
    background: #0b0b1a;
    color: #c8c8ff;
    border-bottom: 3px solid #7c4dff;
}
QTabBar::tab:hover:!selected {
    background: #15152a;
    color: #aaaacc;
}
QGroupBox {
    background: #0f0f22;
    border: 1px solid #28284a;
    border-radius: 10px;
    margin-top: 14px;
    padding: 14px;
    font-weight: 700;
    color: #9090cc;
}
QGroupBox::title {
    subcontrol-origin: margin;
    left: 14px;
    top: -8px;
    padding: 0 6px;
    background: #0f0f22;
    color: #7c4dff;
}
QLineEdit, QSpinBox, QComboBox {
    background: #14142a;
    color: #e6e6f5;
    border: 1px solid #2e2e50;
    border-radius: 6px;
    padding: 7px 11px;
    font-size: 13px;
    selection-background-color: #7c4dff;
}
QLineEdit:focus, QSpinBox:focus {
    border: 1px solid #7c4dff;
}
QPushButton {
    background: #7c4dff;
    color: #fff;
    border: none;
    border-radius: 7px;
    padding: 9px 22px;
    font-weight: 700;
    font-size: 13px;
}
QPushButton:hover {
    background: #9068ff;
}
QPushButton:pressed {
    background: #5a30cc;
}
QPushButton#sec {
    background: #1e1e38;
    color: #c8c8ff;
    border: 1px solid #3a3a60;
}
QPushButton#sec:hover {
    background: #2a2a50;
}
QPushButton#danger {
    background: #ff4757;
}
QPushButton#danger:hover {
    background: #ff6b7a;
}
QTextEdit {
    background: #0a0a18;
    color: #c8c8e8;
    border: 1px solid #28284a;
    border-radius: 8px;
    font-family: 'JetBrains Mono', 'Consolas', monospace;
    font-size: 12px;
    selection-background-color: #7c4dff;
}
QScrollBar:vertical {
    background: #0f0f22;
    width: 8px;
    border-radius: 4px;
}
QScrollBar::handle:vertical {
    background: #2a2a50;
    border-radius: 4px;
    min-height: 20px;
}
QScrollBar::handle:vertical:hover {
    background: #3a3a60;
}
QLabel {
    color: #a0a0c0;
}
    )";
}

// ════════════════════════════════════════════════════════════════
MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle("LAB_8 — Визуализация структур данных");
    resize(1200, 800);

    // Применяем стиль
    setStyleSheet(globalStyle());

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(16);

    m_tabs = new QTabWidget(this);
    layout->addWidget(m_tabs);

    setupBSTTab();
    setupArrayHeapTab();
    setupListHeapTab();
    setupHashTab();
}

void MainWindow::logTo(QTextEdit* log, const QString& msg) {
    // Удаляем HTML-теги для отображения
    QString plain = msg;
    plain.replace("<b>", "**").replace("</b>", "**");
    log->append(plain);
}

// ════════════════════════════════════════════════════════════════
void MainWindow::setupBSTTab() {
    auto* tab = new QWidget();
    auto* layout = new QHBoxLayout(tab);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(12);

    // Левая панель — визуализация
    m_bstWidget = new BSTVisWidget(tab);
    m_bstWidget->setMinimumSize(600, 500);
    connect(m_bstWidget, &BSTVisWidget::logMessage, this, [this](const QString& msg) {
        logTo(m_bstLog, msg);
    });
    layout->addWidget(m_bstWidget, 3);

    // Правая панель — управление
    auto* controlPanel = new QWidget();
    auto* ctrlLayout = new QVBoxLayout(controlPanel);
    ctrlLayout->setContentsMargins(0, 0, 0, 0);
    ctrlLayout->setSpacing(10);

    // Группа операций
    auto* opsGroup = new QGroupBox("Операции с деревом");
    auto* opsLayout = new QVBoxLayout(opsGroup);
    opsLayout->setSpacing(8);

    auto* addLayout = new QHBoxLayout();
    addLayout->addWidget(new QLabel("Добавить:"));
    m_bstInput = new QLineEdit();
    m_bstInput->setPlaceholderText("число");
    m_bstInput->setMaximumWidth(100);
    auto* addBtn = new QPushButton("➕ Добавить");
    connect(addBtn, &QPushButton::clicked, this, [this]() {
        bool ok;
        int val = m_bstInput->text().toInt(&ok);
        if (ok) { m_bstWidget->opAdd(val); m_bstInput->clear(); }
    });
    addLayout->addWidget(m_bstInput);
    addLayout->addWidget(addBtn);
    opsLayout->addLayout(addLayout);

    auto* removeLayout = new QHBoxLayout();
    removeLayout->addWidget(new QLabel("Удалить:"));
    auto* removeInput = new QLineEdit();
    removeInput->setPlaceholderText("число");
    removeInput->setMaximumWidth(100);
    auto* removeBtn = new QPushButton("🗑️ Удалить");
    removeBtn->setObjectName("danger");
    connect(removeBtn, &QPushButton::clicked, this, [this, removeInput]() {
        bool ok;
        int val = removeInput->text().toInt(&ok);
        if (ok) { m_bstWidget->opRemove(val); removeInput->clear(); }
    });
    removeLayout->addWidget(removeInput);
    removeLayout->addWidget(removeBtn);
    opsLayout->addLayout(removeLayout);

    auto* findLayout = new QHBoxLayout();
    findLayout->addWidget(new QLabel("Найти:"));
    auto* findInput = new QLineEdit();
    findInput->setPlaceholderText("число");
    findInput->setMaximumWidth(100);
    auto* findBtn = new QPushButton("🔍 Найти");
    connect(findBtn, &QPushButton::clicked, this, [this, findInput]() {
        bool ok;
        int val = findInput->text().toInt(&ok);
        if (ok) { m_bstWidget->opFind(val); findInput->clear(); }
    });
    findLayout->addWidget(findInput);
    findLayout->addWidget(findBtn);
    opsLayout->addLayout(findLayout);

    opsLayout->addStretch();
    ctrlLayout->addWidget(opsGroup);

    // Группа обходов
    auto* traverseGroup = new QGroupBox("Обходы дерева");
    auto* travLayout = new QVBoxLayout(traverseGroup);
    travLayout->setSpacing(8);

    auto* inorderBtn = new QPushButton("🔄 Inorder (L-N-R)");
    connect(inorderBtn, &QPushButton::clicked, this, [this]() {
        m_bstWidget->opTraverse(BSTVisWidget::TraverseType::Inorder);
    });
    travLayout->addWidget(inorderBtn);

    auto* preorderBtn = new QPushButton("🔄 Preorder (N-L-R)");
    connect(preorderBtn, &QPushButton::clicked, this, [this]() {
        m_bstWidget->opTraverse(BSTVisWidget::TraverseType::Preorder);
    });
    travLayout->addWidget(preorderBtn);

    auto* postorderBtn = new QPushButton("🔄 Postorder (L-R-N)");
    connect(postorderBtn, &QPushButton::clicked, this, [this]() {
        m_bstWidget->opTraverse(BSTVisWidget::TraverseType::Postorder);
    });
    travLayout->addWidget(postorderBtn);

    auto* levelorderBtn = new QPushButton("🔄 Level-order (BFS)");
    connect(levelorderBtn, &QPushButton::clicked, this, [this]() {
        m_bstWidget->opTraverse(BSTVisWidget::TraverseType::LevelOrder);
    });
    travLayout->addWidget(levelorderBtn);

    travLayout->addStretch();
    ctrlLayout->addWidget(traverseGroup);

    // Кнопка очистки
    auto* clearBtn = new QPushButton("🗑️ Очистить дерево");
    clearBtn->setObjectName("danger");
    connect(clearBtn, &QPushButton::clicked, this, [this]() {
        m_bstWidget->clearTree();
    });
    ctrlLayout->addWidget(clearBtn);

    // Лог
    auto* logGroup = new QGroupBox("Журнал событий");
    auto* logLayout = new QVBoxLayout(logGroup);
    m_bstLog = new QTextEdit();
    m_bstLog->setReadOnly(true);
    m_bstLog->setMaximumHeight(200);
    logLayout->addWidget(m_bstLog);
    ctrlLayout->addWidget(logGroup);

    layout->addWidget(controlPanel, 1);
    m_tabs->addTab(tab, "🌳 Дерево поиска (BST)");
}

// ════════════════════════════════════════════════════════════════
void MainWindow::setupArrayHeapTab() {
    auto* tab = new QWidget();
    auto* layout = new QHBoxLayout(tab);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(12);

    m_arrayHeapWidget = new ArrayHeapVisWidget(tab);
    m_arrayHeapWidget->setMinimumSize(600, 500);
    connect(m_arrayHeapWidget, &ArrayHeapVisWidget::logMessage, this, [this](const QString& msg) {
        logTo(m_arrayHeapLog, msg);
    });
    layout->addWidget(m_arrayHeapWidget, 3);

    auto* controlPanel = new QWidget();
    auto* ctrlLayout = new QVBoxLayout(controlPanel);
    ctrlLayout->setContentsMargins(0, 0, 0, 0);
    ctrlLayout->setSpacing(10);

    auto* opsGroup = new QGroupBox("Операции с кучей (массив)");
    auto* opsLayout = new QVBoxLayout(opsGroup);
    opsLayout->setSpacing(8);

    auto* pushLayout = new QHBoxLayout();
    pushLayout->addWidget(new QLabel("Push:"));
    m_arrayHeapInput = new QLineEdit();
    m_arrayHeapInput->setPlaceholderText("число");
    m_arrayHeapInput->setMaximumWidth(100);
    auto* pushBtn = new QPushButton("➕ Push");
    connect(pushBtn, &QPushButton::clicked, this, [this]() {
        bool ok;
        int val = m_arrayHeapInput->text().toInt(&ok);
        if (ok) { m_arrayHeapWidget->opPush(val); m_arrayHeapInput->clear(); }
    });
    pushLayout->addWidget(m_arrayHeapInput);
    pushLayout->addWidget(pushBtn);
    opsLayout->addLayout(pushLayout);

    auto* popBtn = new QPushButton("🗑️ Pop");
    popBtn->setObjectName("danger");
    connect(popBtn, &QPushButton::clicked, this, [this]() {
        m_arrayHeapWidget->opPop();
    });
    opsLayout->addWidget(popBtn);

    auto* clearBtn = new QPushButton("🗑️ Очистить кучу");
    clearBtn->setObjectName("danger");
    connect(clearBtn, &QPushButton::clicked, this, [this]() {
        m_arrayHeapWidget->opClear();
    });
    opsLayout->addWidget(clearBtn);

    opsLayout->addStretch();
    ctrlLayout->addWidget(opsGroup);

    auto* logGroup = new QGroupBox("Журнал");
    auto* logLayout = new QVBoxLayout(logGroup);
    m_arrayHeapLog = new QTextEdit();
    m_arrayHeapLog->setReadOnly(true);
    logLayout->addWidget(m_arrayHeapLog);
    ctrlLayout->addWidget(logGroup);

    layout->addWidget(controlPanel, 1);
    m_tabs->addTab(tab, "📊 Куча (Array)");
}

// ════════════════════════════════════════════════════════════════
void MainWindow::setupListHeapTab() {
    auto* tab = new QWidget();
    auto* layout = new QHBoxLayout(tab);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(12);

    m_listHeapWidget = new ListHeapVisWidget(tab);
    m_listHeapWidget->setMinimumSize(600, 500);
    connect(m_listHeapWidget, &ListHeapVisWidget::logMessage, this, [this](const QString& msg) {
        logTo(m_listHeapLog, msg);
    });
    layout->addWidget(m_listHeapWidget, 3);

    auto* controlPanel = new QWidget();
    auto* ctrlLayout = new QVBoxLayout(controlPanel);
    ctrlLayout->setContentsMargins(0, 0, 0, 0);
    ctrlLayout->setSpacing(10);

    auto* opsGroup = new QGroupBox("Операции с кучей (список)");
    auto* opsLayout = new QVBoxLayout(opsGroup);
    opsLayout->setSpacing(8);

    auto* pushLayout = new QHBoxLayout();
    pushLayout->addWidget(new QLabel("Push:"));
    m_listHeapInput = new QLineEdit();
    m_listHeapInput->setPlaceholderText("число");
    m_listHeapInput->setMaximumWidth(100);
    auto* pushBtn = new QPushButton("➕ Push");
    connect(pushBtn, &QPushButton::clicked, this, [this]() {
        bool ok;
        int val = m_listHeapInput->text().toInt(&ok);
        if (ok) { m_listHeapWidget->opPush(val); m_listHeapInput->clear(); }
    });
    pushLayout->addWidget(m_listHeapInput);
    pushLayout->addWidget(pushBtn);
    opsLayout->addLayout(pushLayout);

    auto* popBtn = new QPushButton("🗑️ Pop");
    popBtn->setObjectName("danger");
    connect(popBtn, &QPushButton::clicked, this, [this]() {
        m_listHeapWidget->opPop();
    });
    opsLayout->addWidget(popBtn);

    auto* clearBtn = new QPushButton("🗑️ Очистить кучу");
    clearBtn->setObjectName("danger");
    connect(clearBtn, &QPushButton::clicked, this, [this]() {
        m_listHeapWidget->opClear();
    });
    opsLayout->addWidget(clearBtn);

    opsLayout->addStretch();
    ctrlLayout->addWidget(opsGroup);

    auto* logGroup = new QGroupBox("Журнал");
    auto* logLayout = new QVBoxLayout(logGroup);
    m_listHeapLog = new QTextEdit();
    m_listHeapLog->setReadOnly(true);
    logLayout->addWidget(m_listHeapLog);
    ctrlLayout->addWidget(logGroup);

    layout->addWidget(controlPanel, 1);
    m_tabs->addTab(tab, "🔗 Куча (Linked)");
}

// ════════════════════════════════════════════════════════════════
void MainWindow::setupHashTab() {
    auto* tab = new QWidget();
    auto* layout = new QHBoxLayout(tab);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(12);

    m_hashWidget = new HashTableVisWidget(tab);
    m_hashWidget->setMinimumSize(600, 500);
    connect(m_hashWidget, &HashTableVisWidget::logMessage, this, [this](const QString& msg) {
        logTo(m_hashLog, msg);
    });
    layout->addWidget(m_hashWidget, 3);

    auto* controlPanel = new QWidget();
    auto* ctrlLayout = new QVBoxLayout(controlPanel);
    ctrlLayout->setContentsMargins(0, 0, 0, 0);
    ctrlLayout->setSpacing(10);

    auto* opsGroup = new QGroupBox("Операции с хэш-таблицей");
    auto* opsLayout = new QVBoxLayout(opsGroup);
    opsLayout->setSpacing(8);

    auto* insertLayout = new QHBoxLayout();
    insertLayout->addWidget(new QLabel("Key:"));
    m_hashKeyInput = new QLineEdit();
    m_hashKeyInput->setPlaceholderText("ключ");
    m_hashKeyInput->setMaximumWidth(80);
    insertLayout->addWidget(m_hashKeyInput);
    insertLayout->addWidget(new QLabel("Value:"));
    m_hashValInput = new QLineEdit();
    m_hashValInput->setPlaceholderText("знач.");
    m_hashValInput->setMaximumWidth(80);
    insertLayout->addWidget(m_hashValInput);
    auto* insertBtn = new QPushButton("➕ Insert");
    connect(insertBtn, &QPushButton::clicked, this, [this]() {
        bool ok1, ok2;
        int key = m_hashKeyInput->text().toInt(&ok1);
        int val = m_hashValInput->text().toInt(&ok2);
        if (ok1 && ok2) {
            m_hashWidget->opInsert(key, val);
            m_hashKeyInput->clear();
            m_hashValInput->clear();
        }
    });
    insertLayout->addWidget(insertBtn);
    opsLayout->addLayout(insertLayout);

    auto* findLayout = new QHBoxLayout();
    findLayout->addWidget(new QLabel("Find Key:"));
    auto* findInput = new QLineEdit();
    findInput->setPlaceholderText("ключ");
    findInput->setMaximumWidth(100);
    auto* findBtn = new QPushButton("🔍 Найти");
    connect(findBtn, &QPushButton::clicked, this, [this, findInput]() {
        bool ok;
        int key = findInput->text().toInt(&ok);
        if (ok) { m_hashWidget->opFind(key); findInput->clear(); }
    });
    findLayout->addWidget(findInput);
    findLayout->addWidget(findBtn);
    opsLayout->addLayout(findLayout);

    auto* removeLayout = new QHBoxLayout();
    removeLayout->addWidget(new QLabel("Remove:"));
    auto* removeInput = new QLineEdit();
    removeInput->setPlaceholderText("ключ");
    removeInput->setMaximumWidth(100);
    auto* removeBtn = new QPushButton("🗑️ Удалить");
    removeBtn->setObjectName("danger");
    connect(removeBtn, &QPushButton::clicked, this, [this, removeInput]() {
        bool ok;
        int key = removeInput->text().toInt(&ok);
        if (ok) { m_hashWidget->opRemove(key); removeInput->clear(); }
    });
    removeLayout->addWidget(removeInput);
    removeLayout->addWidget(removeBtn);
    opsLayout->addLayout(removeLayout);

    auto* clearBtn = new QPushButton("🗑️ Очистить таблицу");
    clearBtn->setObjectName("danger");
    connect(clearBtn, &QPushButton::clicked, this, [this]() {
        m_hashWidget->opClear();
    });
    opsLayout->addWidget(clearBtn);

    opsLayout->addStretch();
    ctrlLayout->addWidget(opsGroup);

    auto* logGroup = new QGroupBox("Журнал");
    auto* logLayout = new QVBoxLayout(logGroup);
    m_hashLog = new QTextEdit();
    m_hashLog->setReadOnly(true);
    logLayout->addWidget(m_hashLog);
    ctrlLayout->addWidget(logGroup);

    layout->addWidget(controlPanel, 1);
    m_tabs->addTab(tab, "🗂️ Хэш-таблица");
}
