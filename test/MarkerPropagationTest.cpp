#include "gtest/gtest.h"
#include "container/PetriNetTraits.h"
#include "container/PetriNet.h"

class Base
{
public:
    Base(){}

    Base(const std::string m_name) : m_name(m_name) { }


    const std::string& getName() const {
        return m_name;
    }

private:
    std::string m_name;
};

class City : public Base
{
public:
    City() : Base() { }
    City(const std::string m_name) : Base(m_name) { }
};

class InterCity : public Base
{
public:
    InterCity() : Base() { }
    InterCity(const std::string m_name) : Base(m_name) { }
};

class Mutex : public Base
{
public:
    Mutex() : Base() { }
    Mutex(const std::string m_name) : Base(m_name) { }
};

class AccessToken : public Base
{
public:
    AccessToken() : Base() { }
    AccessToken(const std::string m_name) : Base(m_name) { }
};

class Train : public Base
{
public:
    Train() : Base() { }
    Train(const std::string m_name) : Base(m_name) {
    }
};

class ExitCity : public Base
{
public:
    ExitCity() : Base() { }
    ExitCity(const std::string m_name) : Base(m_name) {
    }
};

class EnterCity : public Base
{
public:
    EnterCity() : Base() { }
    EnterCity(const std::string m_name) : Base(m_name) {
    }
};

using MarkerList = meta::TypeList<Train, AccessToken>;
using StateList = meta::TypeList<City, InterCity, Mutex>;
using TransitionList = meta::TypeList<ExitCity, EnterCity>;


class PetriNetMarkerPropagationTest : public testing::Test
{
public:
    using RailwayPetriNetTraits = PetriNetTraits<MarkerList, TransitionList, StateList>;
    using RailwayPetriNet = container::PetriNet<RailwayPetriNetTraits>;
    using IdType = typename RailwayPetriNetTraits::IdType;

    virtual void SetUp() override {
        railwayPetriNet = std::unique_ptr<RailwayPetriNet>(new RailwayPetriNet());
        // State creation
        IdType city1 = railwayPetriNet->addState<City>(City(std::string("City A")));
        IdType city2 = railwayPetriNet->addState<City>(City(std::string("City B")));
        IdType interCity = railwayPetriNet->addState<InterCity>(InterCity(std::string("InterCity")));
        IdType roadMutex = railwayPetriNet->addState<Mutex>(Mutex(std::string("Mutex")));

        // Transition Creation
        IdType cityExit = railwayPetriNet->addTransition<ExitCity>(ExitCity(std::string("ExitCity")));
        IdType cityEnter = railwayPetriNet->addTransition<EnterCity>(EnterCity(std::string("EnterCity")));

        // Marker Creation
        IdType train = railwayPetriNet->addMarker<Train>(city1, Train(std::string("Train")));
        IdType accessToken = railwayPetriNet->addMarker<AccessToken>(roadMutex, AccessToken(std::string("AccessToken")));

        // Linking
        railwayPetriNet->addStateToTransitionConnection<Mutex, ExitCity>(roadMutex, cityExit);
        railwayPetriNet->addStateToTransitionConnection<City, ExitCity>(city1, cityExit);

        railwayPetriNet->addTransitionToStateConnection<ExitCity, InterCity>(cityExit, interCity);
        railwayPetriNet->addStateToTransitionConnection<InterCity, EnterCity>(interCity, cityEnter);

        railwayPetriNet->addTransitionToStateConnection<EnterCity, Mutex>(cityEnter, roadMutex);
        railwayPetriNet->addTransitionToStateConnection<EnterCity, City>(cityEnter, city2);
    }

    virtual void TearDown() override {
        railwayPetriNet.reset();
    }

    std::unique_ptr<RailwayPetriNet> railwayPetriNet;
};

TEST_F(PetriNetMarkerPropagationTest, sunnyCase)
{
    ASSERT_EQ(1, railwayPetriNet->executeMarkersPropagation());
    ASSERT_EQ(1, railwayPetriNet->executeMarkersPropagation());
    ASSERT_EQ(0, railwayPetriNet->executeMarkersPropagation());
    ASSERT_TRUE(true);
}