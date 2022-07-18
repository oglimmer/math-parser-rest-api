#pragma  once

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"


#include OATPP_CODEGEN_BEGIN(DTO)

class TokenElement : public oatpp::DTO {

    DTO_INIT(TokenElement, DTO)

    DTO_FIELD(String, type);

    DTO_FIELD(String, data);

};

class TokenizeResponse : public oatpp::DTO {

    DTO_INIT(TokenizeResponse, DTO)

    DTO_FIELD(List < Object < TokenElement >>, tokens);

};

class ASTResponse : public oatpp::DTO {

    DTO_INIT(ASTResponse, DTO)

    DTO_FIELD(Fields < Any >, rootExpression);

//    DTO_FIELD(Any, rootExpression);//this is not supported by oatpp
};

class CalcResponse : public oatpp::DTO {

    DTO_INIT(CalcResponse, DTO)

    DTO_FIELD(Float64, result);

    DTO_FIELD(String, parsedExpression);

    DTO_FIELD(Object<ASTResponse>, ast);

    DTO_FIELD(List < Object < TokenElement >>, tokens);

    DTO_FIELD(Int32, processingTimeInMicros);

};

#include OATPP_CODEGEN_END(DTO)
