#pragma  once

#include "dto/DTOs.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/protocol/http/incoming/Request.hpp"

#include "mathparser/InvalidFormulaException.hpp"
#include "mathparser/FunctionParser.hpp"
#include "mathparser/ast/Expression.hpp"
#include "mathparser/tokenize/Token.hpp"

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

    ENDPOINT_INFO(resolve) {
        info->summary = "Resolves an expression";
        info->addResponse<Float64>(Status::CODE_200, "text/plain");
        info->queryParams.add<String>("expression").description = "The math function to parse and resolve, e.g. 3+4";
        info->queryParams.add<String>("x").description = "Variable x";
        info->queryParams["x"].required = false;
    }

    ENDPOINT("GET", "/v1/resolve", resolve, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        try {
            oatpp::network::Url::Parameters queryParams = oatpp::network::Url::Parser::parseQueryParams(
                    request->getPathTail());
            auto expression = getInputExpression(queryParams);
            std::map<std::string, long double> vars = prepareVars(queryParams);
            FunctionParser fp;
            std::unique_ptr<std::vector<std::shared_ptr<Token>>> tokens = fp.tokenize(expression);

            std::shared_ptr<Expression> exp = fp.tokensToExpression(tokens);

            long double resultVal = exp->resolve(vars);
            return createResponse(Status::CODE_200, std::to_string(resultVal));
        } catch (const InvalidFormulaException &e) {
            return createResponse(Status::CODE_400, e.what());
        }
    }

    ENDPOINT_INFO(tokenize) {
        info->summary = "Tokenizes an expression";
        info->addResponse < Object < TokenizeResponse >> (Status::CODE_200, "application/json");
        info->queryParams.add<String>("expression").description = "The math function to parse and resolve, e.g. 3+4";
        info->queryParams.add<String>("x").description = "Variable x";
        info->queryParams["x"].required = false;
    }

    ENDPOINT("GET", "/v1/tokenize", tokenize, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        try {
            oatpp::network::Url::Parameters queryParams = oatpp::network::Url::Parser::parseQueryParams(
                    request->getPathTail());
            auto expression = getInputExpression(queryParams);
            std::map<std::string, long double> vars = prepareVars(queryParams);
            FunctionParser fp;
            std::unique_ptr<std::vector<std::shared_ptr<Token>>> tokens = fp.tokenize(expression);
            auto response = TokenizeResponse::createShared();
            response->tokens = {};
            for (auto it = tokens->begin(); it != tokens->end(); ++it) {
                auto token = TokenElement::createShared();
                std::shared_ptr<Token> &tokenElement = *it;
                token->data = tokenElement->getData();
                token->type = tokenElement->getType();
                response->tokens->push_back(token);
            }
            return createDtoResponse(Status::CODE_200, response);
        } catch (const InvalidFormulaException &e) {
            return createResponse(Status::CODE_400, e.what());
        }
    }

    ADD_CORS(calc)

    ENDPOINT_INFO(calc) {
        info->summary = "Tokenizes, parses and resolves an expression";
        info->addResponse < Object < CalcResponse >> (Status::CODE_200, "application/json");
        info->queryParams.add<String>("expression").description = "The math function to parse and resolve, e.g. 3+4";
        info->queryParams.add<String>("x").description = "Variable x";
        info->queryParams["x"].required = false;
    }

    ENDPOINT("GET", "/v1/calc", calc, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        auto startTime = std::chrono::system_clock::now();

        try {
            oatpp::network::Url::Parameters queryParams = oatpp::network::Url::Parser::parseQueryParams(
                    request->getPathTail());
            std::map<std::string, long double> vars = prepareVars(queryParams);
            auto expression = getInputExpression(queryParams);
            FunctionParser fp;
            std::unique_ptr<std::vector<std::shared_ptr<Token>>> tokens = fp.tokenize(expression);

            std::shared_ptr<Expression> exp = fp.tokensToExpression(tokens);

            long double resultVal = exp->resolve(vars);
            auto calcResponse = CalcResponse::createShared();
            calcResponse->result = resultVal;
            calcResponse->tokens = {};
            for (auto it = tokens->begin(); it != tokens->end(); ++it) {
                auto token = TokenElement::createShared();
                std::shared_ptr<Token> &tokenElement = *it;
                token->data = tokenElement->getData();
                token->type = tokenElement->getType();
                calcResponse->tokens->push_back(token);
            }
            calcResponse->parsedExpression = exp->toString();
            auto endTime = std::chrono::system_clock::now();
            auto durationInMicros = std::chrono::duration_cast<std::chrono::microseconds>((endTime - startTime));
            calcResponse->processingTimeInMicros = durationInMicros.count();
            return createDtoResponse(Status::CODE_200, calcResponse);
        } catch (const InvalidFormulaException &e) {
            return createResponse(Status::CODE_400, e.what());
        }
    }

    std::map<std::string, long double> prepareVars(oatpp::network::Url::Parameters queryParams) const {
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
        return vars;
    }

    std::string getInputExpression(oatpp::network::Url::Parameters queryParams) const {
        auto expression = queryParams.get("expression");
        if (!expression || expression.getPtr()->empty()) {
            throw InvalidFormulaException(
                    "You have to specify the query parameter expression, e.g. https://math.oglimmer.de/v1/calc?expression=sqrt(2^3+1)");
        }
        expression = urlDecode(expression);
        OATPP_LOGI("CalcController", "input='%s'", expression.getValue("<<EMPTY>>").c_str());
        return expression;
    }


};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen
