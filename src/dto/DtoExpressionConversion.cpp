

#include "DtoExpressionConversion.hpp"

NumberConvertable::NumberConvertable(long double val) : Number(val) {
}

void NumberConvertable::convert(oatpp::Fields<oatpp::Any> &fields) const {
    fields->push_back(std::make_pair<oatpp::String, oatpp::String>("type", "Number"));
    fields->push_back(std::make_pair<oatpp::String, oatpp::Float64>("number", this->val));
}


ParenthesisConvertable::ParenthesisConvertable(char symbol) : Parenthesis(symbol) {
}

void ParenthesisConvertable::convert(oatpp::Fields<oatpp::Any> &fields) const {
    fields->push_back(std::make_pair<oatpp::String, oatpp::String>("type", "Parenthesis"));

    auto nestedExp = oatpp::Fields<oatpp::Any>::createShared();
    std::shared_ptr<DtoConvertable> nestedExpPointer = std::dynamic_pointer_cast<DtoConvertable>(this->nestedExp);
    nestedExpPointer->convert(nestedExp);

    fields->push_back(std::make_pair<>("nestedExpression", nestedExp));
}


VariableConvertable::VariableConvertable(const std::string &varName) : Variable(varName) {
}

void VariableConvertable::convert(oatpp::Fields<oatpp::Any> &fields) const {
    fields->push_back(std::make_pair<oatpp::String, oatpp::String>("type", "Variable"));
    fields->push_back(std::make_pair<oatpp::String, oatpp::String>("variableName", this->variableName));
}

ConstantConvertable::ConstantConvertable(const std::string &constantName) : Constant(constantName) {
}

void ConstantConvertable::convert(oatpp::Fields<oatpp::Any> &fields) const {
    fields->push_back(std::make_pair<oatpp::String, oatpp::String>("type", "Constant"));
    fields->push_back(std::make_pair<oatpp::String, oatpp::String>("constantName", this->constantImpl.getName()));
}

PostfixOperationConvertable::PostfixOperationConvertable(const std::string &operatorName) : PostfixOperation(
        operatorName) {
}

void PostfixOperationConvertable::convert(oatpp::Fields<oatpp::Any> &fields) const {
    fields->push_back(std::make_pair<oatpp::String, oatpp::String>("type", "PostfixOperation"));
    fields->push_back(
            std::make_pair<oatpp::String, oatpp::String>("operatorName", this->postfixOperationImpl.getName()));

    auto nestedExp = oatpp::Fields<oatpp::Any>::createShared();
    std::shared_ptr<DtoConvertable> nestedExpPointer = std::dynamic_pointer_cast<DtoConvertable>(this->nestedExp);
    nestedExpPointer->convert(nestedExp);

    fields->push_back(std::make_pair<>("nestedExpression", nestedExp));
}

BinaryOpConvertable::BinaryOpConvertable(std::shared_ptr<Expression> op1, std::shared_ptr<Operation> op)
        : BinaryOperationExpression(
        op1, op) {

}

BinaryOpConvertable::BinaryOpConvertable(std::shared_ptr<Expression> op1, std::shared_ptr<Operation> op,
                                         std::shared_ptr<Expression> op2) : BinaryOperationExpression(op1, op, op2) {

}

void BinaryOpConvertable::convert(oatpp::Fields<oatpp::Any> &fields) const {
    fields->push_back(std::make_pair<oatpp::String, oatpp::String>("type", "Operation"));
    auto op1 = oatpp::Fields<oatpp::Any>::createShared();
    auto op2 = oatpp::Fields<oatpp::Any>::createShared();

    std::shared_ptr<DtoConvertable> opP1 = std::dynamic_pointer_cast<DtoConvertable>(this->op1);
    std::shared_ptr<DtoConvertable> opP2 = std::dynamic_pointer_cast<DtoConvertable>(this->op2);
    opP1->convert(op1);
    opP2->convert(op2);

    fields->push_back(std::make_pair<>("operand1", op1));
    fields->push_back(std::make_pair<>("operand2", op2));
    fields->push_back(std::make_pair<oatpp::String, oatpp::String>("operatorSymbol",
                                                                   this->op ? this->op->toString() : "NO_SYMBOL"));
}

std::shared_ptr<ASTNode> ASTBuilderWeb::toASTNode(TokenType tokenType, std::string data) const {
    switch (tokenType) {
        case TokenType::NUMBER:
            return std::shared_ptr<ASTNode>(new NumberConvertable(std::atoll(data.c_str())));
        case TokenType::OPERATOR:
            return std::shared_ptr<ASTNode>(new Operation(data[0]));
        case TokenType::PARENTHESIS_OPEN:
        case TokenType::PARENTHESIS_CLOSE:
            return std::shared_ptr<ASTNode>(new ParenthesisConvertable(data[0]));
        case TokenType::CONSTANT:
            return std::shared_ptr<ASTNode>(new ConstantConvertable(data));
        case TokenType::VARIABLE:
            return std::shared_ptr<ASTNode>(new VariableConvertable(data));
        case TokenType::POSTFIX_OPERATOR:
            return std::shared_ptr<ASTNode>(new PostfixOperationConvertable(data));
    }
    return nullptr;
}

std::shared_ptr<BinaryOperationExpression> ASTBuilderWeb::createBinaryOperationExpression(
        std::shared_ptr<Expression> op1, std::shared_ptr<Operation> op, std::shared_ptr<Expression> op2) const {
    if (op2 == nullptr) {
        return std::shared_ptr<BinaryOperationExpression>(new BinaryOpConvertable(op1, op));
    } else {
        return std::shared_ptr<BinaryOperationExpression>(new BinaryOpConvertable(op1, op, op2));
    }
}