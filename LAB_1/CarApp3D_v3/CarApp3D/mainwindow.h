#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>

class GlWidget;
class GlScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void buildUI();
    void updateInfo();

    GlWidget* m_gl  = nullptr;
    GlScene*  m_car = nullptr;
    QLabel*   m_lblStatus = nullptr;
    QLabel*   m_lblInfo   = nullptr;
    QColor    m_carColor  = QColor(35, 85, 200);

    QList<QPushButton*> m_doorBtns;

private slots:
    void onLoaded();
    void onMoveForward();   void onMoveBack();
    void onRotateLeft();    void onRotateRight();
    void onScaleUp();       void onScaleDown();
    void onReset();
    void onOpenDoors();     void onCloseDoors();
    void onToggleLights();  void onChooseColor();
    void onAnimStep();      void onAnimFinished();
};
