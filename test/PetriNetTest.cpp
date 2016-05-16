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

TEST_F(PetriNetStateOperationsTest, ifPassedStateIdShouldReturnCorrectState)
{
    IdType stateId = -1;
    ASSERT_NO_THROW({ stateId = m_petriNet->addState<int>(10); });
    ASSERT_EQ(10, m_petriNet->getStateById<int>(stateId));

}

TEST_F(PetriNetStateOperationsTest, ifPassedTransitionIdShouldReturnCorrectTransition)
{
    IdType transitionId = -1;
    ASSERT_NO_THROW({ transitionId = m_petriNet->addTransition<int>(10); });
    ASSERT_EQ(10, m_petriNet->getTransitionById<int>(transitionId));

}

TEST_F(PetriNetStateOperationsTest, ifPassedMarkerIdShouldReturnCorrectMarker)
{
    IdType stateId = -1;
    ASSERT_NO_THROW({ stateId = m_petriNet->addState<int>(10); });

    IdType markerId = -1;
    ASSERT_NO_THROW({ markerId = m_petriNet->addMarker<int>(stateId, 30); });

    ASSERT_EQ(30, m_petriNet->getMarkerById<int>(markerId));
}

TEST_F(PetriNetStateOperationsTest, CheckInitialSizeOfStatesWithInt)
{
    ASSERT_EQ(0, m_petriNet->sizeState<int>());
}

TEST_F(PetriNetStateOperationsTest, CheckInitialSizeOfTransitionsWithInt)
{
    ASSERT_EQ(0, m_petriNet->sizeTransition<int>());
}

TEST_F(PetriNetStateOperationsTest, CheckInitialSizeOfMarkersWithInt)
{
    ASSERT_EQ(0, m_petriNet->sizeMarker<int>());
}

TEST_F(PetriNetStateOperationsTest, CheckSizeOfStatesWithIntAfterInsertion)
{
    IdType stateId = -1;
    ASSERT_NO_THROW({ stateId = m_petriNet->addState<int>(10); });
    ASSERT_EQ(1, m_petriNet->sizeState<int>());
}

TEST_F(PetriNetStateOperationsTest, CheckSizeOfTransitionsWithIntAfterInsertion)
{
    IdType transitionId = -1;
    ASSERT_NO_THROW({ transitionId = m_petriNet->addTransition<int>(10); });
    ASSERT_EQ(1, m_petriNet->sizeTransition<int>());
}

TEST_F(PetriNetStateOperationsTest, CheckSizeOfMarkersWithIntAfterInsertion)
{
    IdType stateId = -1;
    ASSERT_NO_THROW({ stateId = m_petriNet->addState<int>(10); });

    IdType markerId = -1;
    ASSERT_NO_THROW({ markerId = m_petriNet->addMarker<int>(stateId, 10); });

    ASSERT_EQ(1, m_petriNet->sizeMarker<int>());
}

TEST_F(PetriNetStateOperationsTest, CheckSizeOfStatesWithIntAfterRemove)
{
    IdType stateId1 = -1;
    IdType stateId2 = -1;
    ASSERT_NO_THROW({ stateId1 = m_petriNet->addState<int>(10); });
    ASSERT_NO_THROW({ stateId2 = m_petriNet->addState<int>(20); });
    ASSERT_EQ(2, m_petriNet->sizeState<int>());
    m_petriNet->removeState<int>(stateId1);
    ASSERT_EQ(1, m_petriNet->sizeState<int>());
}

TEST_F(PetriNetStateOperationsTest, CheckSizeOfTransitionsWithIntAfterRemove)
{
    IdType transitionId1 = -1;
    IdType transitionId2 = -1;
    ASSERT_NO_THROW({ transitionId1 = m_petriNet->addTransition<int>(10); });
    ASSERT_NO_THROW({ transitionId2 = m_petriNet->addTransition<int>(20); });
    ASSERT_EQ(2, m_petriNet->sizeTransition<int>());
    m_petriNet->removeTransition<int>(transitionId1);
    ASSERT_EQ(1, m_petriNet->sizeTransition<int>());

}

TEST_F(PetriNetStateOperationsTest, CheckSizeOfMarkersWithIntAfterRemove)
{
    IdType stateId1 = -1;
    IdType stateId2 = -1;
    ASSERT_NO_THROW({ stateId1 = m_petriNet->addState<int>(10); });
    ASSERT_NO_THROW({ stateId2 = m_petriNet->addState<int>(20); });

    IdType markerId1 = -1;
    IdType markerId2 = -1;
    ASSERT_NO_THROW({ markerId1 = m_petriNet->addMarker<int>(stateId1, 10); });
    ASSERT_NO_THROW({ markerId2 = m_petriNet->addMarker<int>(stateId2, 10); });

    ASSERT_EQ(2, m_petriNet->sizeMarker<int>());
    m_petriNet->removeMarker<int>(markerId1);
    ASSERT_EQ(1, m_petriNet->sizeMarker<int>());
}
