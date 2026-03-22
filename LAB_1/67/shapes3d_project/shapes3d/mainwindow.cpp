#include <QRandomGenerator>
#include "mainwindow.h"
#include "circle_shape.h"
#include "triangle_shape.h"
#include "rectangle_shape.h"
#include "square_shape.h"
#include "rhombus_shape.h"
#include "star_shape.h"
#include "hexagon_shape.h"
#include "custom_shape.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QComboBox>
#include <QListWidget>
#include <QTabWidget>
#include <QScrollArea>
#include <QColorDialog>
#include <QMessageBox>
#include <QCheckBox>
#include <QFont>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("3D Shapes – Лабораторная работа №1");
    setMinimumSize(1100, 700);
    setupUi();

    // Добавим несколько фигур по умолчанию для демонстрации
    auto* tri = new TriangleShape(80, this);
    tri->setCenterOfMass(-150, 0, 0);
    m_ownedShapes.append(tri);
    m_renderer->addShape(tri);
    addShapeToList(tri);

    auto* rect = new RectangleShape(100, 60, this);
    rect->setCenterOfMass(0, 0, 0);
    m_ownedShapes.append(rect);
    m_renderer->addShape(rect);
    addShapeToList(rect);

    auto* star = new StarShape(StarShape::Star5, 80, 35, this);
    star->setCenterOfMass(160, 0, 0);
    m_ownedShapes.append(star);
    m_renderer->addShape(star);
    addShapeToList(star);

    m_shapeList->setCurrentRow(0);
}

// ── Построение UI ────────────────────────────────────────────────────────────

