#include "RenderWindow.h"
#include <QVulkanFunctions>
#include <QFile>
#include "VulkanWindow.h"
#include "WorldAxis.h"
#include "box.h"
#include "plane.h"
#include "rooflesshouse.h"
#include "wall.h"
#include <qmath.h>



// Hardcoded mesh for now. Will be put in its own class soon!
// NB 1: Vulkan's near/far plane (Z axis) is at 0/1 instead of -1/1, as in OpenGL!
// NB 2: Vulkan Y is negated in clip space so we fix that when making the projection matrix
// **PLAY WITH THIS**
/*static float vertexData[] = {
    // Y up, front = CCW
    // X,     Y,     Z,     R,    G,    B
    0.0f,   0.5f,  0.0f,   1.0f, 0.0f, 0.0f,    //top vertex - red
    -0.5f,  -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    //bottom left vertex - green
    0.5f,  -0.5f,  0.0f,   0.0f, 0.0f, 1.0f     //bottom right vertex - blue
};*/

//Utility function for alignment:
static inline VkDeviceSize aligned(VkDeviceSize v, VkDeviceSize byteAlign)
{
    return (v + byteAlign - 1) & ~(byteAlign - 1);
}


/*** RenderWindow class ***/

RenderWindow::RenderWindow(QVulkanWindow *w, bool msaa)
	: mWindow(w)
{
    if (msaa) {
        const QList<int> counts = w->supportedSampleCounts();
        qDebug() << "Supported sample counts:" << counts;
        for (int s = 16; s >= 4; s /= 2) {
            if (counts.contains(s)) {
                qDebug("Requesting sample count %d", s);
                mWindow->setSampleCount(s);
                break;
            }
        }
    }

    //mObjects.push_back(new TriangleSurface("vertex1.txt"));
    //mObjects.push_back(new VKGraph("vertex2.txt"));
   // mObjects.push_back(new TriangleSurface("vertex3.txt"));
    //mObjects.push_back(new TriangleSurface());


    //player
    mPlayer = new box(0,0,0);
    mPlayer->setName("player");
    mPlayer->setTag("player");
    mObjects.push_back(mPlayer);
    mPlayer->move(10,1,20);


    /******************************************
     * Scene setup
     ******************************************/
    mObjects.push_back(new WorldAxis());
    mObjects.push_back(new plane());
    // !at(0) er spilleren
    mObjects.at(1)->setName("axis");
    mObjects.at(2)->setName("plane");
    //  axis and plane
    mObjects.at(1)->enableCollision = false;
    mObjects.at(2)->enableCollision = false;

    //pickups
    mObjects.push_back(new box(1,1,0));
    mObjects.push_back(new box(1,1,0));
    mObjects.push_back(new box(1,1,0));
    mObjects.push_back(new box(1,1,0));
    mObjects.push_back(new box(1,1,0));
    mObjects.push_back(new box(1,1,0));
    mObjects.at(3)->setName("pickup1");
    mObjects.at(4)->setName("pickup2");
    mObjects.at(5)->setName("pickup3");
    mObjects.at(6)->setName("pickup4");
    mObjects.at(7)->setName("pickup5");
    mObjects.at(8)->setName("pickup6");

    for(int i = 3; i < 8; i++)
    {
        mObjects.at(i)->setTag("pickup");
        if(i < 6)
        {
            mObjects.at(i)->move(20,1,5*i);
        }
        else{
            mObjects.at(i)->move(25,1,(i-3)*5);
        }
    }
    mObjects.at(8)->setTag("pickup");
    mObjects.at(8)->move(43,1,20);      // Last pickup, inside the house
    //Enemies
    mObjects.push_back(new box(1,0,0));
    mObjects.push_back(new box(1,0,0));

    mObjects.at(9)->setName("enemy1");
    mObjects.at(10)->setName("enemy2");
    mObjects.at(9)->setTag("enemy");
    mObjects.at(10)->setTag("enemy");

    mObjects.at(9)->move(28,1,15);
    mObjects.at(10)->move(15,1,20);

    //House

    mObjects.push_back(new box(0,0,1));
    mObjects.at(11)->setName("house");
    mObjects.at(11)->setTag("house");
    mObjects.at(11)->move(40,5,20);
    mObjects.at(11)->scale(5);
    mObjects.at(11)->radius = 3.6;

    //door
    mObjects.push_back(new wall(0,1,1));
    mObjects.at(12)->setName("door");
    mObjects.at(12)->setTag("wall");
    mObjects.at(12)->move(32,1,20);
    mObjects.at(12)->scale(3);
    mObjects.at(12)->radius = 2.6;

    //Roofless house for scene 2
    mObjects.push_back(new rooflessHouse(0,0,1));
    mObjects.at(13)->setName("rooflesshouse");
    mObjects.at(13)->setTag("rooflesshouse");
    mObjects.at(13)->move(40,5,20);
    mObjects.at(13)->scale(5);
    mObjects.at(13)->radius = 3.6;



    /******************************************/

    //Legger inn i map
    for(auto it = mObjects.begin();it!=mObjects.end();it++)
    {
        mMap.insert(std::pair<std::string,VisualObject*>{(*it)->getName(),*it} );
    }

      mCamera.setPosition(QVector3D(-12.5,-40,-30)); // Camera is -40 away from origo
      mCamera.pitch(65);


      mVulkanWindow = dynamic_cast<VulkanWindow*>(w);
}

