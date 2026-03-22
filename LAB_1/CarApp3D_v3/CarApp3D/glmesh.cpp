#include "glmesh.h"
#include <QOpenGLFunctions>
#include <QOpenGLContext>

// Helper: get GL functions from current context
static QOpenGLFunctions* glf()
{
    return QOpenGLContext::currentContext()->functions();
}

void GlMesh::upload(const std::vector<GlVertex>& verts,
                     const std::vector<unsigned int>& idx)
{
    auto* f = glf();
    if (!f) return;
    indexCount = (GLsizei)idx.size();

    f->glGenBuffers(1, &vbo);
    f->glBindBuffer(GL_ARRAY_BUFFER, vbo);
    f->glBufferData(GL_ARRAY_BUFFER,
                    verts.size() * sizeof(GlVertex),
                    verts.data(), GL_STATIC_DRAW);
    f->glBindBuffer(GL_ARRAY_BUFFER, 0);

    f->glGenBuffers(1, &ibo);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                    idx.size() * sizeof(unsigned int),
                    idx.data(), GL_STATIC_DRAW);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


    void GlMesh::draw() const
{
    if (!vbo || !ibo || indexCount == 0) return;
    auto* f = glf();
    if (!f) return;

    // ← ОТКЛЮЧИТЬ COLOR_MATERIAL для правильного применения материалов
    glDisable(GL_COLOR_MATERIAL);
    
    // Установить material ambient и diffuse
    const auto& c = mat.baseColor;
    GLfloat matAmbient[]  = {c[0]*0.3f, c[1]*0.3f, c[2]*0.3f, c[3]};
    GLfloat matDiffuse[]  = {c[0], c[1], c[2], c[3]};
    GLfloat matSpecular[] = {0.3f, 0.3f, 0.3f, 1.0f};
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 32.0f);

    // Emissive
    GLfloat em[] = { mat.emissive[0], mat.emissive[1], mat.emissive[2], 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, em);

    // Texture
    if (mat.texBase) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, mat.texBase);
    } else {
        glDisable(GL_TEXTURE_2D);
    }

    f->glBindBuffer(GL_ARRAY_BUFFER, vbo);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    if (mat.texBase) glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer  (3, GL_FLOAT, sizeof(GlVertex), (void*)offsetof(GlVertex, pos));
    glNormalPointer  (   GL_FLOAT, sizeof(GlVertex), (void*)offsetof(GlVertex, nrm));
    if (mat.texBase)
        glTexCoordPointer(2, GL_FLOAT, sizeof(GlVertex), (void*)offsetof(GlVertex, uv));

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
  if (mat.baseColor[3] < 0.99f) {
        glDisable(GL_CULL_FACE);
    }
    
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    
    
    if (mat.baseColor[3] < 0.99f) {
        glEnable(GL_CULL_FACE);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    if (mat.texBase) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    f->glBindBuffer(GL_ARRAY_BUFFER, 0);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glEnable(GL_COLOR_MATERIAL);
}

void GlMesh::free()
{
    auto* f = glf();
    if (!f) return;
    if (vbo) { f->glDeleteBuffers(1, &vbo); vbo = 0; }
    if (ibo) { f->glDeleteBuffers(1, &ibo); ibo = 0; }
    if (mat.texBase) { glDeleteTextures(1, &mat.texBase); mat.texBase = 0; }
}
