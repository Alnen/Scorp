#include "../include/mainwindow.h"
#include <QApplication>
#include <QDebug>

#include "PetriNetUsing.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*
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
    petriNet->addTransitionToStateConnection<PetriNetComponent::ExitFromStation, PetriNetComponent::Semaphore>(transition_1, blocking_state);
    petriNet->addStateToTransitionConnection<PetriNetComponent::InterState, PetriNetComponent::EnterToStation>(inter_state, transition_2);
    petriNet->addStateToTransitionConnection<PetriNetComponent::Semaphore, PetriNetComponent::EnterToStation>(blocking_state, transition_2);

    int train_marker = petriNet->addMarker<PetriNetComponent::Train>(state_1, PetriNetComponent::Train(1001));
    int access_token = petriNet->addMarker<PetriNetComponent::AccessToken>(blocking_state, PetriNetComponent::AccessToken());

    petriNet->executeMarkersPropagation();
    */

    qDebug() << "Normal start";
    MainWindow w;
    w.show();

    return a.exec();
}