void RenderWindow::initResources()
{
    qDebug("\n ***************************** initResources ******************************************* \n");

    VkDevice logicalDevice = mWindow->device();
    mDeviceFunctions = mWindow->vulkanInstance()->deviceFunctions(logicalDevice);

    /* Prepare the vertex and uniform data.The vertex data will never
    change so one buffer is sufficient regardless of the value of
    QVulkanWindow::CONCURRENT_FRAME_COUNT. */

    // const int concurrentFrameCount = mWindow->concurrentFrameCount(); // 2 on Oles Machine
    const VkPhysicalDeviceLimits *pdevLimits = &mWindow->physicalDeviceProperties()->limits;
    const VkDeviceSize uniAlign = pdevLimits->minUniformBufferOffsetAlignment;
    qDebug("uniform buffer offset alignment is %u", (uint)uniAlign); //64 on Oles machine

    /// Dag 240125: Create correct buffers for all objects in mObjects with createBuffer() function
    for (auto it=mObjects.begin(); it!=mObjects.end(); it++)
    {
        createBuffer(logicalDevice, uniAlign, *it);
    }

    /********************************* Vertex layout: *********************************/
    VkVertexInputBindingDescription vertexBindingDesc{};    //Updated to a more common way to write it
    vertexBindingDesc.binding = 0;
    vertexBindingDesc.stride = sizeof(vertex);
    vertexBindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;      //always this when not using instanced data

    /********************************* Shader bindings: *********************************/
    //Descritpion of the attributes used for vertices in the shader
    VkVertexInputAttributeDescription vertexAttrDesc[2];    //Updated to a more common way to write it
    vertexAttrDesc[0].location = 0;
    vertexAttrDesc[0].binding = 0;
    vertexAttrDesc[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    vertexAttrDesc[0].offset = 0;

    vertexAttrDesc[1].location = 1;
    vertexAttrDesc[1].binding = 0;
    vertexAttrDesc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    vertexAttrDesc[1].offset = 3 * sizeof(float);

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};	    // C++11: {} is the same as memset(&bufferInfo, 0, sizeof(bufferInfo));
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.pNext = nullptr;
    vertexInputInfo.flags = 0;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &vertexBindingDesc;
    vertexInputInfo.vertexAttributeDescriptionCount = 2; // position and color
    vertexInputInfo.pVertexAttributeDescriptions = vertexAttrDesc;
    /***********/

    // Pipeline cache - supposed to increase performance
    VkPipelineCacheCreateInfo pipelineCacheInfo{};
    pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    VkResult result = mDeviceFunctions->vkCreatePipelineCache(logicalDevice, &pipelineCacheInfo, nullptr, &mPipelineCache);
    if (result != VK_SUCCESS)
        qFatal("Failed to create pipeline cache: %d", result);

    // Pipeline layout
    // Set up the push constant info
    VkPushConstantRange pushConstantRange{};    //Updated to more common way to write it
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; // | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = 19 * sizeof(float); // 16 floats for the model matrix + 3 for color

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pushConstantRangeCount = 1;  // OEF: PushConstants update
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange; // OEF: PushConstants update
    result = mDeviceFunctions->vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &mPipelineLayout);
    if (result != VK_SUCCESS)
        qFatal("Failed to create pipeline layout: %d", result);

    /********************************* Create shaders *********************************/
    //Creates our actual shader modules
    VkShaderModule vertShaderModule = createShader(QStringLiteral(":/color_vert.spv"));
    VkShaderModule fragShaderModule = createShader(QStringLiteral(":/color_frag.spv"));

    //Updated to more common way to write it:
    VkPipelineShaderStageCreateInfo vertShaderCreateInfo{};
    vertShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderCreateInfo.module = vertShaderModule;
    vertShaderCreateInfo.pName = "main";                // start function in shader

    VkPipelineShaderStageCreateInfo fragShaderCreateInfo{};
    fragShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderCreateInfo.module = fragShaderModule;
    fragShaderCreateInfo.pName = "main";                // start function in shader

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderCreateInfo, fragShaderCreateInfo };

    /*********************** Graphics pipeline ********************************/
    VkGraphicsPipelineCreateInfo pipelineInfo{};    //Will use this variable a lot in the next 100s of lines
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2; //vertex and fragment shader
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;

    // The viewport and scissor will be set dynamically via vkCmdSetViewport/Scissor in setRenderPassParameters().
    // This way the pipeline does not need to be touched when resizing the window.
    VkPipelineViewportStateCreateInfo viewport{};
    viewport.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport.viewportCount = 1;
    viewport.scissorCount = 1;
    pipelineInfo.pViewportState = &viewport;

    // **** Input Assembly **** - describes how primitives are assembled in the Graphics pipeline
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;       //Draw triangles
    inputAssembly.primitiveRestartEnable = VK_FALSE;                    //Allow strips to be connected, not used in TriangleList
    pipelineInfo.pInputAssemblyState = &inputAssembly;

    // **** Rasterizer **** - takes the geometry and turns it into fragments
    VkPipelineRasterizationStateCreateInfo rasterization{};
    rasterization.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterization.polygonMode = VK_POLYGON_MODE_FILL;           // VK_POLYGON_MODE_LINE will make a wireframe;
    rasterization.cullMode = VK_CULL_MODE_NONE;                 // VK_CULL_MODE_BACK_BIT will cull backsides
    rasterization.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;  // Front face is counter clockwise - could be clockwise with VK_FRONT_FACE_CLOCKWISE
    rasterization.lineWidth = 1.0f;                             // Not important for VK_POLYGON_MODE_FILL
    pipelineInfo.pRasterizationState = &rasterization;

    // Enable multisampling
    VkPipelineMultisampleStateCreateInfo multisample{};
    multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample.rasterizationSamples = mWindow->sampleCountFlagBits();
    pipelineInfo.pMultisampleState = &multisample;

    // **** Color Blending **** -
    // how to blend the color of a fragment that is already in the framebuffer with the color of the fragment being added

    VkPipelineColorBlendAttachmentState colorBlendAttachment{}; // Need this struct for ColorBlending CreateInfo
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT
                                          | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;  // Colors to apply blending to - was hardcoded to 0xF;

    VkPipelineColorBlendStateCreateInfo colorBlend{};
    colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlend.attachmentCount = 1;                             // the one we made above
    colorBlend.pAttachments = &colorBlendAttachment;
    pipelineInfo.pColorBlendState = &colorBlend;                // no blending for now, write out all of rgba

    // **** Depth Stencil ****
    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    pipelineInfo.pDepthStencilState = &depthStencil;

    // **** Dynamic State **** - dynamic states can be changed without recreating the pipeline
    VkDynamicState dynamicEnable[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    VkPipelineDynamicStateCreateInfo dynamic{};
    dynamic.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic.dynamicStateCount = sizeof(dynamicEnable) / sizeof(VkDynamicState);
    dynamic.pDynamicStates = dynamicEnable;
    pipelineInfo.pDynamicState = &dynamic;

    pipelineInfo.layout = mPipelineLayout;
    pipelineInfo.renderPass = mWindow->defaultRenderPass();

    result = mDeviceFunctions->vkCreateGraphicsPipelines(logicalDevice, mPipelineCache, 1, &pipelineInfo, nullptr, &mPipeline1);
    if (result != VK_SUCCESS)
        qFatal("Failed to create graphics pipeline: %d", result);

    //Making a pipeline for drawing lines
    mPipeline2 = mPipeline1;                                    // reusing most of the settings from the first pipeline
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;   // draw lines
    rasterization.polygonMode = VK_POLYGON_MODE_FILL;           // VK_POLYGON_MODE_LINE will make a wireframe; VK_POLYGON_MODE_FILL
    rasterization.lineWidth = 5.0f;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    result = mDeviceFunctions->vkCreateGraphicsPipelines(logicalDevice, mPipelineCache, 1, &pipelineInfo, nullptr, &mPipeline2);
    if (result != VK_SUCCESS)
        qFatal("Failed to create graphics pipeline: %d", result);


    // Destroying the shader modules, we won't need them anymore after the pipeline is created
    if (vertShaderModule)
        mDeviceFunctions->vkDestroyShaderModule(logicalDevice, vertShaderModule, nullptr);
    if (fragShaderModule)
        mDeviceFunctions->vkDestroyShaderModule(logicalDevice, fragShaderModule, nullptr);

    getVulkanHWInfo(); // if you want to get info about the Vulkan hardware
}

