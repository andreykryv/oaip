#pragma once

#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QVector3D>
#include <QString>
#include <vector>
#include <array>

typedef unsigned int GLuint;
typedef int          GLsizei;

// ============================================================
// GlMesh — один меш из GLB, готовый к рендерингу.
// Хранит VBO/IBO + материальные параметры + трансформ.
// ============================================================
struct GlVertex {
    float pos[3];
    float nrm[3];
    float uv[2];
};

struct GlMaterial {
    std::array<float,4> baseColor = {0.6f,0.6f,0.6f,1.0f};
    std::array<float,3> emissive  = {0.0f,0.0f,0.0f};
    float               roughness = 0.7f;
    float               metallic  = 0.0f;
    GLuint              texBase   = 0;  // GL texture ID (0 = none)
};

class GlMesh {
public:
    QString     name;
    GlMaterial  mat;
    QMatrix4x4  localTransform;   // from GLB node transform
    QMatrix4x4  animTransform;    // for door animation (identity by default)

    bool        headlight = false;  // is this a headlight/taillight mesh?
    bool        doorLeft  = false;
    bool        doorRight = false;

    // GL objects
    GLuint vbo = 0, ibo = 0;
    GLsizei indexCount = 0;

    void upload(const std::vector<GlVertex>& verts,
                const std::vector<unsigned int>& idx);
    void draw() const;
    void free();
};
