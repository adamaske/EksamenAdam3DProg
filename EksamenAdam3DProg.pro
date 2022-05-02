QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

SOURCES += main.cpp \
    aabbcollision.cpp \
    camera.cpp \
    collisionshape.cpp \
    cubecollision.cpp \
    enemy.cpp \
    logger.cpp \
    mainwindow.cpp \
    objmesh.cpp \
    oobbcollision.cpp \
    renderwindow.cpp \
    shader.cpp \
    spherecollision.cpp \
    terrain.cpp \
    texture.cpp \
    trophy.cpp \
    vector3d.cpp \
    vertex.cpp \
    visualobject.cpp \
    visualsun.cpp \
    xyz.cpp

HEADERS += \
    QuadTree.h \
    aabbcollision.h \
    camera.h \
    collisionshape.h \
    cubecollision.h \
    enemy.h \
    logger.h \
    mainwindow.h \
    objmesh.h \
    oobbcollision.h \
    renderwindow.h \
    shader.h \
    spherecollision.h \
    stb_image.h \
    terrain.h \
    texture.h \
    trophy.h \
    vector3d.h \
    vertex.h \
    visualobject.h \
    visualsun.h \
    xyz.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    billboardshader.frag \
    billboardshader.vert \
    heightshader.frag \
    heightshader.vert \
    lightshader.frag \
    lightshader.vert \
    plainshader.frag \
    plainshader.vert \
    textureshader.frag \
    textureshader.vert