void RenderWindow::initSwapChainResources()
{
    qDebug("\n ***************************** initSwapChainResources ******************************************* \n");

    // Projection matrix - how the scene will be projected into the render window

    //find the size of the window
    const QSize sz = mWindow->swapChainImageSize();
    mCamera.perspective(25.0f,sz.width()/(float)sz.height(),0.01f,100.0f);





}

void RenderWindow::startNextFrame()
{
    //OEF: Handeling input from keyboard and mouse is done in VulkanWindow
    //Has to be done each frame to get smooth movement
    mVulkanWindow->handleInput();
    mCamera.update();               //input can have moved the camera
    //qDebug("%f %f %f %f", mCamera.getPosition().x(),mCamera.getPosition().y(),mCamera.getPosition().z(), mCamera.getPitch());
    //collision detection
    onCollision(mPlayer);
    onCollisionEnd(mPlayer);

    /* NPC patrolling*/
    if(patrolRoute == 0)
    {

        if(patrolCounter != 100)
        {
            mObjects.at(9)->move(0,0,.1);
            mObjects.at(10)->move(0,0,.1);
            patrolCounter++;
        }else{
            patrolCounter = 0;
            patrolRoute = 1;
        }
    }
    if(patrolRoute == 1)
    {

        if(patrolCounter != 100)
        {
            mObjects.at(9)->move(0,0,-.1);
            mObjects.at(10)->move(0,0,-.1);
            patrolCounter++;
        }else{
            patrolCounter = 0;
            patrolRoute = 0;
        }
    }



    VkCommandBuffer commandBuffer = mWindow->currentCommandBuffer();

    setRenderPassParameters(commandBuffer);

    VkDeviceSize vbOffset{ 0 };     //Offsets into buffer being bound

    /********************************* Our draw call!: *********************************/
    for (std::vector<VisualObject*>::iterator it=mObjects.begin(); it!=mObjects.end(); it++)
    {
        if ((*it)->drawType == 0)
            mDeviceFunctions->vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline1);
        else
            mDeviceFunctions->vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline2);

        mDeviceFunctions->vkCmdBindVertexBuffers(commandBuffer, 0, 1, &(*it)->mBuffer, &vbOffset);
        pushConstants(mCamera.cMatrix() * (*it)->mMatrix, (*it)->mColor);
        mDeviceFunctions->vkCmdDraw(commandBuffer, (*it)->mVertices.size(), 1, 0, 0);
    }
    /***************************************/

    mDeviceFunctions->vkCmdEndRenderPass(commandBuffer);

    //mObjects.at(1)->rotate(1.0f, 0.0f, 0.0f, 1.0f);

    mWindow->frameReady();
    mWindow->requestUpdate(); // render continuously, throttled by the presentation rate
}

