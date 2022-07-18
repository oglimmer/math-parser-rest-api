#pragma once

#include "oatpp/core/Types.hpp"

#include "ast/Number.hpp"
#include "ast/Variable.hpp"
#include "ast/Constant.hpp"
#include "ast/Parenthesis.hpp"
#include "ast/PostfixOperation.hpp"
#include "ast/BinaryOperationExpression.hpp"


class DtoConvertable {
public:
    virtual void convert(oatpp::Fields<oatpp::Any> &fields) const = 0;

};

class NumberConvertable : public Number, public DtoConvertable {
public:
    NumberConvertable(long double val);

    void convert(oatpp::Fields<oatpp::Any> &fields) const override;
};

class ParenthesisConvertable : public Parenthesis, public DtoConvertable {
public:
    ParenthesisConvertable(char symbol);

    void convert(oatpp::Fields<oatpp::Any> &fields) const override;
};


class VariableConvertable : public Variable, public DtoConvertable {
public:
    VariableConvertable(const std::string &varName);

    void convert(oatpp::Fields<oatpp::Any> &fields) const override;
};


class ConstantConvertable : public Constant, public DtoConvertable {
public:
    ConstantConvertable(const std::string &constantName);

    void convert(oatpp::Fields<oatpp::Any> &fields) const override;
};

class PostfixOperationConvertable : public PostfixOperation, public DtoConvertable {
public:
    PostfixOperationConvertable(const std::string &operatorName);

    void convert(oatpp::Fields<oatpp::Any> &fields) const override;
};


class BinaryOpConvertable : public BinaryOperationExpression, public DtoConvertable {
public:
    BinaryOpConvertable(std::shared_ptr<Expression> op1, std::shared_ptr<Operation> op);

    BinaryOpConvertable(std::shared_ptr<Expression> op1, std::shared_ptr<Operation> op,
                        std::shared_ptr<Expression> op2);

    void convert(oatpp::Fields<oatpp::Any> &fields) const override;
};

class ASTBuilderWeb : public ASTBuilder {
public:
    std::shared_ptr<ASTNode> toASTNode(TokenType tokenType, std::string data) const;

    std::shared_ptr<BinaryOperationExpression> createBinaryOperationExpression(
            std::shared_ptr<Expression> op1, std::shared_ptr<Operation> op, std::shared_ptr<Expression> op2) const;

};
