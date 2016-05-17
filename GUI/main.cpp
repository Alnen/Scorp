#include "mainwindow.h"
#include <QApplication>
//#include "DB/ScorpDBInterface.h"
#include "DB/ScorpDBSell.h"

#include <fstream>
#include <string>

#include <QDebug>

#include "include/container/internal/TypeStorage.h"
#include "include/meta/TypeListStorage.h"
#include "include/container/PetriNet.h"
#include <iostream>
#include "include/container/PetriNetTraits.h"
#include "include/meta/TypeList.h"
#include "include/container/internal/IntegralIdGenerator.h"
#include "include/container/internal/PetriNetStorage.h"

class SpecialMarker
{
public:
    SpecialMarker()
    {
        m_x = 0.0;
        m_y = 0.0;
    }

    SpecialMarker(double x, double y) :
            m_x(x),
            m_y(y)
    {

    }

    double getX() const
    {
        return m_x;
    }

    double getY() const
    {
        return m_y;
    }

    void setX(double x)
    {
        m_x = x;
    }

    void setY(double y)
    {
        m_y = y;
    }

private:
    double m_x = 0.0;
    double m_y = 0.0;
};

using TestTypeList = meta::TypeList<int, char, float>;

using TestPetriNetTraits = PetriNetTraits<meta::TypeList<SpecialMarker, int>, TestTypeList, TestTypeList>;
using TestPetriNet = container::PetriNet<TestPetriNetTraits>;
using TestPetriNetStorage = PetriNetStorage<TestPetriNetTraits>;

int main(int argc, char *argv[])
{
    TestPetriNet petriNet;
    auto stateId = petriNet.addState<int>(10);
    auto transitionId = petriNet.addTransition<int>(20);
    stateId = petriNet.addState<int>(20);
    auto id = petriNet.addMarker<SpecialMarker>(stateId, SpecialMarker(20.0, 10.0));
    qDebug() << "new marker ID: " << id;
    //std::cout << "new marker ID: " << id << std::endl;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