VkShaderModule RenderWindow::createShader(const QString &name)
{
    //This uses Qt's own file opening and resource system
    //We probably will replace it with pure C++ when expanding the program
    QFile file(name);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Failed to read shader %s", qPrintable(name));
        return VK_NULL_HANDLE;
    }
    QByteArray blob = file.readAll();
    file.close();

    VkShaderModuleCreateInfo shaderInfo;
    memset(&shaderInfo, 0, sizeof(shaderInfo));
    shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderInfo.codeSize = blob.size();
    shaderInfo.pCode = reinterpret_cast<const uint32_t *>(blob.constData());
    VkShaderModule shaderModule;
    VkResult err = mDeviceFunctions->vkCreateShaderModule(mWindow->device(), &shaderInfo, nullptr, &shaderModule);
    if (err != VK_SUCCESS) {
        qWarning("Failed to create shader module: %d", err);
        return VK_NULL_HANDLE;
    }

    return shaderModule;
}

void RenderWindow::setModelMatrix(QMatrix4x4 modelMatrix)
{

	mDeviceFunctions->vkCmdPushConstants(mWindow->currentCommandBuffer(), mPipelineLayout, 
        VK_SHADER_STAGE_VERTEX_BIT, 0, 16 * sizeof(float), modelMatrix.constData());
}

