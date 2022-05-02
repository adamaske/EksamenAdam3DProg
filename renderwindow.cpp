#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>
#include <string>

#include "shader.h"
#include "mainwindow.h"
#include "logger.h"
#include "visualobject.h"
#include "camera.h"
#include "cube.h"
#include "objmesh.h"
#include "texture.h"
#include "light.h"
#include "pointlight.h"
#include "terrain.h"
#include "trophy.h"
#include "enemy.h"
RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow), mQuadTree(Point2D(50,50), Point2D(-50,50), Point2D(50,-50), Point2D(-50,-50))

{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);
}

RenderWindow::~RenderWindow()
{
    //cleans up the GPU memory
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init()
{
    //Get the instance of the utility Output logger
    //Have to do this, else program will crash (or you have to put in nullptr tests...)
    mLogger = Logger::getInstance();

    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));
    //********************** General OpenGL stuff **********************

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        mLogger->logText("makeCurrent() failed", LogType::REALERROR);
        return;
    }

    //just to make sure we don't init several times
    //used in exposeEvent()
    if (!mInitialized)
        mInitialized = true;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Print render version info (what GPU is used):
    //Nice to see if you use the Intel GPU or the dedicated GPU on your laptop
    // - can be deleted
    mLogger->logText("The active GPU and API:", LogType::HIGHLIGHT);
    std::string tempString;
    tempString += std::string("  Vendor: ") + std::string((char*)glGetString(GL_VENDOR)) + "\n" +
            std::string("  Renderer: ") + std::string((char*)glGetString(GL_RENDERER)) + "\n" +
            std::string("  Version: ") + std::string((char*)glGetString(GL_VERSION));
    mLogger->logText(tempString);

    //Print info about opengl texture limits on this GPU:
    int textureUnits;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &textureUnits);
    tempString += std::string("  This GPU as ") + std::to_string(textureUnits) + std::string(" texture units / slots in total, \n");

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
    tempString += std::string("  and supports ") + std::to_string(textureUnits) + std::string(" texture units pr shader");

    mLogger->logText(tempString);

    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    //    glEnable(GL_CULL_FACE);       //draws only front side of models - usually what you want - test it out!
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);    //gray color used in glClear GL_COLOR_BUFFER_BIT


    mShaders.insert(std::pair<std::string, Shader*>{"PlainShader", new Shader("../3Dprog22/plainshader.vert",
                                                                                 "../3Dprog22/plainshader.frag")});
    mShaders.insert(std::pair<std::string, Shader*>{"TextureShader", new Shader("../3Dprog22/textureshader.vert",
                                                                                "../3Dprog22/textureshader.frag")});
    mShaders.insert(std::pair<std::string, Shader*>{"LightShader", new Shader("../3Dprog22/lightshader.vert",
                                                                                "../3Dprog22/lightshader.frag")});
    mShaders.insert(std::pair<std::string, Shader*>{"HeightShader", new Shader("../3Dprog22/heightshader.vert",
                                                                                "../3Dprog22/heightshader.frag")});
    //Set the material properties of the lightshader
    mShaders["LightShader"]->use();
    mShaders["LightShader"]->SetUniform1i(0, "material.diffuse");
    mShaders["LightShader"]->SetUniform1i(1, "material.specular");
    mShaders["LightShader"]->SetUniform1f(32.0f, "material.shininess");
    //Set directionallight properties of the lightshader
    mShaders["LightShader"]->SetUniform3f(0.2f, 1.0f, 0.3f, "dirLight.direction");
    mShaders["LightShader"]->SetUniform3f(0.1f, 0.1f, 0.1f, "dirLight.ambient" );
    mShaders["LightShader"]->SetUniform3f(0.04f, 0.04f, 0.04f, "dirLight.diffuse" );
    mShaders["LightShader"]->SetUniform3f( 0.5f, 0.5f, 0.5f, "dirLight.specular");

    //Create camera
    mCamera = new Camera();
    //creating objects to be drawn
    mMap.insert(std::pair<std::string, VisualObject*>{"Plane",
                new ObjMesh("../3Dprog22/plane.obj", *mShaders["LightShader"], new Texture("../3Dprog22/hammer.bmp"))});
    mMap.insert(std::pair<std::string, VisualObject*>{"Cube",
                new Cube(*mShaders["PlainShader"])});
    mMap.insert(std::pair<std::string, VisualObject*>{"Dog",
                new ObjMesh("../3Dprog22/object.obj", *mShaders["LightShader"], new Texture("../3Dprog22/hund.bmp"))});
    mTerrain = new Terrain(*mShaders["LightShader"]);
    mMap.insert(std::pair<std::string, VisualObject*>{"Terrain", mTerrain});
    //mMap["Terrain"]->SetPosition(QVector3D(0, -11, 0));

    mMap["Dog"]->SetRotation(QVector3D(0, -1, 0));
    mMap["Dog"]->SetPosition(QVector3D(0, 0, 0));

    mMap["Plane"]->SetPosition(QVector3D(0, 4, 20));
    mMap["Plane"]->SetRotation(QVector3D(1, 0, 0));
    mMap["Plane"]->SetScale(QVector3D(5, 5, 5));

    for(int i = 0; i < 10; i++){
        //Construct point light with light shader and with its index, so it call set it correctly in the draw function
        mMap.insert(std::pair<std::string, VisualObject*>{"PointLight " + std::to_string(i), new PointLight(*mShaders["LightShader"], new Texture(),mPointLights)});
        mMap["PointLight " + std::to_string(i)]->SetPosition(QVector3D(-30 + rand() % 60, rand() % 8, -30 + rand() % 60));
        mPointLights++;
    }
    //Set the lightshader.frag's pointLightUsed to the amount of point lights
    mShaders["LightShader"]->SetUniform1i(mPointLights, "pointLightsUsed");

    //Trophies
    for(int i = 0; i < 10; i++){
        //Creat trophies
        mMap.insert(std::pair<std::string, VisualObject*>{"Trophy " + std::to_string(i), new Trophy("../3Dprog22/trophy.obj", *mShaders["LightShader"], new Texture())});
        mMap["Trophy " + std::to_string(i)]->SetPosition(QVector3D(-20 + rand() % 40, 0, -20 + rand() % 40));
    }
    //Enemies
    for(int i = 0; i < 10; i++){
        //Creat trophies
        mMap.insert(std::pair<std::string, VisualObject*>{"Enemy " + std::to_string(i), new Enemy("../3Dprog22/enemy.obj", *mShaders["LightShader"], new Texture())});
        mMap["Enemy " + std::to_string(i)]->SetPosition(QVector3D(-100 + rand() % 200, 0, -100 + rand() % 200));
    }
    //Subdivide quadtree
    mQuadTree.subDivide(2);
    //init every object
    for (auto it = mMap.begin(); it != mMap.end(); it++) {
        //Adds all visual objects to the quadtree
        mQuadTree.insert((*it).second->getPosition2D(), (*it).first, (*it).second);
        (*it).second->init();
        (*it).second->UpdateTransform();
    }

    glBindVertexArray(0);       //unbinds any VertexArray - good practice
}

