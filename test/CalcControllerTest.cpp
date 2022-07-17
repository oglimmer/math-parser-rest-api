#include "CalcControllerTest.hpp"

#include "controller/CalcController.hpp"

#include "app/CalcApiTestClient.hpp"
#include "app/TestComponent.hpp"

#include "oatpp/web/client/HttpRequestExecutor.hpp"

#include "oatpp-test/web/ClientServerTestRunner.hpp"

void CalcControllerTest::onRun() {

    TestComponent component;

    oatpp::test::web::ClientServerTestRunner runner;

    runner.addController(std::make_shared<CalcController>());

    runner.run([this, &runner] {

        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>, clientConnectionProvider);
        OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);

        auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(clientConnectionProvider);

        auto client = CalcApiTestClient::createShared(requestExecutor, objectMapper);

        {
            auto calcResponse = client->calcWithParam("3+1");
            OATPP_ASSERT(calcResponse->getStatusCode() == 200);
            auto result = calcResponse->readBodyToDto<oatpp::Object<CalcResponse >>(objectMapper.get());
            OATPP_ASSERT(result);
            OATPP_ASSERT(result->result.getValue(-1) == 4);
        }

        {
            auto calcResponse = client->calcNoParam();
            OATPP_ASSERT(calcResponse->getStatusCode() == 400);
            auto result = calcResponse->readBodyToString();
            OATPP_ASSERT(result);
            OATPP_ASSERT(result == "You have to specify the query parameter expression, e.g. https://math.oglimmer.de/v1/calc?expression=sqrt(2^3+1)");
        }

        {
            auto calcResponse = client->calcWithParam("");
            OATPP_ASSERT(calcResponse->getStatusCode() == 400);
            auto result = calcResponse->readBodyToString();
            OATPP_ASSERT(result);
            OATPP_ASSERT(result == "You have to specify the query parameter expression, e.g. https://math.oglimmer.de/v1/calc?expression=sqrt(2^3+1)");
        }

        {
            auto calcResponse = client->calcWithXVariable("3+1+x", "4");
            OATPP_ASSERT(calcResponse->getStatusCode() == 200);
            auto result = calcResponse->readBodyToDto<oatpp::Object<CalcResponse >>(objectMapper.get());
            OATPP_ASSERT(result);
            OATPP_ASSERT(result->result.getValue(-1) == 8);
        }

    }, std::chrono::minutes(10) /* test timeout */);

    std::this_thread::sleep_for(std::chrono::seconds(1));

}