void RenderWindow::getVulkanHWInfo()
{
    qDebug("\n ***************************** Vulkan Hardware Info ******************************************* \n");
    QVulkanInstance *inst = mWindow->vulkanInstance();
    mDeviceFunctions = inst->deviceFunctions(mWindow->device());

    QString info;
    info += QString::asprintf("Number of physical devices: %d\n", int(mWindow->availablePhysicalDevices().count()));

    QVulkanFunctions *f = inst->functions();
    VkPhysicalDeviceProperties props;
    f->vkGetPhysicalDeviceProperties(mWindow->physicalDevice(), &props);
    info += QString::asprintf("Active physical device name: '%s' version %d.%d.%d\nAPI version %d.%d.%d\n",
                              props.deviceName,
                              VK_VERSION_MAJOR(props.driverVersion), VK_VERSION_MINOR(props.driverVersion),
                              VK_VERSION_PATCH(props.driverVersion),
                              VK_VERSION_MAJOR(props.apiVersion), VK_VERSION_MINOR(props.apiVersion),
                              VK_VERSION_PATCH(props.apiVersion));

    info += QStringLiteral("Supported instance layers:\n");
    for (const QVulkanLayer &layer : inst->supportedLayers())
        info += QString::asprintf("    %s v%u\n", layer.name.constData(), layer.version);
    info += QStringLiteral("Enabled instance layers:\n");
    for (const QByteArray &layer : inst->layers())
        info += QString::asprintf("    %s\n", layer.constData());

    info += QStringLiteral("Supported instance extensions:\n");
    for (const QVulkanExtension &ext : inst->supportedExtensions())
        info += QString::asprintf("    %s v%u\n", ext.name.constData(), ext.version);
    info += QStringLiteral("Enabled instance extensions:\n");
    for (const QByteArray &ext : inst->extensions())
        info += QString::asprintf("    %s\n", ext.constData());

    info += QString::asprintf("Color format: %u\nDepth-stencil format: %u\n",
                              mWindow->colorFormat(), mWindow->depthStencilFormat());

    info += QStringLiteral("Supported sample counts:");
    const QList<int> sampleCounts = mWindow->supportedSampleCounts();
    for (int count : sampleCounts)
        info += QLatin1Char(' ') + QString::number(count);
    info += QLatin1Char('\n');

    qDebug(info.toUtf8().constData());
    qDebug("\n ***************************** Vulkan Hardware Info finished ******************************************* \n");
}

