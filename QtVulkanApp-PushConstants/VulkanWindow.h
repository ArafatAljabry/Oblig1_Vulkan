#ifndef VULKANWINDOW_H
#define VULKANWINDOW_H

#include <QVulkanWindow>
#include "visualobject.h"
#include "input.h"

/*The QVulkanWindow subclass reimplements the factory function QVulkanWindow::createRenderer().
This returns a new instance of the QVulkanWindowRenderer subclass.
A QVulkanWindow is a Vulkan-capable QWindow that manages a Vulkan device, a graphics queue,
a command pool and buffer, a depth-stencil image and a double-buffered FIFO swapchain, etc.
In order to be able to access various Vulkan resources via the window object,
a pointer to the window is passed and stored via the constructor.*/
class VulkanWindow : public QVulkanWindow
{
    Q_OBJECT

private:
    void setCameraSpeed(float value);

    float mCameraSpeed{0.005f};
    float mCameraRotateSpeed{-0.1f};
    int mMouseXlast {0}; // for mouse rotate input
    int mMouseYlast {0};
    Input mInput;
    float objectMovementSpeed{0.25f};
    //For iteration through object container
    int objectIterator{0};
    class Camera* mCamera{nullptr};

public:
    VulkanWindow();

    QVulkanWindowRenderer* createRenderer() override;
    QVulkanWindowRenderer* getRendererWindow() const {return mRenderWindow;}
    void setSelectedObject(VisualObject* object){ mSelectedObject = object;}
    void setObjectMovementSpeed(float newSpeed){objectMovementSpeed = newSpeed;}
    void handleInput();
    float getObjectMovementSpeed(){return objectMovementSpeed;}
signals:
    void frameQueued(int colorValue);

protected:
    //The QVulkanWindow is a QWindow that we inherit from and have these functions
    // to capture mouse and keyboard.
    // Uncomment to use (you also have to make the definitions of
    // these functions in the cpp-file to use them of course!)
    //
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
              //the only one we use now
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    QVulkanWindowRenderer* mRenderWindow;
    //int mIndex{0};
    VisualObject* mSelectedObject{nullptr};


};
#endif // VULKANWINDOW_H
