#include "glscene.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifdef Q_OS_MAC
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QImage>
#include <QDebug>
#include <QtMath>


static QOpenGLFunctions* glf() {
    return QOpenGLContext::currentContext()->functions();
}

// ─────────────────────────────────────────────────────────────
GlScene::GlScene(QObject* parent)
    : MovingRectangle(parent)
{
    setDimensions(1.72f, 1.39f, 4.42f);
    connect(&m_doorTimer, &QTimer::timeout,this, [this]{
        m_doorElapsed += 16;
        float t = qBound(0.0f, float(m_doorElapsed)/float(m_doorDuration), 1.0f);
        float s = t*t*(3.0f-2.0f*t);
        m_doorAngle = m_doorStartAngle + (m_doorTargetAngle - m_doorStartAngle)*s;
        applyDoorAngle(m_doorAngle);
        emit doorAnimStep(m_doorAngle);
        if(t >= 1.0f) { m_doorTimer.stop(); emit doorAnimDone(); }
    });
}

GlScene::~GlScene()
{
    for(auto& m : m_meshes) m.free();
    for(auto t : m_texCache) if(t) glDeleteTextures(1,&t);
}

double GlScene::area()      const { return 2.0*(width()*height()+width()*depth()+height()*depth()); }
double GlScene::perimeter() const { return 2.0*(width()+depth()); }

// ── aiMatrix4x4 → QMatrix4x4 ─────────────────────────────────
static QMatrix4x4 toQt(const aiMatrix4x4& m) {
    // aiMatrix4x4 is row-major, QMatrix4x4 expects row-major in ctor
    return QMatrix4x4(
        m.a1,m.a2,m.a3,m.a4,
        m.b1,m.b2,m.b3,m.b4,
        m.c1,m.c2,m.c3,m.c4,
        m.d1,m.d2,m.d3,m.d4);
}

// ── Load GLB ─────────────────────────────────────────────────
bool GlScene::loadGlb(const QString& path)
{
    Assimp::Importer imp;
    const aiScene* sc = imp.ReadFile(
        path.toStdString(),
        aiProcess_Triangulate | aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs     | aiProcess_JoinIdenticalVertices);

    if (!sc || !sc->mRootNode) {
        qCritical() << "Assimp:" << imp.GetErrorString();
        return false;
    }

    // Pre-load all embedded textures (indexed by *N)
    m_texCache.assign(sc->mNumTextures, 0);
    for (unsigned i = 0; i < sc->mNumTextures; ++i)
        m_texCache[i] = loadEmbedded(sc, i);

    QMatrix4x4 identity;
    identity.setToIdentity();
    processNode(sc, sc->mRootNode, identity);

    m_loaded = true;
    qDebug() << "Loaded" << m_meshes.size() << "meshes";
    return true;
}

