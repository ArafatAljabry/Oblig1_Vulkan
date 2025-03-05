#include "VulkanWindow.h"
#include "RenderWindow.h"
#include <QKeyEvent>

VulkanWindow::VulkanWindow()
{  }

QVulkanWindowRenderer* VulkanWindow::createRenderer()
{
    //Makes a new instance of the RenderWindow (our Renderer) class
    return mRenderWindow = new RenderWindow(this, true); // last true == try MSAA
}

void VulkanWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_A)
    {
        qDebug("I pressed the A button");
        dynamic_cast<RenderWindow*>(mRenderWindow)->mCamera.translate(1.0f, 0.0f, 0.0f);
    }
    if(event->key() == Qt::Key_S)
    {
        qDebug("I pressed the S button");
        dynamic_cast<RenderWindow*>(mRenderWindow)->mCamera.translate(0.0f, 0.0f, -1.0f);
    }
    if (event->key() == Qt::Key_D)
    {
        qDebug("I pressed the D button");
        dynamic_cast<RenderWindow*>(mRenderWindow)->mCamera.translate(-1.0f, 0.0f, 0.0f);
    }
    if(event->key() == Qt::Key_W)
    {
        qDebug("I pressed the W button");
        dynamic_cast<RenderWindow*>(mRenderWindow)->mCamera.translate(0.0f, 0.0f, 1.0f);
    }
    if (event->key() == Qt::Key_Escape)
    {
        QCoreApplication::quit();       //Shuts down the whole program
    }
    if(event->key() == Qt::Key_T)
    {

    }

}
