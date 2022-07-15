#pragma  once

#include "oatpp-test/UnitTest.hpp"

class CalcControllerTest : public oatpp::test::UnitTest {
public:

    CalcControllerTest() : UnitTest("TEST[CalcControllerTest]") {}

    void onRun() override;

};
