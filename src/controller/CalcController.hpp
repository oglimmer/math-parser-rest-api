#pragma  once

#include "dto/DTOs.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/protocol/http/incoming/Request.hpp"

#include "mathparser/InvalidFormulaException.hpp"
#include "mathparser/FunctionParser.hpp"
#include "mathparser/ast/Expression.hpp"


std::string urlDecode(const std::string &src) {
    std::string ret;
    char ch;
    int i, ii;
    for (i = 0; i < src.length(); i++) {
        if (int(src[i]) == 37) {
            sscanf(src.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            ret += ch;
            i = i + 2;
        } else {
            ret += src[i];
        }
    }
    return (ret);
}


class CustomErrorHandler : public oatpp::base::Countable, public oatpp::web::server::handler::ErrorHandler {
public:
    CustomErrorHandler() = default;

public:
    static std::shared_ptr<CustomErrorHandler> createShared() {
        return std::make_shared<CustomErrorHandler>();
    }

    std::shared_ptr<oatpp::web::protocol::http::outgoing::Response>
    handleError(const std::exception_ptr &error) override {
        try {
            std::rethrow_exception(error);
        } catch (const oatpp::web::protocol::CommunicationError &e) {
            return oatpp::web::protocol::http::outgoing::ResponseFactory::createResponse(
                    oatpp::web::protocol::http::Status::CODE_400, e.what());
        } catch (const std::runtime_error &e) {
            return oatpp::web::protocol::http::outgoing::ResponseFactory::createResponse(
                    oatpp::web::protocol::http::Status::CODE_500, e.what());
        } catch (...) {
            throw;
        }
    }

    std::shared_ptr<oatpp::web::protocol::http::outgoing::Response>
    handleError(const oatpp::web::protocol::http::Status &status, const oatpp::String &message,
                const Headers &headers) override {
        throw std::logic_error("Function not implemented");
    }

};

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen


class CalcController : public oatpp::web::server::api::ApiController {
public:
    /**
     * Constructor with object mapper.
     * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
     */
    explicit CalcController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
            : oatpp::web::server::api::ApiController(objectMapper) {
        setErrorHandler(CustomErrorHandler::createShared());
    }

public:

    ADD_CORS(calc)

    ENDPOINT_INFO(calc) {
        info->summary = "Resolves an expression";
        info->addResponse<Float64>(Status::CODE_200, "text/plain");
        info->queryParams.add<String>("expression").description = "The math function to parse and resolve, e.g. 3+4";
        info->queryParams["expression"].required = true;
        info->queryParams.add<String>("x").description = "Variable x";
        info->queryParams["expression"].required = false;
    }

    ENDPOINT("GET", "/v1/calc", calc, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        String tail = request->getPathTail();
        auto queryParams = oatpp::network::Url::Parser::parseQueryParams(tail);
        std::map<std::string, long double> vars;
        for (auto element: queryParams.getAll()) {
            if (element.first != "expression") {
                try {
                    long double val = std::stold(element.second.std_str());
                    vars.insert(std::make_pair(element.first.std_str(), val));
                } catch (const std::invalid_argument &e) {
                    // just ignore the parameter
                }
            }
        }
        auto expression = queryParams.get("expression");
        if (!expression || expression.getPtr()->empty()) {
            return createResponse(Status::CODE_400,
                                  "You have to specify the query parameter expression, e.g. expression=3+4");
        }
        expression = urlDecode(expression);
        try {
            FunctionParser fp;
            OATPP_LOGI("CalcController", "input='%s'", expression.getValue("<<EMPTY>>").c_str());
            std::unique_ptr<std::vector<std::shared_ptr<Token>>> tokens = fp.tokenize(expression);

            std::shared_ptr<Expression> exp = fp.tokensToExpression(tokens);

            return createResponse(Status::CODE_200, std::to_string(exp->resolve(vars)));
        } catch (const InvalidFormulaException &e) {
            return createResponse(Status::CODE_400, e.what());
        }
    }


};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen
