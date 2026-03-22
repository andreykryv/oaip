#pragma once
#include "movingrectangle.h"
#include "glmesh.h"

#include <QTimer>
#include <QColor>
#include <vector>

struct aiScene;
struct aiNode;
struct aiMaterial;

// ============================================================
// GlScene — ДОЧЕРНИЙ класс MovingRectangle.
// Загружает GLB (Mercedes 190 Evo II) через Assimp,
// рендерит через OpenGL, анимирует двери, управляет фарами.
// ============================================================
class GlScene : public MovingRectangle
{
    Q_OBJECT
public:
    explicit GlScene(QObject* parent = nullptr);
    ~GlScene() override;

    QString typeName()  const override { return "Mercedes-Benz 190 Evo II"; }
    double  area()      const override;
    double  perimeter() const override;

    // Must be called with active GL context
    bool loadGlb(const QString& path);
    void draw();

    // Car-specific
    void openDoors (int ms = 1400);
    void closeDoors(int ms = 1400);
    void setHeadlights(bool on);
    void toggleHeadlights() { setHeadlights(!m_headlightsOn); }
    void setBodyColor(const QColor& c);
      void drawReflectiveFloor();
    void drawCarReflection();

    float doorAngle()    const { return m_doorAngle; }
    bool  headlightsOn() const { return m_headlightsOn; }
    bool  isLoaded()     const { return m_loaded; }

signals:
    void doorAnimStep(float angle);
    void doorAnimDone();

private:
    void processNode(const aiScene* sc, const aiNode* node,
                     const QMatrix4x4& parent);
    GLuint      loadEmbedded   (const aiScene* sc, unsigned idx);
    GlMaterial  extractMaterial(const aiScene* sc, const aiMaterial* mat);
    void applyDoorAngle(float angle);

    std::vector<GlMesh> m_meshes;
    std::vector<GLuint> m_texCache;   // indexed by embedded texture *N
    bool m_loaded = false;

    // Door animation
    float  m_doorAngle = 0, m_doorStartAngle = 0, m_doorTargetAngle = 0;
    int    m_doorDuration = 1400, m_doorElapsed = 0;
    QTimer m_doorTimer;

    bool m_headlightsOn = false;

    // ── Light beams ───────────────────────────────────────────
    void drawLightBeam(const QVector3D& origin, const QVector3D& direction,
                       float length, float startRadius, float endRadius,
                       const QVector3D& color, float alpha);


};
