#pragma  once


#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/protocol/http/incoming/Request.hpp"


#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

#ifndef GIT_COMMIT_HASH
#define GIT_COMMIT_HASH "?"
#endif
#ifndef GIT_BRANCH
#define GIT_BRANCH "?"
#endif
#ifndef GIT_COMMIT_DATE
#define GIT_COMMIT_DATE "?"
#endif

class RootController : public oatpp::web::server::api::ApiController {
public:
    /**
     * Constructor with object mapper.
     * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
     */
    explicit RootController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
            : oatpp::web::server::api::ApiController(objectMapper) {
    }

    ENDPOINT("GET", "/", root) {
        auto resp = createResponse(Status::CODE_301, "");
        resp->putHeader("Location", "/swagger/ui");
        return resp;
    }

    ENDPOINT("GET", "/health", health) {
        return createResponse(Status::CODE_200, "");
    }

    ENDPOINT("GET", "/info", info) {
        std::string responseText("Built from ");
        responseText += GIT_BRANCH;
        responseText += " committed on ";
        responseText += GIT_COMMIT_DATE;
        responseText += " with #";
        responseText += GIT_COMMIT_HASH;
        return createResponse(Status::CODE_200, responseText);
    }

};


#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

