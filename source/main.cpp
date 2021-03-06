#include "container/internal/TypeStorage.h"
#include "meta/TypeListStorage.h"
#include "container/PetriNet.h"
#include <iostream>
#include "container/PetriNetTraits.h"
#include "meta/TypeList.h"
#include "container/internal/IntegralIdGenerator.h"
#include "container/internal/PetriNetStorage.h"

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

void test1()
{
    TestPetriNet petriNet;

    TypeStorage<int> storage;
    storage.push_back(10);
    storage.push_back(20);
    storage.push_back(30);

    for (const int value : storage)
    {
        std::cout << value << std::endl;
    }

    for (size_t i = 0; i < storage.size(); ++i)
    {
        int& val = storage[i];
        std::cout << val << std::endl;
    }

    for (int value : {10, 20, 30})
    {
        auto it = std::find(storage.begin(), storage.end(), value);
        if (it == storage.end()) continue;
        storage.erase(it);
    }

    std::cout << "Is empty: " << std::boolalpha << storage.empty() << std::endl;
}

int main()
{
    TestPetriNetStorage petriNetStorage;
    auto& intMarkerStorage = petriNetStorage.getMarkerStorage<int>();
    intMarkerStorage[10] = typename TestPetriNetStorage::SpecializedMarkerWrapper<int>::type(10, 15);

    std::cout << "ID: " << intMarkerStorage[10].getId()
              << " Marker: " << intMarkerStorage[10].getMarker() << std::endl;

    auto& specialMarkerStorage = petriNetStorage.getMarkerStorage<SpecialMarker>();
    specialMarkerStorage[11] = typename TestPetriNetStorage::SpecializedMarkerWrapper<SpecialMarker>::type(11, 10, 10.0, 20.0);
    specialMarkerStorage[12] = typename TestPetriNetStorage::SpecializedMarkerWrapper<SpecialMarker>::type(12, 10, SpecialMarker(20.0, 10.0));
    auto& specialMarker = specialMarkerStorage[11];
    std::cout << "ID: " << specialMarker.getId()
              << " MarkerX: " << specialMarker.getMarker().getX()
              << " MarkerY: " << specialMarker.getMarker().getY() << std::endl;
    auto& specialMarker2 = specialMarkerStorage[12];
    std::cout << "ID: " << specialMarker2.getId()
              << " MarkerX: " << specialMarker2.getMarker().getX()
              << " MarkerY: " << specialMarker2.getMarker().getY() << std::endl;

    TestPetriNet petriNet;
    auto stateId = petriNet.addState<int>(10);
    auto transitionId = petriNet.addTransition<int>(20);
    stateId = petriNet.addState<int>(20);
    auto id = petriNet.addMarker<SpecialMarker>(stateId, SpecialMarker(20.0, 10.0));
    std::cout << "new marker ID: " << id << std::endl;

    auto& markerRef1 = petriNet.getMarkerById<SpecialMarker>(id);
    std::cout << "ID: " << id
              << " MarkerX: " << markerRef1.getX()
              << " MarkerY: " << markerRef1.getY() << std::endl;

    markerRef1.setX(markerRef1.getX() + 10.0);
    markerRef1.setY(markerRef1.getY() + 10.0);

    auto& markerRef2 = petriNet.getMarkerById<SpecialMarker>(id);
    std::cout << "ID: " << id
              << " MarkerX: " << markerRef2.getX()
              << " MarkerY: " << markerRef2.getY() << std::endl;
    petriNet.removeMarker<SpecialMarker>(id);

    std::cout << "Number of markers with type SpecialMarker is " << petriNet.sizeMarker<SpecialMarker>() << std::endl;
    std::cout << "Number of state with type int is " << petriNet.sizeState<int>() << std::endl;

    id = petriNet.addMarker<SpecialMarker>(stateId, SpecialMarker(20.0, 10.0));

    std::cout << "After adding new marker " << std::endl;
    std::cout << "Number of markers with type SpecialMarker is " << petriNet.sizeMarker<SpecialMarker>() << std::endl;
    std::cout << "Number of state with type int is " << petriNet.sizeState<int>() << std::endl;

    petriNet.removeState<int>(stateId);
    std::cout << "After deleting state with marker " << std::endl;
    std::cout << "Number of markers with type SpecialMarker is " << petriNet.sizeMarker<SpecialMarker>() << std::endl;
    std::cout << "Number of state with type int is " << petriNet.sizeState<int>() << std::endl;
    std::cout << "Number of transition with type int is " << petriNet.sizeTransition<int>() << std::endl;

    petriNet.removeTransition<int>(transitionId);

    std::cout << "Number of transition with type int is " << petriNet.sizeTransition<int>() << std::endl;

    return 0;
}