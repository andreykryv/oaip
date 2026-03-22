#pragma once
#include <QMainWindow>
#include <QListWidget>
#include <QGroupBox>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QTabWidget>
#include <QCheckBox>
#include "renderer3d.h"
#include "shape.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void onAddShape();
    void onRemoveShape();
    void onShapeSelected(int row);

    // Трансформации
    void onTranslate();
    void onRotate();
    void onScale();
    // Параметры фигуры
    void onApplyParams();

    void onTransformFinished();

    void updateInfoPanel();

private:
    void setupUi();
    void setupShapeList();
    void setupControls();
    void setupInfoPanel();

    void addShapeToList(Shape* shape);
    void populateParamPanel(Shape* shape);
    void setControlsEnabled(bool en);

    Shape* createShape(const QString& type);

    // --- UI elements ---
    Renderer3D*  m_renderer;

    // Левая панель
    QListWidget* m_shapeList;
    QComboBox*   m_shapeTypeCombo;
    QPushButton* m_addBtn;
    QPushButton* m_removeBtn;

    // Информация
    QLabel*      m_infoLabel;

    // Трансформации
    QDoubleSpinBox* m_txSpin;
    QDoubleSpinBox* m_tySpin;
    QDoubleSpinBox* m_tzSpin;
    QDoubleSpinBox* m_angleSpin;
    QDoubleSpinBox* m_pvxSpin;
    QDoubleSpinBox* m_pvySpin;
    QDoubleSpinBox* m_scaleSpin;
    QSpinBox*        m_animDurSpin;
    QCheckBox*       m_animCheck;

    // Параметры фигуры (динамически меняются)
    QGroupBox*      m_paramBox;
    QVector<QLabel*>      m_paramLabels;
    QVector<QDoubleSpinBox*> m_paramSpins;
    QDoubleSpinBox* m_extrusionSpin;
    QPushButton*    m_colorBtn;
    QColor          m_currentColor;

    QVector<Shape*> m_ownedShapes; // shapes we own
};
