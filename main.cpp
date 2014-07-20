#include "alsfd.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ALSFD w;
    w.show();

    return a.exec();
}
