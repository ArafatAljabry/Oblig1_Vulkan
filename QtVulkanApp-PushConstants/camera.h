#ifndef CAMERA_H
#define CAMERA_H
#include <QMatrix4x4>
class Camera
{
private:
    QVector3D mEye{0.0,0.0,9.0}; //Kameraposisjon
    QVector3D mAt{0.0,0.0,9.0}; // Kamerafokus
    QVector3D mUp{0.0,1.0,0.0};// Kamera-orientering

    QMatrix4x4 mProjectionMatrix{};
    QMatrix4x4 mViewMatrix{};

    QVector3D mPosition{0.0f, 0.0f,0.0f};
    float mPitch{0.0f};
    float mYaw{0.0f};

    float mSpeed{0.0f};
public:
    Camera();
    ~Camera();

    void init();
    void perspective(int degrees, double aspect, double nearplane, double farplane);
    void lookAt(const QVector3D& eye, const QVector3D& at, const QVector3D& up);

    void setSpeed(float speed);
    void moveRight(float delta);
    void updateHeight(float deltaHeight);

    void setPosition(const QVector3D& position);
    void update();
    void pitch(float degrees);
    void yaw(float degrees);

    void translate(float dx, float dy, float dz);
    void rotate(float t, float x, float y, float z);
    QMatrix4x4 cMatrix();
};

#endif // CAMERA_H
