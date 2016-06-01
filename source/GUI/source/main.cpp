#include "../include/mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <iostream>
#include <fstream>
//#include "PetriNetUsing.h"
#include "PetriNetUsing.h"
#include "PetriNetComponents.h"

#include "Scorp/Core/ScorpTime.h"

void serialize(std::ostream& output, RailwayPetriNet* petriNet)
{
    output << "Transition::ExitFromStation\n";
    for (auto it = petriNet->beginTransition<PetriNetComponent::ExitFromStation>(); it != petriNet->endTransition<PetriNetComponent::ExitFromStation>(); ++it)
    {
        (*it).second.serialize(output);
        output << "\n";
    }
    output << "Transition::EnterToStation\n";
    for (auto it = petriNet->beginTransition<PetriNetComponent::EnterToStation>(); it != petriNet->endTransition<PetriNetComponent::EnterToStation>(); ++it)
    {
        (*it).second.serialize(output);
        output << "\n";
    }
    output << "State::Station\n";
    for (auto it = petriNet->beginState<PetriNetComponent::Station>(); it != petriNet->endState<PetriNetComponent::Station>(); ++it)
    {
        auto& state = (*it).second;
        state.serialize(output);
        output << "\n";
        auto marker_storage = state.getMarkerStorage<PetriNetComponent::Train>();
        output << "{";
        bool is_first = true;
        for (auto marker_id : marker_storage)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << marker_id;
            is_first = false;
            //auto marker = petriNet->getMarkerById<PetriNetComponent::Train>(marker_id);
            //marker.serialize(output);
        }
        output << "}\n";
        auto transition_storage1 = state.getInTransitionStorage<PetriNetComponent::ExitFromStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage1)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
        auto transition_storage2 = state.getInTransitionStorage<PetriNetComponent::EnterToStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage2)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
        auto transition_storage3 = state.getOutTransitionStorage<PetriNetComponent::ExitFromStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage3)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
        auto transition_storage4 = state.getOutTransitionStorage<PetriNetComponent::EnterToStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage4)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n#\n";
    }
    output << "State::InterState\n";
    for (auto it = petriNet->beginState<PetriNetComponent::InterState>(); it != petriNet->endState<PetriNetComponent::InterState>(); ++it)
    {
        auto& state = (*it).second;
        state.serialize(output);
        auto marker_storage = state.getMarkerStorage<PetriNetComponent::Train>();
        output << "{";
        bool is_first = true;
        for (auto marker_id : marker_storage)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << marker_id;
            is_first = false;
            //auto marker = petriNet->getMarkerById<PetriNetComponent::Train>(marker_id);
            //marker.serialize(output);
        }
        output << "}\n";
        auto transition_storage1 = state.getInTransitionStorage<PetriNetComponent::ExitFromStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage1)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
        auto transition_storage2 = state.getInTransitionStorage<PetriNetComponent::EnterToStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage2)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
        auto transition_storage3 = state.getOutTransitionStorage<PetriNetComponent::ExitFromStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage3)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
        auto transition_storage4 = state.getOutTransitionStorage<PetriNetComponent::EnterToStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage4)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
    }
    output << "State::Semaphore\n";
    for (auto it = petriNet->beginState<PetriNetComponent::Semaphore>(); it != petriNet->endState<PetriNetComponent::Semaphore>(); ++it)
    {
        auto& state = (*it).second;
        state.serialize(output);
        auto marker_storage = state.getMarkerStorage<PetriNetComponent::Train>();
        output << "{";
        bool is_first = true;
        for (auto marker_id : marker_storage)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << marker_id;
            is_first = false;
            //auto marker = petriNet->getMarkerById<PetriNetComponent::Train>(marker_id);
            //marker.serialize(output);
        }
        output << "}\n";
        auto transition_storage1 = state.getInTransitionStorage<PetriNetComponent::ExitFromStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage1)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
        auto transition_storage2 = state.getInTransitionStorage<PetriNetComponent::EnterToStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage2)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
        auto transition_storage3 = state.getOutTransitionStorage<PetriNetComponent::ExitFromStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage3)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
        auto transition_storage4 = state.getOutTransitionStorage<PetriNetComponent::EnterToStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage4)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
    }
    output << "Marker::Train\n";
    for (auto it = petriNet->beginMarker<PetriNetComponent::Train>(); it != petriNet->endMarker<PetriNetComponent::Train>(); ++it)
    {
        (*it).second.serialize(output);
        output << "\n";
    }
    output << "Marker::AccessToken\n";
    for (auto it = petriNet->beginMarker<PetriNetComponent::AccessToken>(); it != petriNet->endMarker<PetriNetComponent::AccessToken>(); ++it)
    {
        (*it).second.serialize(output);
        output << "\n";
    }
}

