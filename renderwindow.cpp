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
#include "objmesh.h"
#include "texture.h"
#include "terrain.h"
#include "trophy.h"
#include "enemy.h"
#include "visualsun.h"
#include "xyz.h"
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

    //Creating shaders
    mShaders.insert(std::pair<std::string, Shader*>{"PlainShader", new Shader("../EksamenAdam3DProg/plainshader.vert",
                                                                                 "../EksamenAdam3DProg/plainshader.frag")});
    mShaders.insert(std::pair<std::string, Shader*>{"TextureShader", new Shader("../EksamenAdam3DProg/textureshader.vert",
                                                                                "../EksamenAdam3DProg/textureshader.frag")});
    mShaders.insert(std::pair<std::string, Shader*>{"LightShader", new Shader("../EksamenAdam3DProg/lightshader.vert",
                                                                                "../EksamenAdam3DProg/lightshader.frag")});
    mShaders.insert(std::pair<std::string, Shader*>{"HeightShader", new Shader("../EksamenAdam3DProg/heightshader.vert",
                                                                                "../EksamenAdam3DProg/heightshader.frag")});
    mShaders.insert(std::pair<std::string, Shader*>{"BillboardShader", new Shader("../EksamenAdam3DProg/billboardshader.vert",
                                                                                "../EksamenAdam3DProg/billboardshader.frag")});
    //Create textures
    mTextures.insert(std::pair<std::string, Texture*>{"Plain", new Texture()});
    mTextures.insert(std::pair<std::string, Texture*>{"Hund", new Texture("../EksamenAdam3DProg/hund.bmp")});
    mTextures.insert(std::pair<std::string, Texture*>{"Hammer", new Texture("../EksamenAdam3DProg/hammer.bmp")});
    mTextures.insert(std::pair<std::string, Texture*>{"Grass", new Texture("../EksamenAdam3DProg/GrassTekstur.bmp")});

    //Oppgave 2
    //Init terrenget med phongshaderen og GrassTekstur
    mTerrain = new Terrain(*mShaders["LightShader"], mTextures["Grass"], ObjectState::STATIC);
    mMap.insert(std::pair<std::string, VisualObject*>{"Terrain", mTerrain});

    //Oppgave 3
    mSun = new VisualSun("../EksamenAdam3DProg/Sun.obj", *mShaders["PlainShader"], ObjectState::STATIC);
    mMap.insert(std::pair<std::string, VisualObject*>{"Sun", mSun});

    //Oppgave 4
    //Lager player fra object.obj, gir phongshader og hund tekstur
    mMap.insert(std::pair<std::string, VisualObject*>{"Player",
                new ObjMesh("../EksamenAdam3DProg/object.obj", *mShaders["LightShader"], mTextures["Hund"], ObjectState::DYNAMIC)});
   mMap["Player"]->RotateRight(-90);
    mMap["Player"]->SetPosition(QVector3D(0, 0, 0));

    //Oppgave 5
    //lager kameraer
    mPlayCamera = new Camera();
    mEditorCamera = new Camera();

    //Oppgave 6
    //Lager aksen som vises i editor modus
    mXYZ = new XYZ(*mShaders["PlainShader"], ObjectState::STATIC);
    mXYZ->init();

    //Subdivide quadtree
    mQuadTree.subDivide(2);
    //init every object
    for (auto it = mMap.begin(); it != mMap.end(); it++) {
        //Adds all visual objects to the quadtree
        mQuadTree.insert((*it).second->getPosition2D(), (*it).first, (*it).second);
        (*it).second->init();
        (*it).second->UpdateTransform();
    }
    mEditorCamera->init();
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

    switch(mGameState){
    //
    case GameState::Editor :

        mEditorCamera->perspective(90, static_cast<float>(width()) / static_cast<float>(height()), 0.1, 3000.0);
        //Yaw til kamera med mus input her

    break;
        //Sett kamera bak spilleren
    case GameState::Play :
        mPlayCamera->init();
        mPlayCamera->perspective(90, static_cast<float>(width()) / static_cast<float>(height()), 0.1, 3000.0); // verticalAngle, aspectRatio, nearPlane,farPlane
        QVector3D PlayerPos = mMap["Player"]->GetPosition();
        //Set y to GetHeight from terrain
        mMap["Player"]->SetPosition(QVector3D(PlayerPos.x(), mTerrain->GetHeight(PlayerPos), PlayerPos.z()));
        PlayerPos = mMap["Player"]->GetPosition();
        mPlayCamera->lookAt(PlayerPos + QVector3D(0,5, -5), PlayerPos, QVector3D{ 0,1,0 });
    break;
    }


    //Oppgave 3
    //Adder vinkelen den er på
    mSun->angle += 0.01f;
    //Hent posisjonen
    QVector3D sunPos = mSun->GetPosition();
    //Cosinus vinkelen ganger radius for x
    sunPos.setX((cos(mSun->angle)*20));
    //Set til 20 meter over bakken
    sunPos.setY(20);
    //Sinus vinkelen ganger radius for z
    sunPos.setZ((sin(mSun->angle)*20));
    //Sett nye posisjonen
    mSun->SetPosition(sunPos);

    //Apply camera to all shaders
    for(auto it = mShaders.begin(); it != mShaders.end(); it++){
        (*it).second->use();
        //
        switch(mGameState){
        //Send editor kamera sin v og p matrix hvis spillet er i editor modus
        case GameState::Editor :
            //Send view and projection matrices to alle the shaders
            (*it).second->SetUniformMatrix4fv(*mEditorCamera->mVmatrix, "vMatrix");
            (*it).second->SetUniformMatrix4fv(*mEditorCamera->mPmatrix, "pMatrix");
            //glUnifor
            //The visual object sends its own modelMatrix to the shader so it dosent need to be done here
            if((*it).first == "LightShader"){
                //Give all lights the camera position
                (*it).second->SetUniform3f(mEditorCamera->GetPosition().x(), mEditorCamera->GetPosition().y(), mEditorCamera->GetPosition().y(),
                                           "cameraPosition");

            }
        break;
            //Bruk play kameraet
        case GameState::Play :
            //Send view and projection matrices to alle the shaders
            (*it).second->SetUniformMatrix4fv(*mPlayCamera->mVmatrix, "vMatrix");
            (*it).second->SetUniformMatrix4fv(*mPlayCamera->mPmatrix, "pMatrix");
            //glUnifor
            //The visual object sends its own modelMatrix to the shader so it dosent need to be done here
            if((*it).first == "LightShader"){
                //Give all lights the camera position
                (*it).second->SetUniform3f(mPlayCamera->GetPosition().x(), mPlayCamera->GetPosition().y(), mPlayCamera->GetPosition().y(),
                                           "cameraPosition");
            }
        break;
        }
        //Set light posistion
        (*it).second->SetUniform3f(mMap["Sun"]->GetPosition().x(), mMap["Sun"]->GetPosition().y(), mMap["Sun"]->GetPosition().z(),
                                    "lightPosition");
    }

    //Draw all objects
    for (auto it = mMap.begin(); it != mMap.end(); it++) {
        //Set the shader matrixes from camera
        (*it).second->UpdateTransform();
        (*it).second->draw();

        switch(mGameState){
            case GameState::Editor :
            //Oppgave 6
            mXYZ->draw();
        break;
            case GameState::Play :

        break;
        }
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

    if (event->key() == Qt::Key_W)
    {
        switch(mGameState){
        case GameState::Editor :
            //Beveg kamera fremover            
            //mEditorCamera->MoveForward(100);
            mEditorCamera->translate(0,0, 1);
        break;
        case GameState::Play :
            //beveg hunden fremover
            mMap["Player"]->MoveForward(-1);
        break;
        }
    }
    if (event->key() == Qt::Key_S)
    {
        switch(mGameState){
        case GameState::Editor :
        //beveg kamera bakover       
            //mEditorCamera->MoveForward(-100);
            mEditorCamera->translate(0,0, -1);
        break;
        case GameState::Play :
        //beveg hunden bakover
            mMap["Player"]->MoveForward(1);
        break;
        }
    }
    if (event->key() == Qt::Key_A)
    {
        switch(mGameState){
        case GameState::Editor :
        //  Roter kamera til venstre
            mEditorCamera->MoveRight(1);
        break;
        case GameState::Play :
        //Roter hunden til venstre
            mMap["Player"]->RotateRight(1);
        break;
        }
    }
    if (event->key() == Qt::Key_D)
    {
        switch(mGameState){
        case GameState::Editor :
        //Roter kamera til høyre
            mEditorCamera->MoveRight(-1);
        break;
        case GameState::Play :
        //Roter hunden til høyre
            mMap["Player"]->RotateRight(-1);
        break;
        }
    }
    if (event->key() == Qt::Key_Q)
    {
        switch(mGameState){
        case GameState::Editor :
        //Beveg kamera oppover
            mEditorCamera->translate(0, 1, 0);
        break;
        case GameState::Play :
        break;
        }
    }
    if (event->key() == Qt::Key_E)
    {
        switch(mGameState){
        case GameState::Editor :
        //beveg kamera bakover
            mEditorCamera->translate(0, -1, 0);
        break;
        case GameState::Play :
        break;
        }
    }
    if (event->key() == Qt::Key_Tab)
    {
        if(mGameState == GameState::Editor){

            mGameState = GameState::Play;
        }else{
            mGameState = GameState::Editor;
        }
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

void RenderWindow::SwapGameMode(){
    if(mGameState == GameState::Editor){
        qDebug() << "In play mode";
        mGameState = GameState::Play;
    }else{
        qDebug() << "In editor mode";
        mGameState = GameState::Editor;
    }
}