// ── Process scene graph ───────────────────────────────────────
void GlScene::processNode(const aiScene* sc, const aiNode* node,
                           const QMatrix4x4& parent)
{
    QMatrix4x4 local = parent * toQt(node->mTransformation);
    QString nodeName  = QString::fromUtf8(node->mName.C_Str()).toUpper();

    for (unsigned mi = 0; mi < node->mNumMeshes; ++mi) {
        const aiMesh* aim = sc->mMeshes[node->mMeshes[mi]];

        std::vector<GlVertex> verts;
        verts.reserve(aim->mNumVertices);
        for (unsigned vi = 0; vi < aim->mNumVertices; ++vi) {
            GlVertex v;
            v.pos[0] = aim->mVertices[vi].x;
            v.pos[1] = aim->mVertices[vi].y;
            v.pos[2] = aim->mVertices[vi].z;
            if (aim->HasNormals()) {
                v.nrm[0]=aim->mNormals[vi].x;
                v.nrm[1]=aim->mNormals[vi].y;
                v.nrm[2]=aim->mNormals[vi].z;
            } else { v.nrm[0]=0;v.nrm[1]=1;v.nrm[2]=0; }
            if (aim->HasTextureCoords(0)) {
                v.uv[0]=aim->mTextureCoords[0][vi].x;
                v.uv[1]=aim->mTextureCoords[0][vi].y;
            } else { v.uv[0]=0;v.uv[1]=0; }
            verts.push_back(v);
        }

        std::vector<unsigned int> idx;
        idx.reserve(aim->mNumFaces*3);
        for (unsigned fi = 0; fi < aim->mNumFaces; ++fi)
            for (unsigned k=0; k<aim->mFaces[fi].mNumIndices; ++k)
                idx.push_back(aim->mFaces[fi].mIndices[k]);

        GlMesh mesh;
        mesh.name          = QString::fromUtf8(node->mName.C_Str());
        mesh.localTransform = local;
        mesh.animTransform.setToIdentity();
        mesh.mat = extractMaterial(sc, sc->mMaterials[aim->mMaterialIndex]);
        mesh.upload(verts, idx);

        const aiMaterial* aiMat = sc->mMaterials[aim->mMaterialIndex];
        aiString matName;
        aiMat->Get(AI_MATKEY_NAME, matName);
        qDebug() << QString("Mesh[%1]: %2 → Mat: %3, TexID: %4")
                        .arg(m_meshes.size())
                        .arg(mesh.name)
                        .arg(matName.C_Str())
                        .arg(mesh.mat.texBase);

        // Classify
        mesh.headlight = nodeName.contains("HEADLIGHT_LENS_RIGHT_MM_LIGHTS") || 
                         nodeName.contains("HEADLIGHT_LENS_LEFT_MM_LIGHTS") ||
                         nodeName.contains("TAILLIGHT") ||
                         nodeName.contains("BRAKES");
        mesh.doorLeft  = nodeName.contains("DOOR_LEFT") || 
                         nodeName.contains("MIRROR_LEFT") ||
                         nodeName.contains("GLASS_LEFT_MM");
        mesh.doorRight = nodeName.contains("DOOR_RIGHT") ||
                         nodeName.contains("MIRROR_RIGHT") ||
                         nodeName.contains("GLASS_RIGHT_MM");

        m_meshes.push_back(std::move(mesh));
    }

    for (unsigned ci = 0; ci < node->mNumChildren; ++ci)
        processNode(sc, node->mChildren[ci], local);
}

// ── Embedded texture loader ───────────────────────────────────
GLuint GlScene::loadEmbedded(const aiScene* sc, unsigned idx)
{
    const aiTexture* tex = sc->mTextures[idx];
    if (!tex) return 0;

    QImage img;
    if (tex->mHeight == 0) {
        // Compressed (PNG/JPG)
        img.loadFromData(reinterpret_cast<const uchar*>(tex->pcData), tex->mWidth);
    } else {
        img = QImage(reinterpret_cast<const uchar*>(tex->pcData),
                     tex->mWidth, tex->mHeight, QImage::Format_ARGB32).copy();
    }
    if (img.isNull()) { qWarning() << "Tex" << idx << "null"; return 0; }

   img = img.convertToFormat(QImage::Format_RGBA8888);

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 img.width(), img.height(), 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, img.constBits());
    glf()->glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    qDebug() << "Tex" << idx << img.width() << "x" << img.height() << "OK";
    return id;
}