void deserialize(std::istream& input, RailwayPetriNet* petriNet)
{
    std::string line;
    std::getline(input, line, '\n');

    while (true)
    {
        std::getline(input, line, '\n');
        if (line.compare("Transition::EnterToStation") == 0)
        {
            break;
        }
        else
        {
            petriNet->addTransition<PetriNetComponent::ExitFromStation>(PetriNetComponent::ExitFromStation());
        }
    }
    //=======================
    /*
    for (auto it = petriNet->beginTransition<PetriNetComponent::ExitFromStation>(); it != petriNet->endTransition<PetriNetComponent::ExitFromStation>(); ++it)
    {
        (*it).second.serialize(output);
        output << "\n";
    }
    output << "Transition::EnterToStation\n";
    for (auto it = petriNet->beginTransition<PetriNetComponent::EnterToStation>(); it != petriNet->endTransition<PetriNetComponent::EnterToStation>(); ++it)
    {
        (*it).second.serialize(output);
        output << "\n";
    }
    output << "State::Station\n";
    for (auto it = petriNet->beginState<PetriNetComponent::Station>(); it != petriNet->endState<PetriNetComponent::Station>(); ++it)
    {
        auto& state = (*it).second;
        state.serialize(output);
        output << "\n";
        auto marker_storage = state.getMarkerStorage<PetriNetComponent::Train>();
        output << "{";
        bool is_first = true;
        for (auto marker_id : marker_storage)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << marker_id;
            is_first = false;
            //auto marker = petriNet->getMarkerById<PetriNetComponent::Train>(marker_id);
            //marker.serialize(output);
        }
        output << "}\n";
        auto transition_storage1 = state.getInTransitionStorage<PetriNetComponent::ExitFromStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage1)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
        auto transition_storage2 = state.getInTransitionStorage<PetriNetComponent::EnterToStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage2)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
        auto transition_storage3 = state.getOutTransitionStorage<PetriNetComponent::ExitFromStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage3)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
        auto transition_storage4 = state.getOutTransitionStorage<PetriNetComponent::EnterToStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage4)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n#\n";
    }
    output << "State::InterState\n";
    for (auto it = petriNet->beginState<PetriNetComponent::InterState>(); it != petriNet->endState<PetriNetComponent::InterState>(); ++it)
    {
        auto& state = (*it).second;
        state.serialize(output);
        auto marker_storage = state.getMarkerStorage<PetriNetComponent::Train>();
        output << "{";
        bool is_first = true;
        for (auto marker_id : marker_storage)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << marker_id;
            is_first = false;
            //auto marker = petriNet->getMarkerById<PetriNetComponent::Train>(marker_id);
            //marker.serialize(output);
        }
        output << "}\n";
        auto transition_storage1 = state.getInTransitionStorage<PetriNetComponent::ExitFromStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage1)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
        auto transition_storage2 = state.getInTransitionStorage<PetriNetComponent::EnterToStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage2)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
        auto transition_storage3 = state.getOutTransitionStorage<PetriNetComponent::ExitFromStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage3)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
        auto transition_storage4 = state.getOutTransitionStorage<PetriNetComponent::EnterToStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage4)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
    }
    output << "State::Semaphore\n";
    for (auto it = petriNet->beginState<PetriNetComponent::Semaphore>(); it != petriNet->endState<PetriNetComponent::Semaphore>(); ++it)
    {
        auto& state = (*it).second;
        state.serialize(output);
        auto marker_storage = state.getMarkerStorage<PetriNetComponent::Train>();
        output << "{";
        bool is_first = true;
        for (auto marker_id : marker_storage)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << marker_id;
            is_first = false;
            //auto marker = petriNet->getMarkerById<PetriNetComponent::Train>(marker_id);
            //marker.serialize(output);
        }
        output << "}\n";
        auto transition_storage1 = state.getInTransitionStorage<PetriNetComponent::ExitFromStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage1)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
        auto transition_storage2 = state.getInTransitionStorage<PetriNetComponent::EnterToStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage2)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
        auto transition_storage3 = state.getOutTransitionStorage<PetriNetComponent::ExitFromStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage3)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
        auto transition_storage4 = state.getOutTransitionStorage<PetriNetComponent::EnterToStation>();
        output << "{";
        is_first = true;
        for (auto transition_id : transition_storage4)
        {
            if (!is_first)
            {
                output << ",";
            }
            output << transition_id;
            is_first = false;
        }
        output << "}\n";
    }
    output << "Marker::Train\n";
    for (auto it = petriNet->beginMarker<PetriNetComponent::Train>(); it != petriNet->endMarker<PetriNetComponent::Train>(); ++it)
    {
        (*it).second.serialize(output);
        output << "\n";
    }
    output << "Marker::AccessToken\n";
    for (auto it = petriNet->beginMarker<PetriNetComponent::AccessToken>(); it != petriNet->endMarker<PetriNetComponent::AccessToken>(); ++it)
    {
        (*it).second.serialize(output);
        output << "\n";
    }
    */
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*
    ScorpCore::Time time = ScorpCore::Time::fromString("0000-00-10T20:15:34");
    std::cout << "Time: d = " << time.getDays() << ", h = " << time.getHours()
              << ", m = " << time.getMinutes() << ", s = " << time.getSeconds() << std::endl;
    std::string str_time = ScorpCore::Time::toString(time);
    std::cout << "Time: " << str_time << std::endl;
    */
    std::unique_ptr<RailwayPetriNet> petriNet = std::move(std::unique_ptr<RailwayPetriNet>(new RailwayPetriNet()));
    int state_1 = petriNet->addState<PetriNetComponent::Station>(PetriNetComponent::Station("Station 1", 100, 100, 2));
    int state_2 = petriNet->addState<PetriNetComponent::Station>(PetriNetComponent::Station("Station 2", 200, 200, 2));
    int blocking_state = petriNet->addState<PetriNetComponent::Semaphore>(PetriNetComponent::Semaphore());
    int inter_state = petriNet->addState<PetriNetComponent::InterState>(PetriNetComponent::InterState());
    int transition_1 = petriNet->addTransition<PetriNetComponent::ExitFromStation>(PetriNetComponent::ExitFromStation());
    int transition_2 = petriNet->addTransition<PetriNetComponent::EnterToStation>(PetriNetComponent::EnterToStation());
    petriNet->addStateToTransitionConnection<PetriNetComponent::Station, PetriNetComponent::ExitFromStation>(state_1, transition_1);
    petriNet->addTransitionToStateConnection<PetriNetComponent::EnterToStation, PetriNetComponent::Station>(transition_2, state_2);
    petriNet->addTransitionToStateConnection<PetriNetComponent::ExitFromStation, PetriNetComponent::InterState>(transition_1, inter_state);
    petriNet->addTransitionToStateConnection<PetriNetComponent::EnterToStation, PetriNetComponent::Semaphore>(transition_2, blocking_state);
    petriNet->addStateToTransitionConnection<PetriNetComponent::InterState, PetriNetComponent::EnterToStation>(inter_state, transition_2);
    petriNet->addStateToTransitionConnection<PetriNetComponent::Semaphore, PetriNetComponent::ExitFromStation>(blocking_state, transition_1);
    int train_marker = petriNet->addMarker<PetriNetComponent::Train>(state_1, PetriNetComponent::Train(1001));
    int access_token = petriNet->addMarker<PetriNetComponent::AccessToken>(blocking_state, PetriNetComponent::AccessToken());
    // Test serialize
    /*
    auto& serialized_state = petriNet->getStateById<PetriNetComponent::Station>(state_1);
    serialized_state.serialize(std::cout);
    std::cout << std::endl;
    auto& serialized_transition = petriNet->getTransitionById<PetriNetComponent::ExitFromStation>(transition_1);
    serialized_transition.serialize(std::cout);
    std::cout << std::endl;
    auto& serialized_marker = petriNet->getMarkerById<PetriNetComponent::Train>(train_marker);
    serialized_marker.serialize(std::cout);
    std::cout << std::endl;

    std::cout << "States:" << std::endl;
    for (auto it = petriNet->beginState<PetriNetComponent::Station>(); it != petriNet->endState<PetriNetComponent::Station>(); ++it)
    {
        (*it).second.serialize(std::cout);
        std::cout << std::endl;
        //std::cout << std::string(identLevel, ' ') <<  enumMapping[static_cast<int>((*it).first)] << " " << (*it).second.getObjectId() << " " << (*it).second.getObjectSerializedType() << std::endl;
    }
    std::cout << std::endl;
    */
    std::ofstream output("serialize_test.txt");
    serialize(output, petriNet.get());

    //qDebug() << "Normal start";
    MainWindow w;
    w.show();

    return a.exec();
}
