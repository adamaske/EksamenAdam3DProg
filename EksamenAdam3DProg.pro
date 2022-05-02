QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

SOURCES += main.cpp \
    aabbcollision.cpp \
    camera.cpp \
    collisionshape.cpp \
    cube.cpp \
    cubecollision.cpp \
    enemy.cpp \
    light.cpp \
    logger.cpp \
    mainwindow.cpp \
    objmesh.cpp \
    oobbcollision.cpp \
    pointlight.cpp \
    renderwindow.cpp \
    shader.cpp \
    spherecollision.cpp \
    terrain.cpp \
    texture.cpp \
    trophy.cpp \
    vector3d.cpp \
    vertex.cpp \
    visualobject.cpp

HEADERS += \
    QuadTree.h \
    aabbcollision.h \
    camera.h \
    collisionshape.h \
    cube.h \
    cubecollision.h \
    enemy.h \
    light.h \
    logger.h \
    mainwindow.h \
    objmesh.h \
    oobbcollision.h \
    pointlight.h \
    renderwindow.h \
    shader.h \
    spherecollision.h \
    stb_image.h \
    terrain.h \
    texture.h \
    trophy.h \
    vector3d.h \
    vertex.h \
    visualobject.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    heightshader.frag \
    heightshader.vert \
    lightshader.frag \
    lightshader.vert \
    plainshader.frag \
    plainshader.vert \
    textureshader.frag \
    textureshader.vert
