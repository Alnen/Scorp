#include "../include/mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <iostream>
#include <fstream>
//#include "PetriNetUsing.h"
#include "PetriNetUsing.h"
#include "PetriNetComponents.h"

#include "Scorp/Core/ScorpTime.h"

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

    //qDebug() << "Normal start";
    MainWindow w;
    w.show();

    return a.exec();
}
