#include "gtest/gtest.h"
#include "meta/TypeEnum.h"

class TypeEnumTest : public testing::Test
{
public:
    using TestTypeList = meta::TypeList<int, float, double>;
    using TestTypeEnum = IntTypeEnum<TestTypeList>;
};

TEST_F(TypeEnumTest, ifGetValueSpecializedWithProperTypeShouldReturnItsIntValue)
{
    EXPECT_EQ(0, TestTypeEnum::getValue<int>());
    EXPECT_EQ(1, TestTypeEnum::getValue<float>());
    EXPECT_EQ(2, TestTypeEnum::getValue<double>());
}

TEST_F(TypeEnumTest, ifGetTypeHandlerShouldReturnCorespondType)
{
    EXPECT_TRUE((std::is_same<int, typename decltype(TestTypeEnum::getTypeHandler<0>())::Type>::value));
    EXPECT_TRUE((std::is_same<float, typename decltype(TestTypeEnum::getTypeHandler<1>())::Type>::value));
    EXPECT_TRUE((std::is_same<double, typename decltype(TestTypeEnum::getTypeHandler<2>())::Type>::value));
}