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
    //Habilita o uso de texturas
     glEnable(GL_TEXTURE_2D);
     //carrega uma imagem BMP
     QImage imgBloco = convertToGLFormat(QImage("bloco.jpg"));
     QImage imgCeu = convertToGLFormat(QImage("ceu.jpg"));
     QImage imgSolo = convertToGLFormat(QImage("solo.jpg"));

     //definir a forma de armazenamento dos pixeis na textura (1 = alinhamento por byte)
     glGenTextures(1, &_texturaBloco); //Make room for our texture
     //define a textura corrente
     glBindTexture(GL_TEXTURE_2D,_texturaBloco);
     //GL_TEXTURE_2D ==> define que será usanda uma textura 2D (bitmaps)
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgBloco.width(), imgBloco.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgBloco.bits());

     //definir a forma de armazenamento dos pixeis na textura (1 = alinhamento por byte)
     glGenTextures(1, &_texturaCeu); //Make room for our texture
     //define a textura corrente
     glBindTexture(GL_TEXTURE_2D, _texturaCeu);
     //GL_TEXTURE_2D ==> define que será usanda uma textura 2D (bitmaps)
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgCeu.width(), imgCeu.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgCeu.bits());

     //definir a forma de armazenamento dos pixeis na textura (1 = alinhamento por byte)
     glGenTextures(1, &_texturaSolo); //Make room for our texture
     //define a textura corrente
     glBindTexture(GL_TEXTURE_2D, _texturaSolo);
     //GL_TEXTURE_2D ==> define que será usanda uma textura 2D (bitmaps)
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgSolo.width(), imgSolo.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgSolo.bits());

     // Set up lighting
     GLfloat ambLight[] = {1, 1, 1, 1.0f};
     GLfloat diffLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
     GLfloat lightPos[] = {100, 100.f,100 ,.0f};
     // Ativa o uso da luz ambiente
     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambLight);
     glLightfv(GL_LIGHT1, GL_AMBIENT, ambLight);
     glLightfv(GL_LIGHT1, GL_DIFFUSE, diffLight);
     glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
     // Habilita a definição da cor do material a partir da cor corrente
     glEnable(GL_COLOR_MATERIAL);
     //Habilita o uso de iluminação
     glEnable(GL_LIGHTING);
     // Habilita a luz de número 1
     glEnable(GL_LIGHT1);
     // Habilita o depth-buffering
     glEnable(GL_DEPTH_TEST);

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
     glBindTexture(GL_TEXTURE_2D, _texturaCeu);
        glTranslatef(0,0,-200);
        glBegin(GL_QUADS);
            glTexCoord3f(0,0,0);glVertex3f(-2000,1000,0);
            glTexCoord3f(1,0,0);glVertex3f(2000,1000,0);
            glTexCoord3f(1,1,0);glVertex3f(1000,-1000,0);
            glTexCoord3f(0,1,0);glVertex3f(-1000,-1000,0);
        glEnd();
    glPopMatrix();
    glRotatef(yrot, 0.0, 1.0, 0.0);
    //Desenha solo
    glBindTexture(GL_TEXTURE_2D, _texturaSolo);
    glBegin(GL_QUADS);
        glTexCoord3f(0,0,0);glVertex3f(-400,-46,400);
         glTexCoord3f(1,0,0);glVertex3f(-400,-46,-400);
        glTexCoord3f(1,1,0);glVertex3f(400,-46,-400);
        glTexCoord3f(0,1,0);glVertex3f(400,-46,400);
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
    glBindTexture(GL_TEXTURE_2D, _texturaBloco);
    //Base
    glBegin(GL_QUADS);  // Wall
        glTexCoord3f(0,0,0);glVertex3f(0,-1,0);
        glTexCoord3f(1,0,0);glVertex3f(0,-1,1);
        glTexCoord3f(1,1,0);glVertex3f(1,-1,1);
        glTexCoord3f(0,1,0);glVertex3f(1,-1,0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, _texturaBloco);
    //Front
    glBegin(GL_QUADS);  // Wall
        glTexCoord3f(0,0,0);glVertex3f(1,-1,1);
        glTexCoord3f(1,0,0);glVertex3f(1,-1,0);
        glTexCoord3f(1,1,0);glVertex3f(1,0,0);
        glTexCoord3f(0,1,0);glVertex3f(1,0,1);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, _texturaBloco);
    // Back
    glBegin(GL_QUADS);  // Wall
        glTexCoord3f(0,0,0);glVertex3f(0,-1,1);
       glTexCoord3f(1,0,0);glVertex3f(0,-1,0);
        glTexCoord3f(1,1,0); glVertex3f(0,0,0);
        glTexCoord3f(0,1,0);glVertex3f(0,0,1);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, _texturaBloco);
    // Top
    glBegin(GL_QUADS);  // Wall
        glTexCoord3f(0,0,0);glVertex3f(0,0,1);
        glTexCoord3f(1,0,0);glVertex3f(0,0,0);
        glTexCoord3f(1,1,0);glVertex3f(1,0,0);
        glTexCoord3f(0,1,0); glVertex3f(1,0,1);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, _texturaBloco);
    // Left
    glBegin(GL_QUADS);  // Wall
        glTexCoord3f(0,0,1);glVertex3f(0,-1,1);
        glTexCoord3f(1,0,1); glVertex3f(1,-1,1);
        glTexCoord3f(1,1,1);glVertex3f(1,0,1);
        glTexCoord3f(0,1,1);glVertex3f(0,0,1);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, _texturaBloco);
    // Right
    glBegin(GL_QUADS);  // Wall
        glTexCoord3f(0,0,1);glVertex3f(0,-1,0);
        glTexCoord3f(1,0,1); glVertex3f(1,-1,0);
        glTexCoord3f(1,1,1);glVertex3f(1,0,0);
        glTexCoord3f(0,1,1); glVertex3f(0,0,0);
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
