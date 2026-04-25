// Исправления для Qt 6

// В createTask4Widget() - исправляем addTab
mainTabs->addTab(createTask1Widget(), "🔢 Dec → Bin");
mainTabs->addTab(createTask2Widget(), "∞ Аккерман");
mainTabs->addTab(createTask3Widget(), "↻ Реверс");
mainTabs->addTab(createTask4Widget(), "🏛️ Ханой");
mainTabs->addTab(createTask5Widget(), "📁 Файлы");

// В setupHanoiScene() - исправляем QChar
auto *label = new QGraphicsTextItem(QString(QChar('A' + i)));

// В buildTree() и runTask5() - используем setBackground вместо setTextColor
QPalette palette = item->data(0, Qt::ForegroundRole).value<QPalette>();
item->setData(0, Qt::ForegroundRole, QColor(ModernTheme::ACCENT_SECOND));
