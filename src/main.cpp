#include "dfmwatch.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DfmWatch w;
    w.show();
    return a.exec();
}
