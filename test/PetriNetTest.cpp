#include "gtest/gtest.h"
#include "container/PetriNet.h"

using TestMarkerList = meta::TypeList<int>;
using TestStateList = meta::TypeList<int>;
using TestTransitionList = meta::TypeList<int>;
using TestPetriNetTrait = PetriNetTraits<TestMarkerList, TestStateList, TestTransitionList>;
using TestPetriNet = PetriNet<TestPetriNetTrait>;

class BasePetriNetTest: public testing::Test
{
public:
    using IdType = TestPetriNetTrait::IdType;

    virtual void SetUp() override {
        m_petriNet = std::unique_ptr<TestPetriNet>(new TestPetriNet());
    }

    virtual void TearDown() override {
        m_petriNet.reset();
    }

    std::unique_ptr<TestPetriNet> m_petriNet;
};


class PetriNetStateOperationsTest: public BasePetriNetTest
{
};

TEST_F(PetriNetStateOperationsTest, ifPassedIntStateShouldSuccessfullyInsertStateAndReturnZeroId)
{
    IdType stateId = -1;
    ASSERT_NO_THROW({ stateId = m_petriNet->addState<int>(10); });
    ASSERT_EQ(0, stateId);
}

TEST_F(PetriNetStateOperationsTest, ifInsertSeveralStatesShouldReturnDifferentIds)
{
    IdType state1Id = -1;
    IdType state2Id = -1;
    ASSERT_NO_THROW({ state1Id = m_petriNet->addState<int>(10); });
    ASSERT_NO_THROW({ state2Id = m_petriNet->addState<int>(20); });
    ASSERT_NE(state1Id, state2Id);
}

TEST_F(PetriNetStateOperationsTest, ifPassedIntTransitionShouldSuccessfullyInsertTransitionAndReturnZeroId)
{
    IdType transitionId = -1;
    ASSERT_NO_THROW({ transitionId = m_petriNet->addTransition<int>(10); });
    ASSERT_EQ(0, transitionId);
}

TEST_F(PetriNetStateOperationsTest, ifInsertSeveralTransitionsShouldReturnDifferentIds)
{
    IdType transition1Id = -1;
    IdType transition2Id = -1;
    ASSERT_NO_THROW({ transition1Id = m_petriNet->addTransition<int>(10); });
    ASSERT_NO_THROW({ transition2Id = m_petriNet->addTransition<int>(20); });
    ASSERT_NE(transition1Id, transition2Id);
}

TEST_F(PetriNetStateOperationsTest, ifPassedIntMarkerShouldSuccessfullyInsertMarkerAndReturnZeroId)
{
    IdType stateId = -1;
    ASSERT_NO_THROW({ stateId = m_petriNet->addState<int>(10); });

    IdType markerId = -1;
    ASSERT_NO_THROW({ markerId = m_petriNet->addMarker<int>(stateId, 10); });
    ASSERT_EQ(1, markerId);
}

TEST_F(PetriNetStateOperationsTest, ifInsertSeveralMarkersShouldReturnDifferentIds)
{
    IdType state1Id = -1;
    IdType state2Id = -1;
    ASSERT_NO_THROW({ state1Id = m_petriNet->addState<int>(10); });
    ASSERT_NO_THROW({ state2Id = m_petriNet->addState<int>(20); });

    IdType marker1Id = -1;
    IdType marker2Id = -1;
    ASSERT_NO_THROW({ marker1Id = m_petriNet->addMarker<int>(state1Id, 30); });
    ASSERT_NO_THROW({ marker2Id = m_petriNet->addMarker<int>(state2Id, 40); });
    ASSERT_NE(marker1Id, marker2Id);
}
