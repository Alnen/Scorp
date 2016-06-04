#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <QApplication>
#include <QDebug>
#include "Scorp/GUI/mainwindow.h"
#include "Scorp/Core/PetriNetUsing.h"
#include "Scorp/Core/PetriNetComponents.h"
#include "Scorp/Core/ScorpTime.h"
//#include "Scorp/Core/Serialization.h"

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
    (int)petriNet->addStateToTransitionConnection<PetriNetComponent::Station, PetriNetComponent::ExitFromStation>(state_1, transition_1);
    (int)petriNet->addTransitionToStateConnection<PetriNetComponent::EnterToStation, PetriNetComponent::Station>(transition_2, state_2);
    (int)petriNet->addTransitionToStateConnection<PetriNetComponent::ExitFromStation, PetriNetComponent::InterState>(transition_1, inter_state);
    (int)petriNet->addTransitionToStateConnection<PetriNetComponent::EnterToStation, PetriNetComponent::Semaphore>(transition_2, blocking_state);
    (int)petriNet->addStateToTransitionConnection<PetriNetComponent::InterState, PetriNetComponent::EnterToStation>(inter_state, transition_2);
    (int)petriNet->addStateToTransitionConnection<PetriNetComponent::Semaphore, PetriNetComponent::ExitFromStation>(blocking_state, transition_1);
    int train_marker = petriNet->addMarker<PetriNetComponent::Train>(state_1, PetriNetComponent::Train(1001));
    int access_token = petriNet->addMarker<PetriNetComponent::AccessToken>(blocking_state, PetriNetComponent::AccessToken());

    /*
    std::ofstream output("serialize_test.txt");
    serialize(output, petriNet.get());
    output.close();
    std::unique_ptr<RailwayPetriNet> petriNet2 = std::move(std::unique_ptr<RailwayPetriNet>(new RailwayPetriNet()));
    std::ifstream input("serialize_test.txt");
    deserialize(input, petriNet2.get());
    std::ofstream output2("deserialize_test.txt");
    serialize(output2, petriNet2.get());
    */

    //qDebug() << "Normal start";
    MainWindow w;
    w.show();

    return a.exec();
}