void RenderWindow::releaseSwapChainResources()
{
    qDebug("\n ***************************** releaseSwapChainResources ******************************************* \n");
}

void RenderWindow::releaseResources()
{
    qDebug("\n ***************************** releaseResources ******************************************* \n");

    VkDevice dev = mWindow->device();

    if (mPipeline1) {
        mDeviceFunctions->vkDestroyPipeline(dev, mPipeline1, nullptr);
        mPipeline1 = VK_NULL_HANDLE;
    }

    if (mPipeline2) {
        mDeviceFunctions->vkDestroyPipeline(dev, mPipeline2, nullptr);
        mPipeline2 = VK_NULL_HANDLE;
    }

    if (mPipelineLayout) {
        mDeviceFunctions->vkDestroyPipelineLayout(dev, mPipelineLayout, nullptr);
        mPipelineLayout = VK_NULL_HANDLE;
    }

    if (mPipelineCache) {
        mDeviceFunctions->vkDestroyPipelineCache(dev, mPipelineCache, nullptr);
        mPipelineCache = VK_NULL_HANDLE;
    }

    if (mDescriptorSetLayout) {
        mDeviceFunctions->vkDestroyDescriptorSetLayout(dev, mDescriptorSetLayout, nullptr);
        mDescriptorSetLayout = VK_NULL_HANDLE;
    }

    if (mDescriptorPool) {
        mDeviceFunctions->vkDestroyDescriptorPool(dev, mDescriptorPool, nullptr);
        mDescriptorPool = VK_NULL_HANDLE;
    }

    if (mBuffer) {
        mDeviceFunctions->vkDestroyBuffer(dev, mBuffer, nullptr);
        mBuffer = VK_NULL_HANDLE;
    }

    if (mBufferMemory) {
        mDeviceFunctions->vkFreeMemory(dev, mBufferMemory, nullptr);
        mBufferMemory = VK_NULL_HANDLE;
    }

    // Samme for alle objekter i container
    for (auto it=mObjects.begin(); it!=mObjects.end(); it++) {
        if ((*it)->mBuffer) {
            mDeviceFunctions->vkDestroyBuffer(dev, (*it)->mBuffer, nullptr);
            (*it)->mBuffer = VK_NULL_HANDLE;
        }
    }
    for (auto it=mObjects.begin(); it!=mObjects.end(); it++) {
        if ((*it)->mBufferMemory) {
            mDeviceFunctions->vkFreeMemory(dev, (*it)->mBufferMemory, nullptr);
            (*it)->mBuffer = VK_NULL_HANDLE;
        }
    }
}

