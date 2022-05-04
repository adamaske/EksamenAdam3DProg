QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

SOURCES += main.cpp \
    beziercurve.cpp \
    bomb.cpp \
    camera.cpp \
    collisionshape.cpp \
    enemy.cpp \
    logger.cpp \
    mainwindow.cpp \
    objmesh.cpp \
    renderwindow.cpp \
    shader.cpp \
    terrain.cpp \
    texture.cpp \
    trophy.cpp \
    vector3d.cpp \
    vertex.cpp \
    visualobject.cpp \
    visualpoint.cpp \
    visualsun.cpp \
    xyz.cpp

HEADERS += \
    QuadTree.h \
    beziercurve.h \
    bomb.h \
    camera.h \
    collisionshape.h \
    enemy.h \
    logger.h \
    mainwindow.h \
    objmesh.h \
    renderwindow.h \
    shader.h \
    stb_image.h \
    terrain.h \
    texture.h \
    trophy.h \
    vector3d.h \
    vertex.h \
    visualobject.h \
    visualpoint.h \
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