void MainWindow::setupUi()
{
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout* mainLayout = new QHBoxLayout(central);
    mainLayout->setSpacing(8);

    // ── Левая панель ─────────────────────────────────────────────────────────
    QWidget* leftPanel = new QWidget;
    leftPanel->setFixedWidth(280);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setSpacing(6);

    // Список фигур
    QGroupBox* listBox = new QGroupBox("Фигуры");
    QVBoxLayout* listLayout = new QVBoxLayout(listBox);
    m_shapeList = new QListWidget;
    m_shapeList->setStyleSheet(
        "QListWidget { background:#1e2030; color:#cdd6f4; border:1px solid #444; }"
        "QListWidget::item:selected { background:#3949ab; }");
    listLayout->addWidget(m_shapeList);

    // Добавление новой фигуры
    QHBoxLayout* addRow = new QHBoxLayout;
    m_shapeTypeCombo = new QComboBox;
    m_shapeTypeCombo->addItems({
        "Circle", "Triangle", "Rectangle", "Square",
        "Rhombus", "Star5", "Star6", "Star8",
        "Hexagon", "Arrow"
    });
    m_addBtn = new QPushButton("Добавить");
    m_removeBtn = new QPushButton("Удалить");
    m_addBtn->setFixedWidth(80);
    m_removeBtn->setFixedWidth(70);
    addRow->addWidget(m_shapeTypeCombo);
    addRow->addWidget(m_addBtn);
    addRow->addWidget(m_removeBtn);
    listLayout->addLayout(addRow);
    leftLayout->addWidget(listBox);

    // Информация о фигуре
    QGroupBox* infoBox = new QGroupBox("Информация");
    QVBoxLayout* infoLayout = new QVBoxLayout(infoBox);
    m_infoLabel = new QLabel("—");
    m_infoLabel->setWordWrap(true);
    m_infoLabel->setStyleSheet("color:#a6e3a1; font-family:monospace;");
    infoLayout->addWidget(m_infoLabel);
    leftLayout->addWidget(infoBox);

    // Параметры фигуры
    m_paramBox = new QGroupBox("Параметры фигуры");
    QVBoxLayout* paramLayout = new QVBoxLayout(m_paramBox);

    // Extrusion
    QHBoxLayout* extRow = new QHBoxLayout;
    extRow->addWidget(new QLabel("Толщина 3D:"));
    m_extrusionSpin = new QDoubleSpinBox;
    m_extrusionSpin->setRange(1, 200);
    m_extrusionSpin->setValue(20);
    extRow->addWidget(m_extrusionSpin);
    paramLayout->addLayout(extRow);

    // Color button
    m_colorBtn = new QPushButton("Цвет фигуры...");
    m_currentColor = Qt::blue;
    paramLayout->addWidget(m_colorBtn);

    // Param spins (up to 4)
    for (int i = 0; i < 4; ++i) {
        QHBoxLayout* row = new QHBoxLayout;
        QLabel* lbl = new QLabel("param:");
        QDoubleSpinBox* sp = new QDoubleSpinBox;
        sp->setRange(1, 500);
        sp->setValue(50);
        row->addWidget(lbl);
        row->addWidget(sp);
        paramLayout->addLayout(row);
        m_paramLabels.append(lbl);
        m_paramSpins.append(sp);
        lbl->hide(); sp->hide();
    }

    QPushButton* applyBtn = new QPushButton("Применить");
    connect(applyBtn, &QPushButton::clicked, this, &MainWindow::onApplyParams);
    paramLayout->addWidget(applyBtn);
    leftLayout->addWidget(m_paramBox);
    leftLayout->addStretch();

    // ── Правая панель (рендерер + управление) ───────────────────────────────
    QWidget* rightPanel = new QWidget;
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setSpacing(6);

    m_renderer = new Renderer3D;
    rightLayout->addWidget(m_renderer, 1);

    // Панель трансформаций
    QGroupBox* transBox = new QGroupBox("Трансформации");
    QGridLayout* transGrid = new QGridLayout(transBox);

    auto makeSpin = [](double min, double max, double val, int dec = 1) {
        auto* s = new QDoubleSpinBox;
        s->setRange(min, max);
        s->setValue(val);
        s->setDecimals(dec);
        s->setFixedWidth(70);
        return s;
    };

    // Перемещение
    transGrid->addWidget(new QLabel("Сдвиг X/Y/Z:"), 0, 0);
    m_txSpin = makeSpin(-500, 500, 50);
    m_tySpin = makeSpin(-500, 500, 0);
    m_tzSpin = makeSpin(-500, 500, 0);
    transGrid->addWidget(m_txSpin, 0, 1);
    transGrid->addWidget(m_tySpin, 0, 2);
    transGrid->addWidget(m_tzSpin, 0, 3);

    // Поворот
    transGrid->addWidget(new QLabel("Угол / Pivot X/Y:"), 1, 0);
    m_angleSpin = makeSpin(-720, 720, 45);
    m_pvxSpin   = makeSpin(-500, 500, 0);
    m_pvySpin   = makeSpin(-500, 500, 0);
    transGrid->addWidget(m_angleSpin, 1, 1);
    transGrid->addWidget(m_pvxSpin,   1, 2);
    transGrid->addWidget(m_pvySpin,   1, 3);

    // Масштаб
    transGrid->addWidget(new QLabel("Масштаб:"), 2, 0);
    m_scaleSpin = makeSpin(0.1, 10, 1.5, 2);
    transGrid->addWidget(m_scaleSpin, 2, 1);

    // Анимация
    m_animCheck = new QCheckBox("Анимировать");
    m_animCheck->setChecked(true);
    transGrid->addWidget(m_animCheck, 2, 2);
    transGrid->addWidget(new QLabel("Время (мс):"), 2, 3);
    m_animDurSpin = new QSpinBox;
    m_animDurSpin->setRange(100, 5000);
    m_animDurSpin->setValue(1000);
    m_animDurSpin->setFixedWidth(70);
    transGrid->addWidget(m_animDurSpin, 2, 4);

    // Кнопки трансформаций
    QPushButton* btnMove  = new QPushButton("Переместить");
    QPushButton* btnRot   = new QPushButton("Повернуть");
    QPushButton* btnScale = new QPushButton("Масштаб");
    transGrid->addWidget(btnMove,  3, 0, 1, 2);
    transGrid->addWidget(btnRot,   3, 2, 1, 1);
    transGrid->addWidget(btnScale, 3, 3, 1, 2);
    rightLayout->addWidget(transBox);

    // ── Стили ─────────────────────────────────────────────────────────────
    central->setStyleSheet(
        "QWidget { background:#1a1b26; color:#c0caf5; }"
        "QGroupBox { border:1px solid #3d4159; border-radius:6px; margin-top:10px; padding:6px; }"
        "QGroupBox::title { color:#7aa2f7; }"
        "QPushButton { background:#3d4159; border:none; border-radius:4px;"
        "              padding:4px 10px; color:#c0caf5; }"
        "QPushButton:hover { background:#565f89; }"
        "QPushButton:pressed { background:#7aa2f7; color:#1a1b26; }"
        "QDoubleSpinBox, QSpinBox, QComboBox {"
        "  background:#24283b; border:1px solid #3d4159; border-radius:4px;"
        "  color:#c0caf5; padding:2px; }"
        "QCheckBox { color:#c0caf5; }"
        "QLabel { color:#a9b1d6; }"
    );

    // ── Сборка ────────────────────────────────────────────────────────────
    mainLayout->addWidget(leftPanel);
    mainLayout->addWidget(rightPanel, 1);

    // ── Сигналы ───────────────────────────────────────────────────────────
    connect(m_addBtn,    &QPushButton::clicked, this, &MainWindow::onAddShape);
    connect(m_removeBtn, &QPushButton::clicked, this, &MainWindow::onRemoveShape);
    connect(m_shapeList, &QListWidget::currentRowChanged,
            this, &MainWindow::onShapeSelected);
    connect(btnMove,  &QPushButton::clicked, this, &MainWindow::onTranslate);
    connect(btnRot,   &QPushButton::clicked, this, &MainWindow::onRotate);
    connect(btnScale, &QPushButton::clicked, this, &MainWindow::onScale);
    connect(m_colorBtn, &QPushButton::clicked, this, [this]() {
        Shape* s = m_renderer->selectedShape();
        if (!s) return;
        QColor c = QColorDialog::getColor(s->color(), this, "Выберите цвет");
        if (c.isValid()) {
            s->setColor(c);
            m_currentColor = c;
        }
    });
}

