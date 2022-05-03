#ifndef CAMERA_H
#define CAMERA_H
#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <QVector3D>
class Camera : public QOpenGLFunctions_4_1_Core
{
private:
    QVector3D mEye{0,0,0};
public:
    QMatrix4x4 mPmatrix;
    QMatrix4x4 mVmatrix;
public:
    Camera();
    ~Camera() { }
    void init();
    void perspective(int degrees, double aspect, double nearplane, double farplane);
    void lookAt(const QVector3D& eye, const QVector3D& at, const QVector3D& up);

    void MoveForward(float amount);
    void MoveRight(float amount);
    void MoveUp(float amount);

    void RotateRight(float amount);

    void SetPosition(QVector3D pos);
    QVector3D GetPosition();
private:
    QVector3D mPosition;
    QVector3D GetForward();
    QVector3D GetRight();
};

#endif // CAMERA_H