// ── Material extraction ───────────────────────────────────────
GlMaterial GlScene::extractMaterial(const aiScene* sc, const aiMaterial* mat)
{
    GlMaterial m;

       
   

    // Base color factor
    aiColor4D col(1,1,1,1);
    if (mat->Get(AI_MATKEY_BASE_COLOR, col) == AI_SUCCESS ||
        mat->Get(AI_MATKEY_COLOR_DIFFUSE, col) == AI_SUCCESS)
        m.baseColor = {col.r, col.g, col.b, col.a};

    // PBR params
    float val;
    if (mat->Get(AI_MATKEY_METALLIC_FACTOR,  val)==AI_SUCCESS) m.metallic  = val;
    if (mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, val)==AI_SUCCESS) m.roughness = val;

    // Emissive
    aiColor3D ec;
    if (mat->Get(AI_MATKEY_COLOR_EMISSIVE, ec)==AI_SUCCESS)
        m.emissive = {ec.r, ec.g, ec.b};

    // ── TEXTURE: try all relevant types in priority order ────
    // For GLTF2 in Assimp 5.x, baseColorTexture is under BASE_COLOR or DIFFUSE
    const aiTextureType tryTypes[] = {
        aiTextureType_BASE_COLOR,     // Assimp 5+ GLTF2 PBR
        aiTextureType_DIFFUSE,        // fallback
        aiTextureType_UNKNOWN,        // some importers use this
    };

    for (auto ttype : tryTypes) {
        aiString texPath;
        if (mat->GetTexture(ttype, 0, &texPath) != AI_SUCCESS) continue;
        QString tp = QString::fromUtf8(texPath.C_Str());
        if (tp.startsWith('*')) {
            int tidx = tp.mid(1).toInt();
            if (tidx >= 0 && tidx < (int)m_texCache.size() && m_texCache[tidx]) {
                m.texBase = m_texCache[tidx];
                break;
            }
        }
    }
        aiString matName;
    mat->Get(AI_MATKEY_NAME, matName);
    QString name = QString::fromUtf8(matName.C_Str()).toUpper();
    
    if (name.contains("005")) {  // Material.005 = windows/glass
        m.baseColor[3] = 0.9f;  // alpha = 25% прозрачность
    }
    
   
    

    qDebug() << "   → Assigned TexID:" << m.texBase;
    return m;
}



// ── Draw ─────────────────────────────────────────────────────
void GlScene::draw()
{
    QMatrix4x4 worldMat = getModelMatrix();
    const float* wm = worldMat.constData();

    glPushMatrix();
    glMultMatrixf(wm);

    for (const GlMesh& mesh : m_meshes) {
        if (mesh.mat.baseColor[3] < 0.99f) continue;  // пропустить прозрачне
        
        glPushMatrix();

        // ── FIX: Door pivot operates in CAR-LOCAL space (world space) ──
        // Order: vertex → localTransform → animTransform → worldMat
        // In GL stack (push order = reverse application):
        //   glMultMatrixf(animTransform)  ← applied AFTER localTransform
        //   glMultMatrixf(localTransform) ← applied first to vertex
        if (mesh.doorLeft || mesh.doorRight) {
            glMultMatrixf(mesh.animTransform.constData());
        }
        glMultMatrixf(mesh.localTransform.constData());

        mesh.draw();
        glPopMatrix();
    }

    // ← ПОТОМ прозрачные мешы (без записи  depth buffer)
    glDepthMask(GL_FALSE);  // не записывать в depth buffer
    for (const GlMesh& mesh : m_meshes) {
        if (mesh.mat.baseColor[3] >= 0.99f) continue;  // пропустить непрозрачные
        
        glPushMatrix();
        if (mesh.doorLeft || mesh.doorRight) {
            glMultMatrixf(mesh.animTransform.constData());
        }
        glMultMatrixf(mesh.localTransform.constData());
        mesh.draw();
        glPopMatrix();
    }
   

   glDepthMask(GL_TRUE);  // включить обратно

    // ── Draw light beams from headlights ──────────────────────
  
    if (m_headlightsOn) {
        QVector3D leftOrigin(0.56f, 0.57f, 1.96f);
        QVector3D rightOrigin(-0.56f, 0.57f, 1.96f);
        QVector3D direction(0.0f, 0.0f, 1.0f);
        
        float s = scaleValue();
        
        drawLightBeam(leftOrigin, direction, 
                      8.0f * s, 0.15f * s, 1.2f * s,
                      QVector3D(1.0f, 0.95f, 0.8f), 0.3f);
        
        drawLightBeam(rightOrigin, direction, 
                      8.0f * s, 0.15f * s, 1.2f * s,
                      QVector3D(1.0f, 0.95f, 0.8f), 0.3f);
    }

    glPopMatrix(); 
}

