#include <QtGui/QApplication>
#include "MainGLWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    setlocale(LC_ALL,"C");

    MainGLWidget w;
    w.show();

    w.go();
    return a.exec();
}

