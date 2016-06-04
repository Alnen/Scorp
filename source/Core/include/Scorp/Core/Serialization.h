#ifndef PETRI_NET_SERIALIZATION_H
#define PETRI_NET_SERIALIZATION_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Scorp/Core/PetriNetUsing.h"
#include "Scorp/Core/PetriNetComponents.h"

template <class Storage>
void serializeIdStorage(std::ostream& output, const Storage& storage)
{
    output << "{";
    bool is_first = true;
    for (size_t i = 0; i < storage.size(); ++i)
    {
        if (!is_first)
        {
            output << ",";
        }
        else
        {
            is_first = false;
        }
        output << storage[i];
    }
    output << "}\n";
}

template <class StateType>
void serializeOneTypeStates(std::ostream& output, RailwayPetriNet* petriNet)
{
    for (auto it = petriNet->beginState<StateType>(); it != petriNet->endState<StateType>(); ++it)
    {
        auto& state = (*it).second;
        state.serialize(output);
        output << "\n";
        auto marker_storage = state.template getMarkerStorage<PetriNetComponent::Train>();
        serializeIdStorage<decltype(marker_storage)>(output, marker_storage);
        auto marker_storage2 = state.template getMarkerStorage<PetriNetComponent::AccessToken>();
        serializeIdStorage<decltype(marker_storage2)>(output, marker_storage2);
        auto transition_storage1 = state.template getInTransitionStorage<PetriNetComponent::ExitFromStation>();
        serializeIdStorage<decltype(transition_storage1)>(output, transition_storage1);
        auto transition_storage2 = state.template getInTransitionStorage<PetriNetComponent::EnterToStation>();
        serializeIdStorage<decltype(transition_storage2)>(output, transition_storage2);
        auto transition_storage3 = state.template getOutTransitionStorage<PetriNetComponent::ExitFromStation>();
        serializeIdStorage<decltype(transition_storage3)>(output, transition_storage3);
        auto transition_storage4 = state.template getOutTransitionStorage<PetriNetComponent::EnterToStation>();
        serializeIdStorage<decltype(transition_storage4)>(output, transition_storage4);
        output << "#\n";
    }
}

template <class TransitionType>
void serializeOneTypeTransitions(std::ostream& output, RailwayPetriNet* petriNet)
{
    for (auto it = petriNet->beginTransition<TransitionType>(); it != petriNet->endTransition<TransitionType>(); ++it)
    {
        auto& transition = (*it).second;
        transition.serialize(output);
        output << "\n";
        auto state_storage1 = transition.template getInStateStorage<PetriNetComponent::Station>();
        serializeIdStorage<decltype(state_storage1)>(output, state_storage1);
        auto state_storage2 = transition.template getInStateStorage<PetriNetComponent::InterState>();
        serializeIdStorage<decltype(state_storage2)>(output, state_storage2);
        auto state_storage3 = transition.template getInStateStorage<PetriNetComponent::Semaphore>();
        serializeIdStorage<decltype(state_storage3)>(output, state_storage3);
        auto state_storage4 = transition.template getOutStateStorage<PetriNetComponent::Station>();
        serializeIdStorage<decltype(state_storage4)>(output, state_storage4);
        auto state_storage5 = transition.template getOutStateStorage<PetriNetComponent::InterState>();
        serializeIdStorage<decltype(state_storage5)>(output, state_storage5);
        auto state_storage6 = transition.template getOutStateStorage<PetriNetComponent::Semaphore>();
        serializeIdStorage<decltype(state_storage6)>(output, state_storage6);
        output << "#\n";
    }
}

template <class MarkerType>
void serializeOneTypeMarkers(std::ostream& output, RailwayPetriNet* petriNet)
{
    for (auto it = petriNet->beginMarker<MarkerType>(); it != petriNet->endMarker<MarkerType>(); ++it)
    {
        (*it).second.serialize(output);
        output << "\n";
    }
}

void serialize(std::ostream& output, RailwayPetriNet* petriNet)
{
    output << "Transition::ExitFromStation\n";
    serializeOneTypeTransitions<PetriNetComponent::ExitFromStation>(output, petriNet);
    output << "Transition::EnterToStation\n";
    serializeOneTypeTransitions<PetriNetComponent::EnterToStation>(output, petriNet);
    output << "State::Station\n";
    serializeOneTypeStates<PetriNetComponent::Station>(output, petriNet);
    output << "State::InterState\n";
    serializeOneTypeStates<PetriNetComponent::InterState>(output, petriNet);
    output << "State::Semaphore\n";
    serializeOneTypeStates<PetriNetComponent::Semaphore>(output, petriNet);
    output << "Marker::Train\n";
    serializeOneTypeMarkers<PetriNetComponent::Train>(output, petriNet);
    output << "Marker::AccessToken\n";
    serializeOneTypeMarkers<PetriNetComponent::AccessToken>(output, petriNet);
    output << "FileEnd\n";
}

template <class Storage>
void deserializeIdStorage(std::istream& input, Storage& storage)
{
    std::string line;
    std::getline(input, line, '\n');
    std::stringstream ss(line);
    std::getline(ss, line, '{');
    std::getline(ss, line, '}');
    std::stringstream ss2(line);
    std::string item;
    while (std::getline(ss2, item, ','))
    {
        storage.push_back(std::stoi(item));
    }
}

