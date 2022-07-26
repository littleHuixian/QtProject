#include "sqliteqtdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SqliteQtDialog w;
    w.show();

    return a.exec();
}
