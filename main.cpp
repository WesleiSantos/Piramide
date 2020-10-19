#include "piramide.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Check for OpenGL support
    if (!QGLFormat::hasOpenGL()) {
        qDebug("Your system does not seem to support OpenGL. Cannot run this example.");
        return EXIT_FAILURE;
    }
    Piramide w;
    w.resize(600,600);
    w.show();
    return a.exec();
}

