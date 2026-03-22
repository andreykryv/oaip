#include "glwidget.h"
#include "glscene.h"

#ifdef Q_OS_MAC
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

#include <QSurfaceFormat>
#include <QFile>
#include <QStandardPaths>
#include <QCoreApplication>
#include <cmath>

GlWidget::GlWidget(QWidget* parent) : QOpenGLWidget(parent)
{
    QSurfaceFormat fmt;
    fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
    fmt.setVersion(2, 1);
    fmt.setDepthBufferSize(24);
    fmt.setSamples(4);
    setFormat(fmt);
    setMouseTracking(true);

    m_scene = new GlScene(this);
    connect(m_scene, &GlScene::animStep, this, QOverload<>::of(&GlWidget::update));
    connect(m_scene, &GlScene::doorAnimStep, [this](float){ update(); });
}

GlWidget::~GlWidget()
{
    makeCurrent();
    delete m_scene;
    doneCurrent();
}

void GlWidget::initializeGL()
{
    glClearColor(0.10f, 0.11f, 0.18f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
glDepthMask(GL_TRUE); 
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    setupLighting();

    // ── Load GLB (GL context is active here) ─────────────────
    // Extract from QRC to temp file
    QString tmpPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation)
                      + "/mercedes190evo2.glb";

    if (!QFile::exists(tmpPath)) {
        QFile src(":/car.glb");
        if (src.open(QFile::ReadOnly)) {
            QFile dst(tmpPath);
            if (dst.open(QFile::WriteOnly)) {
                dst.write(src.readAll());
                dst.close();
            }
            src.close();
        }
    }

    if (m_scene->loadGlb(tmpPath)) {
        m_loaded = true;
        emit modelLoaded();
    }

    update();
}

void GlWidget::setupLighting()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    // Light 0 – sun (top-right-front)
    GLfloat p0[] = { 4.0f, 8.0f, 5.0f, 0.0f };  // directional
    GLfloat d0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat a0[] = { 0.20f, 0.20f, 0.22f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, p0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  d0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, d0);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  a0);

    // Light 1 – fill (bottom-left)
    GLfloat p1[] = {-3.0f, 2.0f, -4.0f, 0.0f };
    GLfloat d1[] = { 0.35f, 0.35f, 0.45f, 1.0f };
    GLfloat a1[] = { 0.05f, 0.05f, 0.08f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, p1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  d1);
    glLightfv(GL_LIGHT1, GL_AMBIENT,  a1);
}

void GlWidget::resizeGL(int w, int h)
{
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w/h, 0.05, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

void GlWidget::drawGrid()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    // Dark asphalt plane
    float y = -0.10f;
    glColor3f(0.18f, 0.19f, 0.22f);
    glBegin(GL_QUADS);
        glVertex3f(-25,y,-25); glVertex3f(25,y,-25);
        glVertex3f(25,y,25);   glVertex3f(-25,y,25);
    glEnd();

    // Grid lines
    glColor3f(0.28f, 0.30f, 0.34f);
    glLineWidth(1.0f);
    for (int i = -25; i <= 25; i += 2) {
        glBegin(GL_LINES);
            glVertex3f((float)i, y+0.001f, -25);
            glVertex3f((float)i, y+0.001f,  25);
            glVertex3f(-25, y+0.001f, (float)i);
            glVertex3f( 25, y+0.001f, (float)i);
        glEnd();
    }

    glEnable(GL_LIGHTING);
}

void GlWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Orbit camera
    float azRad = qDegreesToRadians(m_azimuth);
    float elRad = qDegreesToRadians(m_elevation);
    float ex = m_distance * cosf(elRad) * sinf(azRad);
    float ey = m_distance * sinf(elRad) + m_pivotY;
    float ez = m_distance * cosf(elRad) * cosf(azRad);

    gluLookAt(ex, ey, ez,
              0.0, m_pivotY, 0.0,
              0.0, 1.0, 0.0);

    // Update light positions in world space
    GLfloat p0[] = { 4.0f, 8.0f, 5.0f, 0.0f };
    GLfloat p1[] = {-3.0f, 2.0f,-4.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, p0);
    glLightfv(GL_LIGHT1, GL_POSITION, p1);

     if (m_scene && m_loaded) {
        m_scene->drawReflectiveFloor();   // пол
        m_scene->drawCarReflection();     // отражение
        m_scene->draw();                  // машина + лучи
    }

    if (m_scene && m_loaded)
        m_scene->draw();
}

// ── Camera interaction ────────────────────────────────────────

void GlWidget::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton || e->button() == Qt::RightButton) {
        m_lastPos  = e->pos();
        m_dragging = true;
    }
}

void GlWidget::mouseMoveEvent(QMouseEvent* e)
{
    if (!m_dragging) return;
    QPoint d = e->pos() - m_lastPos;

    if (e->buttons() & Qt::LeftButton) {
        m_azimuth   += d.x() * 0.5f;
        m_elevation -= d.y() * 0.5f;
        m_elevation  = qBound(-89.0f, m_elevation, 89.0f);
    }
    if (e->buttons() & Qt::RightButton) {
        m_distance -= d.y() * 0.05f;
        m_distance  = qBound(1.5f, m_distance, 50.0f);
    }
    m_lastPos = e->pos();
    update();
}

void GlWidget::mouseReleaseEvent(QMouseEvent*) { m_dragging = false; }

void GlWidget::wheelEvent(QWheelEvent* e)
{
    m_distance -= e->angleDelta().y() * 0.008f;
    m_distance  = qBound(1.5f, m_distance, 50.0f);
    update();
}