// Called each frame - doing the rendering!!!
void RenderWindow::render()
{
    calculateFramerate(); //display framerate
    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();

    //clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mCamera->init();

    mCamera->perspective(90, static_cast<float>(width()) / static_cast<float>(height()), 0.1, 3000.0); // verticalAngle, aspectRatio, nearPlane,farPlane
    QVector3D dogPos = mMap["Dog"]->GetPosition();
    //Set y to GetHeight from terrain
    mMap["Dog"]->SetPosition(QVector3D(dogPos.x(), mTerrain->GetHeight(dogPos), dogPos.z()));
    dogPos = mMap["Dog"]->GetPosition();
    mCamera->lookAt(dogPos + QVector3D(0,5, -5), dogPos, QVector3D{ 0,1,0 });

    //Apply camera to all shaders
    for(auto it = mShaders.begin(); it != mShaders.end(); it++){
        (*it).second->use();
        //Send view and projection matrices to alle the shaders
        (*it).second->SetUniformMatrix4fv(*mCamera->mVmatrix, "vMatrix");
        (*it).second->SetUniformMatrix4fv(*mCamera->mPmatrix, "pMatrix");
        //glUnifor
        //The visual object sends its own modelMatrix to the shader so it dosent need to be done here
        if((*it).first == "LightShader"){
            //Give all lights the camera position
            (*it).second->SetUniform3f(mCamera->GetPosition().x(), mCamera->GetPosition().y(), mCamera->GetPosition().y(),
                                       "cameraPosition");
        }

    }

    //Draw all objects
    for (auto it = mMap.begin(); it != mMap.end(); it++) {
        //Set the shader matrixes from camera
        (*it).second->UpdateTransform();
        (*it).second->draw();
    }

    //Qt require us to call this swapBuffers() -function.
    // swapInterval is 1 by default which means that swapBuffers() will (hopefully) block
    // and wait for vsync.
     mContext->swapBuffers(this);
}