// ── Управление фигурами ──────────────────────────────────────────────────────

void MainWindow::addShapeToList(Shape* shape)
{
    QString label = QString("%1. %2 [%.0f,%.0f,%.0f]")
        .arg(m_shapeList->count()+1)
        .arg(shape->typeName())
        .arg(shape->centerOfMass().x())
        .arg(shape->centerOfMass().y())
        .arg(shape->centerOfMass().z());
    m_shapeList->addItem(label);
    connect(shape, &Shape::changed, this, &MainWindow::updateInfoPanel);
}

Shape* MainWindow::createShape(const QString& type)
{
    Shape* s = nullptr;
    // Смещаем новые фигуры случайно
    float ox = (QRandomGenerator::global()->bounded(300)) - 150;
    float oy = (QRandomGenerator::global()->bounded(200)) - 100;

    if      (type == "Circle")    s = new CircleShape(60, this);
    else if (type == "Triangle")  s = new TriangleShape(90, this);
    else if (type == "Rectangle") s = new RectangleShape(110, 70, this);
    else if (type == "Square")    s = new SquareShape(75, this);
    else if (type == "Rhombus")   s = new RhombusShape(120, 80, this);
    else if (type == "Star5")     s = new StarShape(StarShape::Star5, 75, 32, this);
    else if (type == "Star6")     s = new StarShape(StarShape::Star6, 75, 45, this);
    else if (type == "Star8")     s = new StarShape(StarShape::Star8, 70, 38, this);
    else if (type == "Hexagon")   s = new HexagonShape(70, this);
    else if (type == "Arrow")     s = new ArrowShape(120, 25, 60, 0.4f, this);

    if (s) s->setCenterOfMass(ox, oy, 0);
    return s;
}

void MainWindow::onAddShape()
{
    QString type = m_shapeTypeCombo->currentText();
    Shape* s = createShape(type);
    if (!s) return;
    m_ownedShapes.append(s);
    m_renderer->addShape(s);
    addShapeToList(s);
    m_shapeList->setCurrentRow(m_shapeList->count() - 1);
}

