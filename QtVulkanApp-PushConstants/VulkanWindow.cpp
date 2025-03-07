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
    if(event->key() == Qt::Key_0)
    {
        mIndex = 0;
    }

    if(event->key() == Qt::Key_1)
    {
        mIndex = 1;
    }

    if (event->key() == Qt::Key_A)
    {
        qDebug("I pressed the A button");
        dynamic_cast<RenderWindow*>(mRenderWindow)->mObjects.at(mIndex)->move(-0.1f);
    }

    if(event->key() == Qt::Key_S)
    {
        qDebug("I pressed the S button");
        dynamic_cast<RenderWindow*>(mRenderWindow)->mObjects.at(mIndex)->scale(0.9f);
    }

    if (event->key() == Qt::Key_Escape)
    {
        QCoreApplication::quit();       //Shuts down the whole program
    }

    if(event->key() == Qt::Key_T)
    {
        dynamic_cast<RenderWindow*>(mRenderWindow)->mCamera.translate(.0f,.0f,1.0f);

    }
    if(event->key() == Qt::Key_R)
    {
        dynamic_cast<RenderWindow*>(mRenderWindow)->mCamera.rotate(45.0f, 0.0f, 0.0f, 1.0f);

    }

}
