#include "mainwindow.h"
#include <QApplication>
//#include "DB/ScorpDBInterface.h"
#include "DB/ScorpDBSell.h"

#include <fstream>
#include <string>

#include <QDebug>

/*
class Marker
{
private:
    int id;
    int state_id;
public:
    void serialize(std::ofstream& output)
    {
        //
    }

    void deserialize(std::ifstream& input)
    {
        //
    }
};

// Train : 'Train', id, state_id
// AccessToken : 'AccessToken', id, state_id
// Semaphore : 'Semaphore'
// City : 'City', id, x, y
// InterCity : 'InterCity'
// ExitCity: 'ExitCity', id, x, y
// EnterCity: 'EnterCity', id, x, y
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //ScorpDBSell* db_interface = new ScorpDBSell("../Scorp/DB/ScorpDB.db", false);

    //std::ifstream ifs("../Scorp/test.txt", std::ifstream::in);
    //deserialize(ifs);

    MainWindow w;
    w.show();

    return a.exec();
}
