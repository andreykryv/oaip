#include "movingrectangle.h"
#include <QtMath>

MovingRectangle::MovingRectangle(QObject* parent) : QObject(parent)
{
    connect(&m_timer, &QTimer::timeout, this, &MovingRectangle::onTick);
}

double MovingRectangle::area()      const { return 2.0*(m_w*m_h + m_w*m_d + m_h*m_d); }
double MovingRectangle::perimeter() const { return 2.0*(m_w + m_d); }

QMatrix4x4 MovingRectangle::getModelMatrix() const {
    QMatrix4x4 mat;
    mat.translate(m_pos);
    mat.rotate(m_orient);
    mat.scale(m_scale);
    return mat;
}

void MovingRectangle::translate(float dx, float dy, float dz)
{ m_pos += {dx,dy,dz}; }

void MovingRectangle::rotateDeg(float angle, float ax, float ay, float az)
{ m_orient = QQuaternion::fromAxisAndAngle(ax,ay,az,angle) * m_orient; }

void MovingRectangle::setScale(float s) { m_scale = s; }
void MovingRectangle::resetTransform()  { m_pos={}; m_orient={}; m_scale=1.0f; }

void MovingRectangle::animMoveTo(QVector3D target, int ms)
{
    m_timer.stop();
    m_ak=AK::Move; m_aStartPos=m_pos; m_aTargetPos=target;
    m_aDur=ms; m_aElapsed=0; m_timer.start(kTick);
}
void MovingRectangle::animRotate(float deg, QVector3D axis, int ms)
{
    m_timer.stop();
    m_ak=AK::Rotate; m_aTotalDeg=deg; m_aAccumDeg=0;
    m_aAxis=axis.normalized(); m_aDur=ms; m_aElapsed=0; m_timer.start(kTick);
}
void MovingRectangle::animScaleTo(float target, int ms)
{
    m_timer.stop();
    m_ak=AK::Scale; m_aStartScl=m_scale; m_aTargetScl=target;
    m_aDur=ms; m_aElapsed=0; m_timer.start(kTick);
}

void MovingRectangle::onTick()
{
    m_aElapsed += kTick;
    float t = qBound(0.0f, float(m_aElapsed)/float(m_aDur), 1.0f);
    float s = t*t*(3.0f-2.0f*t);

    switch(m_ak) {
    case AK::Move:
        m_pos = m_aStartPos + (m_aTargetPos - m_aStartPos)*s; break;
    case AK::Rotate: {
        float deg = m_aTotalDeg*s, delta = deg - m_aAccumDeg;
        m_aAccumDeg = deg;
        m_orient = QQuaternion::fromAxisAndAngle(m_aAxis, delta) * m_orient; break;
    }
    case AK::Scale:
        m_scale = m_aStartScl + (m_aTargetScl - m_aStartScl)*s; break;
    default: break;
    }
    emit animStep();
    if(t >= 1.0f) { m_timer.stop(); m_ak=AK::None; emit animFinished(); }
}
