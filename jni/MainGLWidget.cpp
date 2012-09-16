#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QCoreApplication>
#include "MainGLWidget.h"
#include <Input/Input.h>

static std::map<U32, U64> keyMapper;

U64 lastTime;

static void initKeyMapper() {
    keyMapper[Qt::Key_0] = ENGINE_KEYCODE_0;
    keyMapper[Qt::Key_1] = ENGINE_KEYCODE_1;
    keyMapper[Qt::Key_2] = ENGINE_KEYCODE_2;
    keyMapper[Qt::Key_3] = ENGINE_KEYCODE_3;
    keyMapper[Qt::Key_4] = ENGINE_KEYCODE_4;
    keyMapper[Qt::Key_5] = ENGINE_KEYCODE_5;
    keyMapper[Qt::Key_6] = ENGINE_KEYCODE_6;
    keyMapper[Qt::Key_7] = ENGINE_KEYCODE_7;
    keyMapper[Qt::Key_8] = ENGINE_KEYCODE_8;
    keyMapper[Qt::Key_9] = ENGINE_KEYCODE_9;
    keyMapper[Qt::Key_Up] = ENGINE_KEYCODE_UP;
    keyMapper[Qt::Key_Down] = ENGINE_KEYCODE_DOWN;
    keyMapper[Qt::Key_Left] = ENGINE_KEYCODE_LEFT;
    keyMapper[Qt::Key_Right] = ENGINE_KEYCODE_RIGHT;
    keyMapper[Qt::Key_Shift] = ENGINE_KEYCODE_CENTER;
    keyMapper[Qt::Key_A] = ENGINE_KEYCODE_A;
    keyMapper[Qt::Key_B] = ENGINE_KEYCODE_B;
    keyMapper[Qt::Key_C] = ENGINE_KEYCODE_C;
    keyMapper[Qt::Key_D] = ENGINE_KEYCODE_D;
    keyMapper[Qt::Key_E] = ENGINE_KEYCODE_E;
    keyMapper[Qt::Key_F] = ENGINE_KEYCODE_F;
    keyMapper[Qt::Key_G] = ENGINE_KEYCODE_G;
    keyMapper[Qt::Key_H] = ENGINE_KEYCODE_H;
    keyMapper[Qt::Key_I] = ENGINE_KEYCODE_I;
    keyMapper[Qt::Key_J] = ENGINE_KEYCODE_J;
    keyMapper[Qt::Key_K] = ENGINE_KEYCODE_K;
    keyMapper[Qt::Key_L] = ENGINE_KEYCODE_L;
    keyMapper[Qt::Key_M] = ENGINE_KEYCODE_M;
    keyMapper[Qt::Key_N] = ENGINE_KEYCODE_N;
    keyMapper[Qt::Key_O] = ENGINE_KEYCODE_O;
    keyMapper[Qt::Key_P] = ENGINE_KEYCODE_P;
    keyMapper[Qt::Key_Q] = ENGINE_KEYCODE_Q;
    keyMapper[Qt::Key_R] = ENGINE_KEYCODE_R;
    keyMapper[Qt::Key_S] = ENGINE_KEYCODE_S;
    keyMapper[Qt::Key_T] = ENGINE_KEYCODE_T;
    keyMapper[Qt::Key_U] = ENGINE_KEYCODE_U;
    keyMapper[Qt::Key_V] = ENGINE_KEYCODE_V;
    keyMapper[Qt::Key_W] = ENGINE_KEYCODE_W;
    keyMapper[Qt::Key_X] = ENGINE_KEYCODE_X;
    keyMapper[Qt::Key_Y] = ENGINE_KEYCODE_Y;
    keyMapper[Qt::Key_Z] = ENGINE_KEYCODE_Z;
    keyMapper[Qt::Key_Comma] = ENGINE_KEYCODE_COMMA;
    keyMapper[Qt::Key_Period] = ENGINE_KEYCODE_PERIOD;
    keyMapper[Qt::Key_Tab] = ENGINE_KEYCODE_TAB;
    keyMapper[Qt::Key_Space] = ENGINE_KEYCODE_SPACE;
    keyMapper[Qt::Key_Enter] = ENGINE_KEYCODE_ENTER;
    keyMapper[Qt::Key_Delete] = ENGINE_KEYCODE_DEL;
    keyMapper[Qt::Key_Minus] = ENGINE_KEYCODE_MINUS;
    keyMapper[Qt::Key_Equal] = ENGINE_KEYCODE_EQUALS;
    keyMapper[Qt::Key_BracketLeft] = ENGINE_KEYCODE_LEFT_BRACKET;
    keyMapper[Qt::Key_BracketRight] = ENGINE_KEYCODE_RIGHT_BRACKET;
    keyMapper[Qt::Key_Backslash] = ENGINE_KEYCODE_BACKSLASH;
    keyMapper[Qt::Key_Semicolon] = ENGINE_KEYCODE_SEMICOLON;
    keyMapper[Qt::Key_Apostrophe] = ENGINE_KEYCODE_APOSTROPHE;
    keyMapper[Qt::Key_Slash] = ENGINE_KEYCODE_SLASH;
    keyMapper[Qt::Key_At] = ENGINE_KEYCODE_AT;
    keyMapper[Qt::Key_Plus] = ENGINE_KEYCODE_PLUS;
    keyMapper[Qt::Key_F1] = ENGINE_KEYCODE_MENU;
    keyMapper[Qt::Key_Escape] = ENGINE_KEYCODE_ESCAPE;
}