template <class State>
void deserializeState(std::istream& input, std::string& line, RailwayPetriNet* petriNet)
{
    std::stringstream ss(line);
    int state_id = (int)petriNet->deserializeState<State>(ss);
    auto& stateWrapper = petriNet->getStateWrapperById<State>(state_id);
    auto& train_storage = stateWrapper.template getMarkerStorage<PetriNetComponent::Train>();
    auto& access_token_storage = stateWrapper.template getMarkerStorage<PetriNetComponent::AccessToken>();
    auto& in_transition_storage1 = stateWrapper.template getInTransitionStorage<PetriNetComponent::ExitFromStation>();
    auto& in_transition_storage2 = stateWrapper.template getInTransitionStorage<PetriNetComponent::EnterToStation>();
    auto& out_transition_storage1 = stateWrapper.template getOutTransitionStorage<PetriNetComponent::ExitFromStation>();
    auto& out_transition_storage2 = stateWrapper.template getOutTransitionStorage<PetriNetComponent::EnterToStation>();
    deserializeIdStorage<decltype(train_storage)>(input, train_storage);
    deserializeIdStorage<decltype(access_token_storage)>(input, access_token_storage);
    deserializeIdStorage<decltype(in_transition_storage1)>(input, in_transition_storage1);
    deserializeIdStorage<decltype(in_transition_storage2)>(input, in_transition_storage2);
    deserializeIdStorage<decltype(out_transition_storage1)>(input, out_transition_storage1);
    deserializeIdStorage<decltype(out_transition_storage2)>(input, out_transition_storage2);
    std::getline(input, line, '\n'); // #
}

template <class Transition>
void deserializeTransition(std::istream& input, std::string& line, RailwayPetriNet* petriNet)
{
    std::stringstream ss(line);
    int transition_id = (int)petriNet->deserializeTransition<Transition>(ss);
    auto& transitionWrapper = petriNet->getTransitionWrapperById<Transition>(transition_id);
    auto& in_state_storage1 = transitionWrapper.template getInStateStorage<PetriNetComponent::Station>();
    auto& in_state_storage2 = transitionWrapper.template getInStateStorage<PetriNetComponent::InterState>();
    auto& in_state_storage3 = transitionWrapper.template getInStateStorage<PetriNetComponent::Semaphore>();
    auto& out_state_storage1 = transitionWrapper.template getOutStateStorage<PetriNetComponent::Station>();
    auto& out_state_storage2 = transitionWrapper.template getOutStateStorage<PetriNetComponent::InterState>();
    auto& out_state_storage3 = transitionWrapper.template getOutStateStorage<PetriNetComponent::Semaphore>();
    deserializeIdStorage<decltype(in_state_storage1)>(input, in_state_storage1);
    deserializeIdStorage<decltype(in_state_storage2)>(input, in_state_storage2);
    deserializeIdStorage<decltype(in_state_storage3)>(input, in_state_storage3);
    deserializeIdStorage<decltype(out_state_storage1)>(input, out_state_storage1);
    deserializeIdStorage<decltype(out_state_storage2)>(input, out_state_storage2);
    deserializeIdStorage<decltype(out_state_storage3)>(input, out_state_storage3);
    std::getline(input, line, '\n'); // #
}

template <class StateType>
void deserializeOneTypeStates(std::istream& input, const std::string& separator, RailwayPetriNet* petriNet)
{
    std::string line;
    while (true)
    {
        std::getline(input, line, '\n');
        if (line.compare(separator) == 0)
        {
            break;
        }
        else
        {
            deserializeState<StateType>(input, line, petriNet);
        }
    }
}

template <class TransitionType>
void deserializeOneTypeTransitions(std::istream& input, const std::string& separator, RailwayPetriNet* petriNet)
{
    std::string line;
    while (true)
    {
        std::getline(input, line, '\n');
        if (line.compare(separator) == 0)
        {
            break;
        }
        else
        {
            deserializeTransition<TransitionType>(input, line, petriNet);
        }
    }
}

template <class MarkerType>
void deserializeOneTypeMarkers(std::istream& input, const std::string& separator, RailwayPetriNet* petriNet)
{
    std::string line;
    while (true)
    {
        std::getline(input, line, '\n');
        if (line.compare(separator) == 0)
        {
            break;
        }
        else
        {
            std::stringstream ss(line);
            petriNet->deserializeMarker<MarkerType>(ss);
        }
    }
}

void deserialize(std::istream& input, RailwayPetriNet* petriNet)
{
    std::string line;
    std::getline(input, line, '\n'); // "Transition::ExitFromStation"
    deserializeOneTypeTransitions<PetriNetComponent::ExitFromStation>(input, "Transition::EnterToStation", petriNet);
    deserializeOneTypeTransitions<PetriNetComponent::EnterToStation>(input, "State::Station", petriNet);
    deserializeOneTypeStates<PetriNetComponent::Station>(input, "State::InterState", petriNet);
    deserializeOneTypeStates<PetriNetComponent::InterState>(input, "State::Semaphore", petriNet);
    deserializeOneTypeStates<PetriNetComponent::Semaphore>(input, "Marker::Train", petriNet);
    deserializeOneTypeMarkers<PetriNetComponent::Train>(input, "Marker::AccessToken", petriNet);
    deserializeOneTypeMarkers<PetriNetComponent::AccessToken>(input, "FileEnd", petriNet);
}

#endif //PETRI_NET_SERIALIZATION_H