// ── Door animation ────────────────────────────────────────────
void GlScene::openDoors(int ms)
{
    m_doorTimer.stop();
    m_doorStartAngle=m_doorAngle; m_doorTargetAngle=-65.0f;
    m_doorDuration=ms; m_doorElapsed=0; m_doorTimer.start(16);
}

void GlScene::closeDoors(int ms)
{
    m_doorTimer.stop();
    m_doorStartAngle=m_doorAngle; m_doorTargetAngle=0.0f;
    m_doorDuration=ms; m_doorElapsed=0; m_doorTimer.start(16);
}

// Hinge in CAR-LOCAL / world space (after 90°X root rotation):
//   World X = GLTF X  → Left door inner edge: X = +0.584
//   World Y = -GLTF Z → height (doesn't matter for Y-axis rotation)
//   World Z = GLTF Y  → Front edge of door:   Z = +0.891
//
// animTransform = T(hinge) * Ry(angle) * T(-hinge)  in car-local space
// Applied AFTER localTransform in draw(), so it IS in car-local/world space.
void GlScene::applyDoorAngle(float angle)
{
    for (GlMesh& mesh : m_meshes) {
        if (!mesh.doorLeft && !mesh.doorRight) continue;

        // Hinge X: body-side (inner) X of door
        float hx   = mesh.doorLeft ? 0.7f : -0.7f;
        float hy   = 0.0f;  // Y-axis rotation: hinge Y irrelevant
        float hz   = 0.89f;  // front edge of door (world Z = GLTF Y max)
        float sign = mesh.doorLeft ? 1.0f : -1.0f;  // left opens +Y, right opens -Y

        QMatrix4x4 mat;
        mat.translate(hx, hy, hz);
        mat.rotate(sign * angle, 0.0f, 1.0f, 0.0f);  // world Y = up axis
        mat.translate(-hx, -hy, -hz);
        mesh.animTransform = mat;
    }
}

// ── Headlights ────────────────────────────────────────────────
void GlScene::setHeadlights(bool on)
{
    m_headlightsOn = on;
    for (GlMesh& mesh : m_meshes) {
        if (!mesh.headlight) continue;
        QString nm = mesh.name.toUpper();
        if (on) {
            if (nm.contains("HEADLIGHT_LENS_RIGHT_MM_LIGHTS") || nm.contains("HEADLIGHT_LENS_LEFT_MM_LIGHTS") )
                mesh.mat.emissive = {8.0f, 7.5f, 6.5f};
            else  // taillights / brakes
                mesh.mat.emissive = {3.5f,0.1f,0.1f};
        } else {
            mesh.mat.emissive = {0.0f,0.0f,0.0f};
        }
    }
}

// ── Body color ────────────────────────────────────────────────
void GlScene::setBodyColor(const QColor& c)
{
    float r=c.redF(), g=c.greenF(), b=c.blueF();
    for (GlMesh& mesh : m_meshes) {
        QString nm = mesh.name.toUpper();
        if (nm.contains("BODY_MM_EXT")       || nm.contains("DOOR_LEFT_MM_EXT") ||
            nm.contains("DOOR_RIGHT_MM_EXT") || nm.contains("HOOD_MM_EXT")      ||
            nm.contains("BOOT_MM_EXT")       || nm.contains("REARBUMPER_MM_EXT")||
            nm.contains("FRONTBUMPER_MM_EXT")|| nm.contains("MIRROR_LEFT_MM_EXT")||
            nm.contains("MIRROR_RIGHT_MM_EXT")) {
            mesh.mat.baseColor = {r, g, b, 1.0f};
            mesh.mat.texBase = 0;
        }
    }
}