static int lastDown;

MainGLWidget::MainGLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    initKeyMapper();

    setAutoBufferSwap(true);

    setMinimumSize(800, 480);
    engine = new QtEngine(this);

    lastDown=-1;
    gogo = false;

    connect(&t, SIGNAL(timeout()), this, SLOT(updateGL()));
    t.setInterval(0);


    setWindowTitle("QtGameEngine");
}

void MainGLWidget::mousePressEvent(QMouseEvent *e) {
    TouchEvent event;
    if(e->button()==Qt::LeftButton)
        event.pointerId = ENGINE_POINTER_0;
    else if(e->button()==Qt::RightButton)
        event.pointerId = ENGINE_POINTER_1;
    else if(e->button()==Qt::MidButton)
        event.pointerId = ENGINE_POINTER_2;


    lastDown = event.pointerId;

    event.posX = e->pos().x();
    event.posY = height()-e->pos().y();
    event.action = ENGINE_TOUCHACTION_DOWN;

    engine->ProcessTouchInput(event);
}

void MainGLWidget::mouseReleaseEvent(QMouseEvent *e) {
    TouchEvent event;
    if(e->button()==Qt::LeftButton)
        event.pointerId = ENGINE_POINTER_0;
    else if(e->button()==Qt::RightButton)
        event.pointerId = ENGINE_POINTER_1;
    else if(e->button()==Qt::MidButton)
        event.pointerId = ENGINE_POINTER_2;
    lastDown = -1;
    event.posX = e->pos().x();
    event.posY = height()-e->pos().y();
    event.action = ENGINE_TOUCHACTION_UP;

    engine->ProcessTouchInput(event);
}

void MainGLWidget::mouseMoveEvent(QMouseEvent *e) {
    TouchEvent event;

    event.pointerId = lastDown;
    event.posX = e->pos().x();
    event.posY = height()-e->pos().y();
    event.action = ENGINE_TOUCHACTION_MOVE;

    engine->ProcessTouchInput(event);
}

void MainGLWidget::keyPressEvent(QKeyEvent *e) {

    keysPressed+= e->key();

    //qDebug()<<"keyPress"<<e->key();

}

void MainGLWidget::keyReleaseEvent(QKeyEvent *e) {
    keysPressed-= e->key();

    KeyEvent event;
    event.keyCode = keyMapper[e->key()];
    event.action = ENGINE_KEYACTION_UP;

    engine->ProcessKeyInput(event);
}


void MainGLWidget::initializeGL() {
    engine->Initialize();
    engine->OnInitWindow();
    t.start();
}

void MainGLWidget::go() {
    gogo=true;

    while(1) {

        repaint();

        QCoreApplication::processEvents();

    }
}

void MainGLWidget::paintGL() {

    if(engine->IsQuiting()) {
        engine->Release();
        delete engine;
        exit(0);
    }

    if(engine->IsRunning()) {
        foreach(int k, keysPressed) {
            KeyEvent event;
            event.keyCode = keyMapper[k];
            event.action = ENGINE_KEYACTION_DOWN;

            engine->ProcessKeyInput(event);
        }

        engine->SingleFrame();
    }
}

void MainGLWidget::updateGL() {
    paintGL();
}

void MainGLWidget::resizeGL(int w, int h) {
    engine->OnInitWindow();
}

void MainGLWidget::focusInEvent(QFocusEvent *) {
    engine->OnGainedFocus();
}

void MainGLWidget::focusOutEvent(QFocusEvent *) {
    engine->OnLostFocus();
}

void MainGLWidget::closeEvent(QCloseEvent *) {
    t.stop();
    engine->OnTerminateWindow();

    engine->Release();
    delete engine;

    close();
    exit(0);
}


