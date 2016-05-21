#include "gtest/gtest.h"
#include "Scorp/meta/TypeEnum.h"
#include "Scorp/meta/RuntimeTypeSwitch.h"

class TypeEnumTest : public testing::Test
{
public:
    using TestTypeList = meta::TypeList<int, float, double>;
    using TestTypeEnum = meta::IntTypeEnum<TestTypeList>;
};

TEST_F(TypeEnumTest, ifGetValueInstantiatedWithProperTypeShouldReturnItsIntValue)
{
    EXPECT_EQ(0, TestTypeEnum::getValue<int>());
    EXPECT_EQ(1, TestTypeEnum::getValue<float>());
    EXPECT_EQ(2, TestTypeEnum::getValue<double>());
}

TEST_F(TypeEnumTest, ifGetTypeHandlerInstantiatedWithTypeShouldReturnCorespondType)
{
    EXPECT_TRUE((std::is_same<int, typename decltype(TestTypeEnum::getTypeHolder<0>())::type>::value));
    EXPECT_TRUE((std::is_same<float, typename decltype(TestTypeEnum::getTypeHolder<1>())::type>::value));
    EXPECT_TRUE((std::is_same<double, typename decltype(TestTypeEnum::getTypeHolder<2>())::type>::value));
}

template <class CorrectType>
struct TestFunctor
{
    template <class T>
    void operator()()
    {
        ++numberOfTimesExecuted;
        if (std::is_same<T, CorrectType>::value) {
            correctTypeWasRunned = true;
        }
    }

    size_t numberOfTimesExecuted = 0;
    bool correctTypeWasRunned = false;
};

TEST_F(TypeEnumTest, ifGetTypeHandlerShouldReturnCorespondType)
{
    using CorrectType = int;
    using TypeList = meta::TypeList<double, int, float>;
    using TypeEnum = meta::TypeEnum<TypeList, int>;

    TestFunctor<CorrectType> result = std::move(
        meta::calculateBasedOnRealtime<TestFunctor<CorrectType>, TypeList>(TypeEnum::getValue<CorrectType>()));

    ASSERT_EQ(1, result.numberOfTimesExecuted);
    ASSERT_TRUE(result.correctTypeWasRunned);
}