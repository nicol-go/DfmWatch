#include "dfmwatch.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dfmwatch w;
    w.show();
    return a.exec();
}
