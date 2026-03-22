#pragma once

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPoint>

class GlScene;

class GlWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit GlWidget(QWidget* parent = nullptr);
    ~GlWidget() override;

    GlScene* scene() const { return m_scene; }
    bool     loaded()const { return m_loaded; }

signals:
    void modelLoaded();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL()     override;

    void mousePressEvent(QMouseEvent*)  override;
    void mouseMoveEvent (QMouseEvent*)  override;
    void mouseReleaseEvent(QMouseEvent*)override;
    void wheelEvent     (QWheelEvent*)  override;

private:
    void setupLighting();
    void drawGrid();

    GlScene* m_scene  = nullptr;
    bool     m_loaded = false;

    // Orbit camera
    float  m_azimuth   =  25.0f;
    float  m_elevation =  18.0f;
    float  m_distance  =  10.0f;
    float  m_pivotY    =   0.3f;
    QPoint m_lastPos;
    bool   m_dragging  = false;
};
