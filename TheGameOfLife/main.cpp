#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setFixedSize(500, 600);
    w.setWindowTitle("The Game of Life");
    w.show();

    return a.exec();
}
