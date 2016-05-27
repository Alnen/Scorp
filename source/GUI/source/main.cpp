#include "../include/mainwindow.h"
#include <QApplication>
#include <QDebug>

//#include "PetriNetUsing.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*
    std::unique_ptr<RailwayPetriNet> petriNet = std::move(std::unique_ptr<RailwayPetriNet>(new RailwayPetriNet()));
    int state_1 = petriNet->addState<PetryNetComponent::Station>(PetryNetComponent::Station("Station 1", 100, 100, 2));
    int state_2 = petriNet->addState<PetryNetComponent::Station>(PetryNetComponent::Station("Station 2", 200, 200, 2));
    int blocking_state = petriNet->addState<PetryNetComponent::Semaphore>(PetryNetComponent::Semaphore());
    int inter_state = petriNet->addState<PetryNetComponent::InterState>(PetryNetComponent::InterState());

    int transition_1 = petriNet->addTransition<PetryNetComponent::ExitFromStation>(PetryNetComponent::ExitFromStation());
    int transition_2 = petriNet->addTransition<PetryNetComponent::EnterToStation>(PetryNetComponent::EnterToStation());
    petriNet->addStateToTransitionConnection<PetryNetComponent::Station, PetryNetComponent::ExitFromStation>(state_1, transition_1);
    petriNet->addTransitionToStateConnection<PetryNetComponent::EnterToStation, PetryNetComponent::Station>(transition_2, state_2);
    petriNet->addTransitionToStateConnection<PetryNetComponent::ExitFromStation, PetryNetComponent::InterState>(transition_1, inter_state);
    petriNet->addTransitionToStateConnection<PetryNetComponent::ExitFromStation, PetryNetComponent::Semaphore>(transition_1, blocking_state);
    petriNet->addStateToTransitionConnection<PetryNetComponent::InterState, PetryNetComponent::EnterToStation>(inter_state, transition_2);
    petriNet->addStateToTransitionConnection<PetryNetComponent::Semaphore, PetryNetComponent::EnterToStation>(blocking_state, transition_2);
    */

    qDebug() << "Normal start";
    MainWindow w;
    w.show();

    return a.exec();
}
