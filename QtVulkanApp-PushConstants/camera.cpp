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

void Camera::translate(float dx, float dy, float dz)
{
    mViewMatrix.translate(dx,dy,dz);
}

QMatrix4x4 Camera::cMatrix()
{
    return mProjectionMatrix* mViewMatrix;
}
