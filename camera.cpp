#include "camera.h"

Camera::Camera() : mEye{0,0,0}
{  
    mPmatrix.setToIdentity();
    mVmatrix.setToIdentity();
}

void Camera::init()
{
    mPmatrix.setToIdentity();
    mVmatrix.setToIdentity();

    mVmatrix.translate(mPosition);
}

void Camera::perspective(int degrees, double aspect, double nearplane, double farplane)
{
    mPmatrix.perspective(degrees, aspect, nearplane, farplane);
}

void Camera::lookAt(const QVector3D& eye, const QVector3D& at, const QVector3D& up)
{
    mEye = eye;
    mVmatrix.lookAt(eye, at, up);
}

void Camera::MoveForward(float amount)
{
    mVmatrix.translate(amount * GetForward());
    mPosition += GetForward() * amount;
}

void Camera::MoveRight(float amount)
{
   mVmatrix.translate(amount * GetRight());
   mPosition += GetRight() * amount;
}

void Camera::MoveUp(float amount){
    mPosition.setY(mPosition.y() + amount);
}
void Camera::RotateRight(float amount)
{
    mVmatrix.rotate(amount, QVector3D(0, 1,0));
}

void Camera::SetPosition(QVector3D pos)
{
    mVmatrix.setToIdentity();
    mEye = pos;
    mVmatrix.translate(pos);
}

QVector3D Camera::GetPosition(){
    return mEye;
}

QVector3D Camera::GetForward()
{
    QVector3D fwd = mVmatrix.inverted().column(0).toVector3D();
    fwd.setX(-fwd.x());
    return fwd;
}

QVector3D Camera::GetRight()
{
    QVector3D rgt = mVmatrix.inverted().column(2).toVector3D();
    return rgt;
}
