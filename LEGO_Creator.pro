SOURCES += \
    main.cpp \
    MainWindow.cpp \
    ViewerWidget.cpp \
    Lego.cpp \
    LegoNode.cpp

HEADERS += \
    MainWindow.h \
    ViewerWidget.h \
    Lego.h \
    LegoNode.h

LIBS += \
    -L$OSG/lib \
    -losgQt \
    -losg \
    -losgGA \
    -losgDB \
    -losgViewer \
    -losgTerrain \
    -L

QT += opengl

CONFIG += thread
