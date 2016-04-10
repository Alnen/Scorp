#include "gtest/gtest.h"
#include "container/PetriNet.h"

using TestMarkerList = TypeList<int>;
using TestStateList = TypeList<int>;
using TestTransitionList = TypeList<int>;
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