//This function is called from Qt when window is exposed (shown)
// and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    //if not already initialized - run init() function - happens on program start up
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    const qreal retinaScale = devicePixelRatio();

    //Set viewport width and height to the size of the QWindow we have set up for OpenGL
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop == the render()-function
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
        mRenderTimer->start(16);
        mTimeStart.start();
    }
}

//The way this function is set up is that we start the clock before doing the draw call,
// and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)            //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30)    //once pr 30 frames = update the message == twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;     //reset to show a new message in 30 frames
        }
    }
}

//Uses QOpenGLDebugLogger if this is present
//Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)  //if our machine got this class to work
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
        {
            if (!(message.type() == message.OtherType)) // get rid of uninteresting "object ...
                                                        // will use VIDEO memory as the source for
                                                        // buffer object operations"
                // valid error message:
                mLogger->logText(message.message().toStdString(), LogType::REALERROR);
        }
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            mLogger->logText("glGetError returns " + std::to_string(err), LogType::REALERROR);
            switch (err) {
            case 1280:
                mLogger->logText("GL_INVALID_ENUM - Given when an enumeration parameter is not a "
                                "legal enumeration for that function.");
                break;
            case 1281:
                mLogger->logText("GL_INVALID_VALUE - Given when a value parameter is not a legal "
                                "value for that function.");
                break;
            case 1282:
                mLogger->logText("GL_INVALID_OPERATION - Given when the set of state for a command "
                                "is not legal for the parameters given to that command. "
                                "It is also given for commands where combinations of parameters "
                                "define what the legal parameters are.");
                break;
            }
        }
    }
}

//Tries to start the extended OpenGL debugger that comes with Qt
//Usually works on Windows machines, but not on Mac...
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            mLogger->logText("This system can not use QOpenGLDebugLogger, so we revert to glGetError()",
                             LogType::HIGHLIGHT);

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            mLogger->logText("This system can log extended OpenGL errors", LogType::HIGHLIGHT);
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                mLogger->logText("Started Qt OpenGL debug logger");
        }
    }
}

//Event sent from Qt when program receives a keyPress
// NB - see renderwindow.h for signatures on keyRelease and mouse input
void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        mMainWindow->close();       //Shuts down the whole program
    }
    if (event->key() == Qt::Key_Space)
    {

    }
    if (event->key() == Qt::Key_W)
    {
        mMap["Dog"]->move(0,0,1.1f);       //Shuts down the whole program
    }
    if (event->key() == Qt::Key_S)
    {
        mMap["Dog"]->move(0,0,-1.1f);       //Shuts down the whole program
    }
    if (event->key() == Qt::Key_A)
    {
        mMap["Dog"]->move(1.1f, 0,0);       //Shuts down the whole program
    }
    if (event->key() == Qt::Key_D)
    {
        mMap["Dog"]->move(-1.1f, 0,0);       //Shuts down the whole program
    }
    if (event->key() == Qt::Key_Q)
    {
        mMap["Dog"]->move(0, 1,0);       //Shuts down the whole program
    }
    if (event->key() == Qt::Key_E)
    {
        mMap["Dog"]->move(0, -1,0);       //Shuts down the whole program
    }
    //You get the keyboard input like this
//    if(event->key() == Qt::Key_A)
//    {
//        mMainWindow->statusBar()->showMessage(" AAAA");
//    }
//    if(event->key() == Qt::Key_S)
//    {
//        mMainWindow->statusBar()->showMessage(" SSSS");
//    }
}