void MainWindow::onRemoveShape()
{
    int row = m_shapeList->currentRow();
    if (row < 0 || row >= m_ownedShapes.size()) return;

    Shape* s = m_ownedShapes[row];
    m_renderer->removeShape(s);
    m_ownedShapes.remove(row);
    delete s;

    QListWidgetItem* item = m_shapeList->takeItem(row);
    delete item;

    // Обновить нумерацию
    for (int i = 0; i < m_shapeList->count(); ++i) {
        Shape* sh = m_ownedShapes[i];
        m_shapeList->item(i)->setText(
            QString("%1. %2").arg(i+1).arg(sh->typeName()));
    }
    m_renderer->setSelectedIndex(m_shapeList->currentRow());
}

void MainWindow::onShapeSelected(int row)
{
    m_renderer->setSelectedIndex(row);
    if (row < 0 || row >= m_ownedShapes.size()) {
        m_infoLabel->setText("—");
        populateParamPanel(nullptr);
        return;
    }
    Shape* s = m_ownedShapes[row];
    populateParamPanel(s);
    updateInfoPanel();
}

// ── Информационная панель ────────────────────────────────────────────────────

void MainWindow::updateInfoPanel()
{
    int row = m_shapeList->currentRow();
    if (row < 0 || row >= m_ownedShapes.size()) return;
    Shape* s = m_ownedShapes[row];
    QVector3D c = s->centerOfMass();
    m_infoLabel->setText(
        QString("Тип: <b>%1</b><br>"
                "Площадь: <b>%.2f</b><br>"
                "Периметр: <b>%.2f</b><br>"
                "Центр: <b>(%.1f, %.1f, %.1f)</b><br>"
                "Поворот: <b>%.1f°</b><br>"
                "Масштаб: <b>%.2f</b>")
        .arg(s->typeName())
        .arg(s->area())
        .arg(s->perimeter())
        .arg(c.x()).arg(c.y()).arg(c.z())
        .arg(s->rotation2D())
        .arg(s->scale2D())
    );
    // Обновить заголовок в списке
    if (row < m_shapeList->count())
        m_shapeList->item(row)->setText(
            QString("%1. %2 [%.0f,%.0f]")
            .arg(row+1).arg(s->typeName()).arg(c.x()).arg(c.y()));
}

// ── Параметры фигуры ─────────────────────────────────────────────────────────

void MainWindow::populateParamPanel(Shape* shape)
{
    for (auto* l : m_paramLabels) l->hide();
    for (auto* s : m_paramSpins)  s->hide();
    if (!shape) return;

    m_extrusionSpin->setValue(shape->extrusionDepth());
    m_currentColor = shape->color();

    auto showParam = [&](int idx, const QString& name, double val, double mn=1, double mx=500) {
        m_paramLabels[idx]->setText(name + ":");
        m_paramSpins[idx]->setRange(mn, mx);
        m_paramSpins[idx]->setValue(val);
        m_paramLabels[idx]->show();
        m_paramSpins[idx]->show();
    };

    if (auto* c = qobject_cast<CircleShape*>(shape)) {
        showParam(0, "Радиус", c->radius());
    } else if (qobject_cast<TriangleShape*>(shape)) {
        showParam(0, "Сторона (перестр.)", 80);
    } else if (auto* sq = qobject_cast<SquareShape*>(shape)) {
        showParam(0, "Сторона", sq->side());
    } else if (auto* r = qobject_cast<RectangleShape*>(shape)) {
        showParam(0, "Ширина",  r->width());
        showParam(1, "Высота",  r->height());
    } else if (auto* rh = qobject_cast<RhombusShape*>(shape)) {
        showParam(0, "Диагональ 1", rh->diagonal1());
        showParam(1, "Диагональ 2", rh->diagonal2());
    } else if (auto* st = qobject_cast<StarShape*>(shape)) {
        showParam(0, "Внешний R", st->outerRadius());
        showParam(1, "Внутренний R", st->innerRadius());
    } else if (auto* h = qobject_cast<HexagonShape*>(shape)) {
        showParam(0, "Радиус", h->radius());
    } else if (auto* ar = qobject_cast<ArrowShape*>(shape)) {
        showParam(0, "Длина",        ar->length());
        showParam(1, "Хвост (шир.)", ar->tailWidth(), 5, 200);
        showParam(2, "Голова (шир.)", ar->headWidth(), 5, 200);
    }
}

