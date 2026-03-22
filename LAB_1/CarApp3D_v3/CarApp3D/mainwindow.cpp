#include "mainwindow.h"
#include "glwidget.h"
#include "glscene.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFrame>
#include <QColorDialog>
#include <QStatusBar>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle("Mercedes-Benz 190 Evo II  |  Qt OpenGL + Assimp  |  Лаб. №1");
    resize(1280, 760);
    buildUI();
}

void MainWindow::buildUI()
{
    m_gl  = new GlWidget(this);
    m_car = m_gl->scene();
    m_gl->setMinimumSize(820, 560);
    m_gl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    auto* lay = new QHBoxLayout(central);
    lay->setContentsMargins(4,4,4,4);
    lay->setSpacing(6);
    lay->addWidget(m_gl, 1);

    // Control panel
    QWidget* panel = new QWidget;
    panel->setFixedWidth(256);
    auto* pLay = new QVBoxLayout(panel);
    pLay->setSpacing(8);
    pLay->setContentsMargins(4,4,4,4);
    lay->addWidget(panel);

    auto makeGroup = [&](const QString& t) -> QVBoxLayout* {
        auto* g = new QGroupBox(t);
        auto* l = new QVBoxLayout(g);
        l->setSpacing(4);
        pLay->addWidget(g);
        return l;
    };
    auto addBtn = [&](QVBoxLayout* l, const QString& text,
                      const char* slot, const QString& css={}) -> QPushButton*
    {
        auto* b = new QPushButton(text);
        b->setMinimumHeight(34);
        if (!css.isEmpty()) b->setStyleSheet(css);
        l->addWidget(b);
        connect(b, SIGNAL(clicked()), this, slot);
        return b;
    };

    // Movement
    auto* movL = makeGroup("🚗  Перемещение");
    addBtn(movL, "▶  Вперёд",  SLOT(onMoveForward()));
    addBtn(movL, "◀  Назад",   SLOT(onMoveBack()));

    // Rotation
    auto* rotL = makeGroup("🔄  Поворот");
    addBtn(rotL, "↺  −45°",   SLOT(onRotateLeft()));
    addBtn(rotL, "↻  +45°",   SLOT(onRotateRight()));

    // Scale
    auto* scL = makeGroup("⤢  Масштаб");
    addBtn(scL, "➕  ×1.25",   SLOT(onScaleUp()));
    addBtn(scL, "➖  ×0.80",   SLOT(onScaleDown()));

    // Doors
    auto* dLay = makeGroup("🚪  Двери");
    auto* bOp = addBtn(dLay, "🔓  Открыть", SLOT(onOpenDoors()),
                       "background:#1b5c1b; color:white;");
    auto* bCl = addBtn(dLay, "🔒  Закрыть", SLOT(onCloseDoors()),
                       "background:#5c1b1b; color:white;");
    m_doorBtns << bOp << bCl;
    bOp->setEnabled(false);
    bCl->setEnabled(false);

    // Headlights
    auto* lLay = makeGroup("💡  Фары");
    addBtn(lLay, "🔆  Вкл / Выкл", SLOT(onToggleLights()),
           "background:#3a3a08; color:white;");

    // Color
    auto* cLay = makeGroup("🎨  Цвет кузова");
    addBtn(cLay, "🎨  Выбрать...", SLOT(onChooseColor()));

    // Reset
    auto* rLay = makeGroup("↩  Сброс");
    addBtn(rLay, "↩  Сбросить", SLOT(onReset()),
           "background:#1a1a50; color:white;");

    // Info
    m_lblInfo = new QLabel("⏳ Загрузка модели GLB через Assimp...");
    m_lblInfo->setWordWrap(true);
    m_lblInfo->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_lblInfo->setAlignment(Qt::AlignTop);
    m_lblInfo->setMinimumHeight(150);
    m_lblInfo->setStyleSheet("font-size:11px; padding:5px; color:#aaffcc;");
    pLay->addWidget(m_lblInfo);

    auto* hint = new QLabel(
        "<small><b>Камера:</b><br>ЛКМ — вращение<br>"
        "ПКМ / колесо — зум</small>");
    hint->setWordWrap(true);
    hint->setStyleSheet("color:#888; font-size:10px;");
    pLay->addWidget(hint);
    pLay->addStretch();

    m_lblStatus = new QLabel("⏳ Загрузка...");
    statusBar()->addWidget(m_lblStatus, 1);

    connect(m_gl,  &GlWidget::modelLoaded,        this, &MainWindow::onLoaded);
    connect(m_car, &GlScene::animStep,             this, &MainWindow::onAnimStep);
    connect(m_car, &GlScene::animFinished,         this, &MainWindow::onAnimFinished);
    connect(m_car, &GlScene::doorAnimStep, [this](float a){
        m_lblStatus->setText(QString("🚪 Двери: %1°").arg(int(a)));
        updateInfo();
        m_gl->update();
    });
    connect(m_car, &GlScene::doorAnimDone, [this]{
        m_lblStatus->setText("Готов");
        updateInfo();
    });
}

