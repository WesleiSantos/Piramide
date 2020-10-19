#include "piramide.h"
#include <QKeyEvent>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <QTimer>

GLint LAST_NVL=45;//ultimo nivel da pirâmide
GLfloat escala=1;
float yrot=0.0;
GLint DLid;
// Constructor
Piramide::Piramide() {
    setWindowTitle("Piramide");

    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()),this,SLOT(updateGL()));
}

// Empty destructor
Piramide::~Piramide() {}


// Initialize OpenGL
void Piramide::initializeGL() {
    glShadeModel(GL_SMOOTH); // Enable smooth shading
    qglClearColor(Qt::white); // Set the clear color to a black background

    glClearDepth(1.0f); // Depth buffer setup
    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glDepthFunc(GL_LEQUAL); // Set type of depth test

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really nice perspective calculations
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    DLid = createDL(); //cria lista
}

// This is called when the OpenGL window is resized
void Piramide::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // Reset projection matrix

    gluPerspective(45.0f, static_cast<GLfloat>(width)/height, 5.f, LAST_NVL*10); // Calculate aspect ratio

    // Especifica posição do observador e do alvo
    gluLookAt(0.f,0.f,LAST_NVL+100, 0,-40,0, 0,1,0);
}

// OpenGL painting code goes here
void Piramide::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glScalef(escala, escala, escala);
    escala=1;//para impede o aumento da escala no proximo frame
    // Desenha Ceu
    glPushMatrix();
    glLoadIdentity();
     glColor3f(0.2,0.8,0.9);
        glTranslatef(0,0,-100);
        glBegin(GL_QUADS);
            glVertex3f(-2000,1000,0);
            glVertex3f(2000,1000,0);
            glVertex3f(1000,-1000,0);
            glVertex3f(-1000,-1000,0);
        glEnd();
    glPopMatrix();
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glColor3f(0.5, 0.8, 0.3);
    //Desenha solo
    glBegin(GL_QUADS);
        glVertex3f(-200,-46,200);
        glVertex3f(-200,-46,-200);
        glVertex3f(200,-46,-200);
        glVertex3f(200,-46,200);
    glEnd();

    glCallList(DLid); //chama lista de exibição

    if ( yrot != 0.0 ) {//economizar processamento quando não tem mudança no angul da piramide
        timer->start(30);
    }
}

// Key handler
void Piramide::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Up:
        escala=1.1;
        break;
    case Qt::Key_Down:
        escala=0.91;
        break;
    case Qt::Key_Right:
        yrot += 1;
        if (yrot > 360)
            yrot = 0.0;
        break;
    case Qt::Key_Left:
        yrot -= 1;
        if (yrot > 360)
            yrot = 0.0;
        break;
    case Qt::Key_Escape:
        close(); // Quit on Escape
        break;
    case Qt::Key_F1:
        setWindowState(windowState() ^ Qt::WindowFullScreen); // Toggle fullscreen on F1
        break;
    default:
        QGLWidget::keyPressEvent(event); // Let base class handle the other keys
    }
    updateGL();
}

void Piramide::changeEvent(QEvent *event) {
    switch (event->type()) {
    case QEvent::WindowStateChange:
        // Hide cursor if the window is fullscreen, otherwise show it
        if (windowState() == Qt::WindowFullScreen)
            setCursor(Qt::BlankCursor);
        else if(windowState() == Qt::WindowMaximized) {
            update();
        }
        else
            unsetCursor();
        break;
    default:
        break;
    }
}

void Piramide::desenhaCubo() {
     glColor3f(10,20,0);
    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );    //Desenha apenas as linhas do polígono
      //  glLineWidth(4);
    //Base
    glBegin(GL_QUADS);  // Wall
    glVertex3f(0,-1,0);
    glVertex3f(0,-1,1);
    glVertex3f(1,-1,1);
    glVertex3f(1,-1,0);
    glEnd();

    //Front
    glBegin(GL_QUADS);  // Wall
    glVertex3f(1,-1,1);
    glVertex3f(1,-1,0);
    glVertex3f(1,0,0);
    glVertex3f(1,0,1);
    glEnd();


    // Back
    glBegin(GL_QUADS);  // Wall
    glVertex3f(0,-1,1);
    glVertex3f(0,-1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,0,1);
    glEnd();

    // Top
    glBegin(GL_QUADS);  // Wall
    glVertex3f(0,0,1);
    glVertex3f(0,0,0);
    glVertex3f(1,0,0);
    glVertex3f(1,0,1);
    glEnd();

    // Left
    glBegin(GL_QUADS);  // Wall
    glVertex3f(0,-1,1);
    glVertex3f(1,-1,1);
    glVertex3f(1,0,1);
    glVertex3f(0,0,1);
    glEnd();

    // Right
    glBegin(GL_QUADS);  // Wall
    glVertex3f(0,-1,0);
    glVertex3f(1,-1,0);
    glVertex3f(1,0,0);
    glVertex3f(0,0,0);
    glEnd();



}

GLuint Piramide::createDL() {
    GLuint blockDL;

    blockDL = glGenLists(1); //aloca o numero de IDs

    glNewList(blockDL,GL_COMPILE); //inclue na lista
    for (int nvl = 0; nvl < LAST_NVL; nvl++ ){//para determinar o nivel atual da piramide ou o Y onde o cubo sera desenhado
        for (int x=-nvl; x <= nvl ; x++) {// para determinar a coordena X onde o cubo sera desenhado
            for ( int z=-nvl; z <= nvl; z++) {// para determinar a coordena Z onde o cubo sera desenhado
                if( (nvl == 0 || nvl == LAST_NVL) ||
                     (x == -nvl || x == nvl) ||
                     (z == -nvl || z == nvl)
                  ){// apenas desenhar os cubos que estão expõe a aréa externa da piramide
                    glTranslatef(x, -nvl, z);
                    desenhaCubo();
                    glTranslatef(-x, nvl, -z);
                }
            }
        }
    }
    glEndList();

    return(blockDL);
}
