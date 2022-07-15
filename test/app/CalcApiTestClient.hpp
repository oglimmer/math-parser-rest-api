#pragma  once


#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"

/* Begin Api Client code generation */
#include OATPP_CODEGEN_BEGIN(ApiClient)

/**
 * CreatePlayer API client.
 * Use this client to call application APIs.
 */
class CalcApiTestClient : public oatpp::web::client::ApiClient {

API_CLIENT_INIT(CalcApiTestClient)

    API_CALL("GET", "/v1/calc", calcWithParam, QUERY(String, expression))

    API_CALL("GET", "/v1/calc", calcNoParam)

    API_CALL("GET", "/v1/calc", calcWithXVariable, QUERY(String, expression), QUERY(String, x))

};

/* End Api Client code generation */
#include OATPP_CODEGEN_END(ApiClient)