void MainWindow::updateInfo()
{
    if (!m_car) return;
    auto p = m_car->position();
    m_lblInfo->setText(
        QString("<b>Модель:</b> %1<br>"
                "<b>Площадь:</b> %2 м²<br>"
                "<b>Периметр:</b> %3 м<br>"
                "<b>Позиция:</b> (%4, %5, %6)<br>"
                "<b>Масштаб:</b> %7×<br>"
                "<b>Угол дверей:</b> %8°<br>"
                "<b>Фары:</b> %9<br>"
                "<b>Статус:</b> %10")
        .arg(m_car->typeName())
        .arg(QString::number(m_car->area(),      'f', 2))
        .arg(QString::number(m_car->perimeter(), 'f', 2))
        .arg(QString::number(p.x(),'f',2))
        .arg(QString::number(p.y(),'f',2))
        .arg(QString::number(p.z(),'f',2))
        .arg(QString::number(m_car->scaleValue(),'f',2))
        .arg(int(m_car->doorAngle()))
        .arg(m_car->headlightsOn() ? "ВКЛ 💡" : "выкл")
        .arg(m_car->isLoaded()
             ? "<font color='#00ff88'>Загружена ✓</font>"
             : "<font color='#ffaa00'>Загрузка…</font>")
    );
}

void MainWindow::onLoaded()
{
    m_lblStatus->setText("✓ Mercedes-Benz 190 Evo II загружен");
    for (auto* b : m_doorBtns) b->setEnabled(true);
    updateInfo();
    m_gl->update();
}
void MainWindow::onMoveForward()
{
    if (m_car->isAnimating()) return;
    m_car->animMoveTo(m_car->position()+QVector3D(0,0,-4.5f),1800);
    m_lblStatus->setText("▶ Движение вперёд…");
}
void MainWindow::onMoveBack()
{
    if (m_car->isAnimating()) return;
    m_car->animMoveTo(m_car->position()+QVector3D(0,0,4.5f),1800);
    m_lblStatus->setText("◀ Движение назад…");
}
void MainWindow::onRotateLeft()
{
    if (m_car->isAnimating()) return;
    m_car->animRotate(45.0f,{0,1,0},1500);
    m_lblStatus->setText("↺ Поворот влево…");
}
void MainWindow::onRotateRight()
{
    if (m_car->isAnimating()) return;
    m_car->animRotate(-45.0f,{0,1,0},1500);
    m_lblStatus->setText("↻ Поворот вправо…");
}
void MainWindow::onScaleUp()
{
    if (m_car->isAnimating()) return;
    m_car->animScaleTo(m_car->scaleValue()*1.25f,1000);
    m_lblStatus->setText("➕ Увеличение…");
}
void MainWindow::onScaleDown()
{
    if (m_car->isAnimating()) return;
    m_car->animScaleTo(m_car->scaleValue()*0.80f,1000);
    m_lblStatus->setText("➖ Уменьшение…");
}
void MainWindow::onOpenDoors()  { m_car->openDoors(1400);  m_lblStatus->setText("🚪 Открываю…"); }
void MainWindow::onCloseDoors() { m_car->closeDoors(1400); m_lblStatus->setText("🚪 Закрываю…"); }
void MainWindow::onToggleLights()
{
    m_car->toggleHeadlights();
    m_lblStatus->setText(m_car->headlightsOn() ? "💡 Фары включены" : "Фары выключены");
    updateInfo(); m_gl->update();
}
void MainWindow::onChooseColor()
{
    QColor c = QColorDialog::getColor(m_carColor, this, "Цвет кузова");
    if (!c.isValid()) return;
    m_carColor = c;
    m_car->setBodyColor(c);
    m_gl->update();
}
void MainWindow::onReset()
{
    m_car->resetTransform();
    m_lblStatus->setText("↩ Сброшено");
    updateInfo(); m_gl->update();
}
void MainWindow::onAnimStep()    { updateInfo(); m_gl->update(); }
void MainWindow::onAnimFinished(){ m_lblStatus->setText("Готов"); updateInfo(); }
