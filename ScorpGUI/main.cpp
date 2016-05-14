#include "mainwindow.h"
#include <QApplication>
//#include "DB/ScorpDBInterface.h"
#include "DB/ScorpDBSell.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //ScorpDBSell* db_interface = new ScorpDBSell("../Scorp/DB/ScorpDB.db", false);

    MainWindow w;
    w.show();

    return a.exec();
}
