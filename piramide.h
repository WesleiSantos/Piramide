#ifndef PIRAMDIE_H
#define PIRAMIDE_H

#include <QGLWidget>
#include <QHash>
#include <QTime>

class QTimer;

class Piramide : public QGLWidget
{
    Q_OBJECT

public:
    Piramide();
    ~Piramide();
    GLfloat win=250;
    GLint view_w, view_h;


protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent *event);
    void changeEvent(QEvent *event);
    void desenhaCubo();
    GLuint createDL();

private:
    QTimer *timer;
    //define texturas que seram usadas no programa
    GLuint _texturaCeu, _texturaSolo, _texturaBloco;


};
#endif // PIRAMIDE_H


