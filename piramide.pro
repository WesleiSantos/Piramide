QT += opengl widgets

TARGET = Piramide

TEMPLATE = app

SOURCES += \
        main.cpp \
        piramide.cpp

RESOURCES += res.qrc

HEADERS += \
        piramide.h

win32 {
    LIBS += \
            -lopengl32 \
            -lglu32
}

unix {
    LIBS += \
            -lGL \
            -lGLU
}

