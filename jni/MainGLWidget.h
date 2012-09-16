#ifndef MAINGLWIDGET_H
#define MAINGLWIDGET_H

#include <QGLWidget>
#include <Core/QtEngine.h>
#include <QTimer>
#include <QSet>

class MainGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MainGLWidget(QWidget *parent = 0);


    void initializeGL();
    void paintGL();
    void updateGL();
    void resizeGL(int w, int h);

    bool gogo;

    void go();
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    void focusInEvent(QFocusEvent *);

    void focusOutEvent(QFocusEvent *);

    void closeEvent(QCloseEvent *);

signals:

public slots:


private:
    QtEngine* engine;
    QTimer t;
    QSet<int> keysPressed;
    bool m_bFirstRelease;
};

#endif // MAINGLWIDGET_H