// ── Light beam effect ─────────────────────────────────────────
void GlScene::drawLightBeam(const QVector3D& origin, const QVector3D& direction,
                            float length, float startRadius, float endRadius,
                            const QVector3D& color, float alpha)
{
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);  // additive blending для свечения
    glDepthMask(GL_FALSE);

    const int segments = 32;  // количество сегментов конуса
    QVector3D end = origin + direction * length;

    // Вектор "вверх" для построения конуса
    QVector3D up = qAbs(direction.y()) > 0.9f
                       ? QVector3D(1, 0, 0)
                       : QVector3D(0, 1, 0);
    QVector3D right = QVector3D::crossProduct(direction, up).normalized();
    QVector3D actualUp = QVector3D::crossProduct(right, direction).normalized();

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float angle = (float)i / segments * 2.0f * M_PI;
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);

        // Начальная точка (ярко)
        QVector3D startOffset = (right * cosA + actualUp * sinA) * startRadius;
        QVector3D startPoint = origin + startOffset;
        glColor4f(color.x(), color.y(), color.z(), alpha * 0.8f);
        glVertex3f(startPoint.x(), startPoint.y(), startPoint.z());

        // Конечная точка (прозрачно)
        QVector3D endOffset = (right * cosA + actualUp * sinA) * endRadius;
        QVector3D endPoint = end + endOffset;
        glColor4f(color.x(), color.y(), color.z(), 0.0f);
        glVertex3f(endPoint.x(), endPoint.y(), endPoint.z());
    }
    glEnd();
    
    // ← ПОЛНОЕ ВОССТАНОВЛЕНИЕ GL СОСТОЯНИЯ
    glDepthMask(GL_TRUE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);  
    glEnable(GL_LIGHTING);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); 
}
// ── Reflective floor ──────────────────────────────────────────
void GlScene::drawReflectiveFloor()
{
    float y = -0.10f;
    float size = 25.0f;
    
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Тёмно-серый глянцевый пол с лёгким градиентом
    glBegin(GL_QUADS);
        glColor4f(0.15f, 0.16f, 0.18f, 1.0f);  // темнее по краям
        glVertex3f(-size, y, -size);
        glVertex3f( size, y, -size);
        
        glColor4f(0.20f, 0.21f, 0.24f, 1.0f);  // светлее в центре
        glVertex3f( size, y,  size);
        glVertex3f(-size, y,  size);
    glEnd();
    
    // Сетка на полу
    glColor4f(0.25f, 0.27f, 0.30f, 0.5f);
    glLineWidth(1.0f);
    for (int i = -25; i <= 25; i += 2) {
        glBegin(GL_LINES);
            glVertex3f((float)i, y+0.001f, -size);
            glVertex3f((float)i, y+0.001f,  size);
            glVertex3f(-size, y+0.001f, (float)i);
            glVertex3f( size, y+0.001f, (float)i);
        glEnd();
    }
    
    glEnable(GL_LIGHTING);
}

void GlScene::drawCarReflection()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glPushMatrix();
    
    // Зеркальное отражение: flip по Y и сдвиг вниз
    glTranslatef(0.0f, -1.40f, 0.0f);  // 2 * (-0.70)
    glScalef(1.0f, -1.0f, 1.0f);
    
    // Инвертировать culling для отражения
    glCullFace(GL_FRONT);
    
    // Рисуем машину с прозрачностью (отражение)
    glColor4f(1.0f, 1.0f, 1.0f, 0.35f);  // 35% непрозрачности
    
    QMatrix4x4 worldMat = getModelMatrix();
    glMultMatrixf(worldMat.constData());
    
    // Только непрозрачные части (упрощённое отражение)
    for (const GlMesh& mesh : m_meshes) {
        if (mesh.mat.baseColor[3] < 0.99f) continue;
        
        glPushMatrix();
        if (mesh.doorLeft || mesh.doorRight) {
            glMultMatrixf(mesh.animTransform.constData());
        }
        glMultMatrixf(mesh.localTransform.constData());
        
        // Затемнить отражение
        auto origColor = mesh.mat.baseColor;
        const_cast<GlMesh&>(mesh).mat.baseColor = {
            origColor[0]*0.3f, origColor[1]*0.3f, origColor[2]*0.3f, 0.35f
        };
        mesh.draw();
        const_cast<GlMesh&>(mesh).mat.baseColor = origColor;
        
        glPopMatrix();
    }
    
        glCullFace(GL_BACK);
    glPopMatrix();
    glDisable(GL_BLEND);
}
