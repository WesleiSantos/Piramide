#include "piramide.h"
#include <QKeyEvent>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

GLint cartaSelecionada = 6;
float xrot=0.f;
float yrot=0.0;
GLint DLid;
// Constructor
Piramide::Piramide() {
    setWindowTitle("Piramide");
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
    glLoadIdentity();
    gluPerspective(45.0f, static_cast<GLfloat>(width)/height,0.1f, 1000.0f); // Calculate aspect ratio
    // Especifica posição do observador e do alvo
    gluLookAt(-5.f,20.f,70.f, 0,0,0, 0,1,0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// OpenGL painting code goes here
void Piramide::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Desenha Ceu
    glPushMatrix();
     glColor3f(0.2,0.8,0.9);
        glTranslatef(0,0,-100);
        glBegin(GL_QUADS);
            glVertex3f(-200,100,0);
            glVertex3f(200,100,0);
            glVertex3f(100,-100,0);
            glVertex3f(-100,-100,0);
        glEnd();
    glPopMatrix();
    glTranslatef(0,0,-10);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glColor3f(0.5, 0.8, 0.3);
    //Desenha solo
    glBegin(GL_QUADS);
        glVertex3f(-200,-1,200);
        glVertex3f(-200,-1,-200);
        glVertex3f(200,-1,-200);
        glVertex3f(200,-1,200);
    glEnd();

    glCallList(DLid); //chama lista de exibição
}

// Key handler
void Piramide::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Up:
        xrot+=1;
        break;
    case Qt::Key_Down:
        xrot-=1;
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
    int quantidadeNiveis=15; //Quantidade de niveis na piramide
    for(int y=quantidadeNiveis; y>0; y--){
        int quantBlocosLinha = (2*y)-1;
        for(int x=0; x < quantBlocosLinha; x++){
            for(int z=0; z < quantBlocosLinha; z++){
                glPushMatrix();
                glTranslatef(x+(quantidadeNiveis-y),(y-quantidadeNiveis)*-1, z+(quantidadeNiveis-y));
                desenhaCubo();
                glPopMatrix();
            }
        }
    }
    glEndList();

    return(blockDL);
}
