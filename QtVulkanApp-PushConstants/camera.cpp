#include "camera.h"

Camera::Camera() {}
Camera::~Camera() {}

void Camera::init()
{
    mProjectionMatrix.setToIdentity();
    mViewMatrix.setToIdentity();
}

void Camera::perspective(int degrees, double aspect, double nearplane, double farplane)
{
    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.perspective(degrees,aspect,nearplane,farplane);
    //Flip projection because Vulkan's -y axis
    mProjectionMatrix.scale(1.0f,-1.0f,1.0f);
}

void Camera::lookAt(const QVector3D &eye, const QVector3D &at, const QVector3D &up)
{
    mEye = eye;
    mAt = at;
    mUp = up;
    mViewMatrix.setToIdentity();
    mViewMatrix.lookAt(mEye,mAt,mUp);
}

void Camera::setSpeed(float speed)
{
    mSpeed = speed;

}
void Camera::moveRight(float delta)
{
    mPosition.setX(mPosition.x() + delta);
}

void Camera::updateHeight(float deltaHeight)
{
    mPosition.setY(mPosition.y() + deltaHeight);
}

void Camera::setPosition(const QVector3D& position)
{
    mPosition = position;
    update();
}

void Camera::update()
{
    mViewMatrix.setToIdentity();
    mPosition.setZ(mPosition.z() + mSpeed);
    //mViewMatrix.translate(mPosition); //makes rotation work around world origo
    mViewMatrix.rotate(mYaw,0.f,1.f,0.f);
    mViewMatrix.rotate(mPitch,1.f,0.f,0.f); // pitch then yaw makes camera wonkey

     mViewMatrix.translate(mPosition); //Makes rotation work around camera origo
}

void Camera::translate(float dx, float dy, float dz)
{
    mViewMatrix.translate(dx,dy,dz);
}

void Camera::rotate(float t, float x, float y, float z)
{
    mViewMatrix.rotate(t,x,y,z);
}

QMatrix4x4 Camera::cMatrix()
{
    return mProjectionMatrix* mViewMatrix;
}

void Camera::pitch(float degrees)
{
    mPitch += degrees;
}

void Camera::yaw(float degrees)
{
    mYaw += degrees;
}

