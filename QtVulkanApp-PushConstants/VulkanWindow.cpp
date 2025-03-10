#include "VulkanWindow.h"
#include "RenderWindow.h"
#include <QKeyEvent>

void VulkanWindow::setCameraSpeed(float value)
{
    mCameraSpeed += value;

    //Keep within some min and max values
    if(mCameraSpeed < 0.01f)
        mCameraSpeed = 0.01f;
    if(mCameraSpeed > 0.3f)
         mCameraSpeed = 0.3f;
}

VulkanWindow::VulkanWindow()
{  }

QVulkanWindowRenderer* VulkanWindow::createRenderer()
{
    mRenderWindow = new RenderWindow(this,true);
    mCamera = &dynamic_cast<RenderWindow*>(mRenderWindow)->mCamera;


    //Makes a new instance of the RenderWindow (our Renderer) class
    return mRenderWindow; // last true == try MSAA
}

void VulkanWindow::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta();

    if(mInput.RMB)
    {
        if(numDegrees.y() < 1)
            setCameraSpeed(-0.002f);
        if(numDegrees.y() > 1)
            setCameraSpeed(0.002f);
    }
     qDebug("CameraSpeed: %f", mCameraSpeed);
}

void VulkanWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
        mInput.RMB = true;
    if(event->button() == Qt::LeftButton)
        mInput.LMB = true;
    if(event->button() == Qt::MiddleButton)
        mInput.MMB = true;
}

void VulkanWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(mInput.RMB)
    {
        //Using mMouseXYlast as deltaXY so we dont need extra variables
        mMouseXlast = event->pos().x() - mMouseXlast;
        mMouseYlast = event->pos().y() - mMouseYlast;

        if(mMouseXlast != 0)
            dynamic_cast<RenderWindow*>(mRenderWindow)->mCamera.yaw(-mCameraRotateSpeed * mMouseXlast);
        if(mMouseYlast != 0)
            dynamic_cast<RenderWindow*>(mRenderWindow)->mCamera.pitch(-mCameraRotateSpeed * mMouseYlast);
    }

    mMouseXlast = event->pos().x();
    mMouseYlast = event->pos().y();
}
void VulkanWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        mInput.RMB = false;
    if(event->button() == Qt::LeftButton)
        mInput.LMB = false;
    if(event->button() == Qt::MiddleButton)
        mInput.MMB = false;
}

void VulkanWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_0)
    {
        mIndex = 0;
    }

    if(event->key() == Qt::Key_1)
    {
        mIndex = 1;
    }

    //Keyboard

    if(event->key() == Qt::Key_W)
    {
        mInput.W = true;
    }
    if(event->key() == Qt::Key_A)
    {
        mInput.A = true;
    }
    if(event->key() == Qt::Key_S)
    {
        mInput.S = true;
    }
    if(event->key() == Qt::Key_D)
    {
        mInput.D = true;
    }

    // Arrow keys
    if(event->key() == Qt::Key_Up)
    {
        mInput.UP = true;
    }
    if(event->key() == Qt::Key_Down)
    {
        mInput.DOWN = true;
    }
    if(event->key() == Qt::Key_Left)
    {
        mInput.LEFT = true;
    }
    if(event->key() == Qt::Key_Right)
    {
        mInput.RIGHT = true;
    }

    if (event->key() == Qt::Key_Escape)
    {
        QCoreApplication::quit();       //Shuts down the whole program
    }

    if(event->key() == Qt::Key_Q)
    {
        mInput.Q = true;
    }
    if(event->key() == Qt::Key_E)
    {
        mInput.E = true;
    }
    if(event->key() == Qt::Key_C)
    {
        mInput.C = true;
    }

}

void VulkanWindow::keyReleaseEvent(QKeyEvent *event)
{
    //Keyboard

    if(event->key() == Qt::Key_W)
    {
        mInput.W = false;
    }
    if(event->key() == Qt::Key_A)
    {
        mInput.A = false;
    }
    if(event->key() == Qt::Key_S)
    {
        mInput.S = false;
    }
    if(event->key() == Qt::Key_D)
    {
        mInput.D = false;
    }

    // Arrow keys
    if(event->key() == Qt::Key_Up)
    {
        mInput.UP = false;
    }
    if(event->key() == Qt::Key_Down)
    {
        mInput.DOWN = false;
    }
    if(event->key() == Qt::Key_Left)
    {
        mInput.LEFT = false;
    }
    if(event->key() == Qt::Key_Right)
    {
        mInput.RIGHT = false;
    }

    if(event->key() == Qt::Key_Q)
    {
        mInput.Q = false;
    }
    if(event->key() == Qt::Key_E)
    {
        mInput.E = false;
    }
    if(event->key() == Qt::Key_C)
    {
        mInput.C = false;
    }
}

void VulkanWindow::handleInput()
{
    //Camera
    mCamera->setSpeed(0.0f); // Cancel last frame movement

    if(mInput.RMB)
    {
        if(mInput.W)
            mCamera->setSpeed(mCameraSpeed);
        if(mInput.A)
            mCamera->moveRight(mCameraSpeed);
        if(mInput.S)
            mCamera->setSpeed(-mCameraSpeed);
        if(mInput.D)
            mCamera->moveRight(-mCameraSpeed);
        if(mInput.Q)
            mCamera->updateHeight(mCameraSpeed);
        if(mInput.E)
            mCamera->updateHeight(-mCameraSpeed);
    }
}