void MainWindow::onApplyParams()
{
    int row = m_shapeList->currentRow();
    if (row < 0 || row >= m_ownedShapes.size()) return;
    Shape* shape = m_ownedShapes[row];

    shape->setExtrusionDepth(m_extrusionSpin->value());

    if (auto* c = qobject_cast<CircleShape*>(shape)) {
        c->setRadius(m_paramSpins[0]->value());
    } else if (auto* t = qobject_cast<TriangleShape*>(shape)) {
        // пересоздать как равносторонний
        TriangleShape tmp(m_paramSpins[0]->value());
        t->setVertices(tmp.vertices());
    } else if (auto* sq = qobject_cast<SquareShape*>(shape)) {
        sq->setSide(m_paramSpins[0]->value());
    } else if (auto* r = qobject_cast<RectangleShape*>(shape)) {
        r->setSize(m_paramSpins[0]->value(), m_paramSpins[1]->value());
    } else if (auto* rh = qobject_cast<RhombusShape*>(shape)) {
        rh->setDiagonals(m_paramSpins[0]->value(), m_paramSpins[1]->value());
    } else if (auto* st = qobject_cast<StarShape*>(shape)) {
        st->setOuterRadius(m_paramSpins[0]->value());
        st->setInnerRadius(m_paramSpins[1]->value());
    } else if (auto* h = qobject_cast<HexagonShape*>(shape)) {
        h->setRadius(m_paramSpins[0]->value());
    } else if (auto* ar = qobject_cast<ArrowShape*>(shape)) {
        ar->setLength(m_paramSpins[0]->value());
        ar->setTailWidth(m_paramSpins[1]->value());
        ar->setHeadWidth(m_paramSpins[2]->value());
    }
    updateInfoPanel();
}

// ── Трансформации ────────────────────────────────────────────────────────────

void MainWindow::onTranslate()
{
    Shape* s = m_renderer->selectedShape();
    if (!s) return;
    float dx = m_txSpin->value(), dy = m_tySpin->value(), dz = m_tzSpin->value();
    if (m_animCheck->isChecked()) {
        connect(s, &Shape::transformFinished, this, &MainWindow::onTransformFinished,
                Qt::UniqueConnection);
        s->animatedTranslate(dx, dy, dz, m_animDurSpin->value());
    } else {
        s->translate(dx, dy, dz);
        updateInfoPanel();
    }
}

void MainWindow::onRotate()
{
    Shape* s = m_renderer->selectedShape();
    if (!s) return;
    float angle = m_angleSpin->value();
    QVector2D pivot(m_pvxSpin->value(), m_pvySpin->value());
    if (m_animCheck->isChecked()) {
        connect(s, &Shape::transformFinished, this, &MainWindow::onTransformFinished,
                Qt::UniqueConnection);
        s->animatedRotate(angle, pivot, m_animDurSpin->value());
    } else {
        s->rotateFigure(angle, pivot);
        updateInfoPanel();
    }
}

void MainWindow::onScale()
{
    Shape* s = m_renderer->selectedShape();
    if (!s) return;
    float factor = m_scaleSpin->value();
    QVector2D pivot(m_pvxSpin->value(), m_pvySpin->value());
    if (m_animCheck->isChecked()) {
        connect(s, &Shape::transformFinished, this, &MainWindow::onTransformFinished,
                Qt::UniqueConnection);
        s->animatedScale(factor, pivot, m_animDurSpin->value());
    } else {
        s->scaleFigure(factor, pivot);
        updateInfoPanel();
    }
}

void MainWindow::onTransformFinished()
{
    updateInfoPanel();
}

void MainWindow::setControlsEnabled(bool en)
{
    m_addBtn->setEnabled(en);
    m_removeBtn->setEnabled(en);
}