void RenderWindow::createBuffer(VkDevice logicalDevice,
                                const VkDeviceSize uniAlign,
                                VisualObject* visualObject,
                                VkBufferUsageFlags usage)
{
    VkBufferCreateInfo bufferInfo{};
    memset(&bufferInfo, 0, sizeof(bufferInfo)); //clear out the memory
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO; // set the structure type

    // Layout is just the vertex data
    // start offset aligned to unialign
        VkDeviceSize vertexAllocSize = aligned(visualObject->getVertices().size()*sizeof(vertex), uniAlign);
    bufferInfo.size = vertexAllocSize; //One vertex buffer (we don't use uniform buffer in this example)
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT; //Set the usage vertex buffer(not using uniform buffer in this example)

    VkResult err = mDeviceFunctions->vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &visualObject->mBuffer);
    if(err != VK_SUCCESS)
        qFatal("Failed to vreate buffer: %d", err);

    VkMemoryRequirements memReq;
    mDeviceFunctions->vkGetBufferMemoryRequirements(logicalDevice, visualObject->mBuffer, &memReq);

    VkMemoryAllocateInfo memAllocInfo = {
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        nullptr,
        memReq.size,
        mWindow->hostVisibleMemoryIndex()
    };

    err = mDeviceFunctions->vkAllocateMemory(logicalDevice, &memAllocInfo,
                                             nullptr, &visualObject->mBufferMemory);
    if(err != VK_SUCCESS)
        qFatal("Failed to allocate memory: %d", err);

    err = mDeviceFunctions->vkBindBufferMemory(logicalDevice, visualObject ->mBuffer, visualObject->mBufferMemory,0);
    if (err != VK_SUCCESS)
        qFatal("Failed to bind buffer memory: %d err");


    quint8* p{nullptr};
    err = mDeviceFunctions->vkMapMemory(logicalDevice, visualObject->mBufferMemory, 0, memReq.size, 0, reinterpret_cast<void **>(&p));
    if (err != VK_SUCCESS)
        qFatal("Failed to map memory: %d", err);
    // Dag 170125
    // memcpy(p, vertexData, sizeof(vertexData));
    memcpy(p, visualObject->getVertices().data(), visualObject->getVertices().size()*sizeof(vertex));


    mDeviceFunctions->vkUnmapMemory(logicalDevice, visualObject->mBufferMemory);

}

void RenderWindow::pushConstants(QMatrix4x4 modelMatrix, QVector3D color)
{
    float tempArray[19]{};
    QMatrix4x4 tempMatrix = modelMatrix.transposed();
    tempMatrix.copyDataTo(tempArray);
    tempArray[16] = color.x();
    tempArray[17] = color.y();
    tempArray[18] = color.z();
    mDeviceFunctions->vkCmdPushConstants(mWindow->currentCommandBuffer(), mPipelineLayout,
                                         VK_SHADER_STAGE_VERTEX_BIT, 0, 19 * sizeof(float), tempArray);
}

void RenderWindow::setRenderPassParameters(VkCommandBuffer commandBuffer)
{
    const QSize swapChainImageSize = mWindow->swapChainImageSize();

    //Backtgound color of the render window - dark grey
    VkClearColorValue clearColor = { { 0.3, 0.3, 0.3, 1 } };

    VkClearDepthStencilValue clearDepthStencil = { 1, 0 };
    VkClearValue clearValues[3]{};  //C++11 {} works even on arrays!
    clearValues[0].color = clearValues[2].color = clearColor;
    clearValues[1].depthStencil = clearDepthStencil;

    VkRenderPassBeginInfo renderPassBeginInfo{};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = mWindow->defaultRenderPass();
    renderPassBeginInfo.framebuffer = mWindow->currentFramebuffer();
    renderPassBeginInfo.renderArea.extent.width = swapChainImageSize.width();
    renderPassBeginInfo.renderArea.extent.height = swapChainImageSize.height();
    renderPassBeginInfo.clearValueCount = mWindow->sampleCountFlagBits() > VK_SAMPLE_COUNT_1_BIT ? 3 : 2;
    renderPassBeginInfo.pClearValues = clearValues;
    mDeviceFunctions->vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    //Viewport - area of the image to render to, usually (0,0) to (width, height)
    VkViewport viewport{};
    viewport.x = viewport.y = 0.f;
    viewport.width = swapChainImageSize.width();
    viewport.height = swapChainImageSize.height();
    viewport.minDepth = 0.f;                //min framebuffer depth
    viewport.maxDepth = 1.f;                //max framebuffer depth
    mDeviceFunctions->vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    //Scissor - area to draw in the target frame buffer
    VkRect2D scissor{};
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width = viewport.width;
    scissor.extent.height = viewport.height;
    mDeviceFunctions->vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

bool RenderWindow::overlapDetection(VisualObject* obj1, VisualObject* obj2) const
{
    float distBetweenObj = sqrt(
                                std::pow(obj1->getPosition().x() - obj2->getPosition().x(),2) +
                                std::pow(obj1->getPosition().y() - obj2->getPosition().y(),2) +
                                std::pow(obj1->getPosition().z() - obj2->getPosition().z(),2)
                            );

    return distBetweenObj <= obj1->radius + obj2->radius;

}

void RenderWindow::onCollision(VisualObject* obj)
{
    for(VisualObject* j : mObjects)
    {
        if(&j != &mPlayer && overlapDetection(obj,j) && j->enableCollision)
        {
            if(j->getTag() == "pickup")
            {
                mPickupsCollected++;
                j->move(0,-20,0);
                qDebug("%i out of 6 pickups collected",mPickupsCollected);

                if(mPickupsCollected == 6)
                    qDebug("You won!");
            }

            if(j->getTag() == "enemy")
            {
                if(!gameOver)
                {
                    gameOver = true;
                    mVulkanWindow->setObjectMovementSpeed(0.0f);
                    qDebug("You lost");

                }
            }

            if(j->getTag() == "house" && mIsHouse == true)
            {
                if (mObjects.at(12)->isOpen == false)
                {
                    mPlayer->move(mPlayer->radius,0,mPlayer->radius);
                }else{
                //Switches out the house with another one and moves the camera
                mCamera.setPosition(QVector3D(-20.3f,-11.4f,-14.0f));
                j->move(0,-10,0);
                qDebug("Collided with the house");
                mIsHouse = false;}
            }
            if(j->getTag() == "wall" && j->isOpen == false)
            {
                j->rotate(-90,0,1,0);
                j->isOpen = true;
                qDebug("Collided with the wall");
            }

        }
    }
}

void RenderWindow::onCollisionEnd(VisualObject* obj)
{
    for(VisualObject* j : mObjects)
    {
        if(&j != &mPlayer && !(overlapDetection(obj,j)) && j->enableCollision)
        {

            if(j->getTag() == "wall" && j->isOpen == true)
            {
                j->rotate(90,0,1,0);
                j->isOpen = false;
                qDebug("stopped colliding with the wall");
            }
            if(j->getTag() =="rooflesshouse" && mIsHouse == false)
            {
                //Switches out the house with another one and moves the camera
                mCamera.setPosition(QVector3D(-12.5,-40,-30));
                mObjects.at(11)->move(0,10,0);
                qDebug("Collided with the house");
                mIsHouse = true;
            }
            if(j->getTag() == "house" && mIsHouse == true)
            {
                if(mVulkanWindow->getObjectMovementSpeed() != 0.25f && !gameOver)
                    mVulkanWindow->setObjectMovementSpeed(0.25f);
            }
        }
    }
}

VisualObject* RenderWindow::getPlayer() const
{
    for(VisualObject* obj : mObjects)
    {
        if(obj->getTag() == "player")
            return obj;
        qDebug("No object with mTag 'player' found");
        return mPlayer; // default is nullptr
    }
}


