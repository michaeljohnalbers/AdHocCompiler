/**
 * @file Parser.cpp
 * @brief Implementation of Parser class
 *
 * @author Michael Albers
 */

#include <iomanip>
#include <iostream>
#include <sstream>

#include "ErrorWarningTracker.h"
#include "ExpressionRecord.h"
#include "OperatorRecord.h"
#include "Parser.h"

//***************
// Parser::Parser
//***************
Parser::Parser(Scanner &theScanner,
               CodeGenerator &theGenerator,
               ErrorWarningTracker &theEWTracker) :
  myASTRoot{std::string("<system goal>")},
  myGenerator{theGenerator},
  myEWTracker{theEWTracker},
  myScanner{theScanner}
{
}

//**************
// Parser::parse
//**************
void Parser::parse()
{
  myParentNode.push(&myASTRoot);
  systemGoal();
  myParentNode.pop();
}

//**************
// Parser::addOp
//**************
void Parser::addOp(OperatorRecord &theOperator)
{
  printFunction("addOp");

  Token peekToken(myScanner.peek());
  switch (peekToken.getToken())
  {
    case Token::Type::PlusOp:
      myParentNode.top()->addChild(peekToken);
      printParse(12);
      match(Token::Type::PlusOp);
      theOperator = myGenerator.processOperator(
        myScanner.getCurrentToken().getLiteral());
      break;

    case Token::Type::MinusOp:
      myParentNode.top()->addChild(peekToken);
      printParse(13);
      match(Token::Type::MinusOp);
      theOperator = myGenerator.processOperator(
        myScanner.getCurrentToken().getLiteral());
      break;

    default:
      myEWTracker.reportError(peekToken, 2,
                              Token::Type::PlusOp, Token::Type::MinusOp);
      break;
  }
  myParentNode.pop();
}

//*******************
// Parser::expression
//*******************
void Parser::expression(ExpressionRecord &theExpression)
{
  // ID number for each call of this function (for tracking recursive calls)
  static uint32_t callId = 0;

  ++callId;
  std::string function{"expression[" + std::to_string(callId) + "]"};
  printFunction(function);

  auto primaryNode = myParentNode.top()->addChild("<primary>");
  printParse(8);

  myParentNode.push(primaryNode);
  ExpressionRecord leftOperand;
  primary(leftOperand);

  Token peekToken(myScanner.peek());
  if (peekToken.getToken() == Token::Type::PlusOp ||
      peekToken.getToken() == Token::Type::MinusOp)
  {
    auto addNode = myParentNode.top()->addChild("<add op>");
    auto expressionNode = myParentNode.top()->addChild("<expression>");
    printParse(8);

    myParentNode.push(addNode);
    OperatorRecord operatorRecord(Token::Type::PlusOp);
    addOp(operatorRecord);

    myParentNode.push(expressionNode);
    ExpressionRecord rightOperand;
    expression(rightOperand);

    theExpression = myGenerator.generateInfix(
      leftOperand, operatorRecord, rightOperand);
  }
  else
  {
    theExpression = leftOperand;
  }
  myParentNode.pop();
  std::cout << "Return from expression[" << callId << "]" << std::endl;
  --callId;
}

//*****************
// Parser::exprList
//*****************
void Parser::exprList()
{
  printFunction("exprList");

  auto expressionNode = myParentNode.top()->addChild("<expression>");
  printParse(7);

  myParentNode.push(expressionNode);

  ExpressionRecord expressionRecord;
  expression(expressionRecord);
  myGenerator.writeExpression(expressionRecord);

  Token peekToken(myScanner.peek());
  if (peekToken.getToken() == Token::Type::Comma)
  {
    myParentNode.top()->addChild(",");
    auto exprListNode = myParentNode.top()->addChild("<exprList>");
    myParentNode.push(exprListNode);
    printParse(7);

    match(Token::Type::Comma);
    exprList();
  }
  myParentNode.pop();
}

//**************
// Parser::ident
//**************
void Parser::ident(ExpressionRecord &theIdentifier)
{
  printFunction("ident");

  match(Token::Type::Id);
  theIdentifier = myGenerator.processId(
    myScanner.getCurrentToken().getLiteral());

  myParentNode.top()->addChild(new ASTNode{myScanner.getCurrentToken()});
  printParse(10);
  myParentNode.pop();
}

//***************
// Parser::idList
//***************
void Parser::idList()
{
  printFunction("idList");

  auto identNode = myParentNode.top()->addChild("<ident>");
  printParse(6);

  myParentNode.push(identNode);
  ExpressionRecord identifier;
  ident(identifier);
  myGenerator.readId(identifier);

  Token peekToken(myScanner.peek());
  if (peekToken.getToken() == Token::Type::Comma)
  {
    myParentNode.top()->addChild(",");
    auto idListNode = myParentNode.top()->addChild("<idList>");
    printParse(6);

    myParentNode.push(idListNode);
    match(Token::Type::Comma);
    idList();
  }
  myParentNode.pop();
}

//****************
// Parser::primary
//****************
void Parser::primary(ExpressionRecord &theExpression)
{
  printFunction("primary");

  Token peekToken(myScanner.peek());
  switch (peekToken.getToken())
  {
    case Token::Type::LParen:
    {
      myParentNode.top()->addChild("(");
      auto expressionNode = myParentNode.top()->addChild("<expression>");
      myParentNode.top()->addChild(")");
      printParse(9);

      myParentNode.push(expressionNode);
      match(Token::Type::LParen);
      expression(theExpression);
      match(Token::Type::RParen);
    }
    break;

    case Token::Type::Id:
    {
      auto identNode = myParentNode.top()->addChild("<ident>");
      printParse(9);
      myParentNode.push(identNode);
      ident(theExpression);
    }
    break;

    case Token::Type::IntLiteral:
    {
      myParentNode.top()->addChild(new ASTNode(peekToken));
      printParse(11);
      match(Token::Type::IntLiteral);
      theExpression = ExpressionRecord(
        ExpressionRecord::Type::Literal,
        myScanner.getCurrentToken().getLiteral());
    }
    break;

    default:
      myEWTracker.reportError(peekToken, 3, Token::Type::LParen,
                              Token::Type::Id, Token::Type::IntLiteral);
      break;
  }
  myParentNode.pop();
}

//****************
// Parser::program
//****************
void Parser::program()
{
  printFunction("program");

  myParentNode.top()->addChild("begin");
  auto statementListNode = myParentNode.top()->addChild("<statement list>");
  myParentNode.top()->addChild("end");
  printParse(1);

  myParentNode.push(statementListNode);
  myGenerator.start();
  match(Token::Type::BeginSym);
  statementList();
  match(Token::Type::EndSym);

  myParentNode.pop();
}


//******************
// Parser::statement
//******************
void Parser::statement()
{
  printFunction("statement");

  Token peekToken(myScanner.peek());
  switch (peekToken.getToken())
  {
    case Token::Type::Id:
    {
      auto identNode = myParentNode.top()->addChild("<ident>");
      myParentNode.top()->addChild(":=");
      auto expressionNode = myParentNode.top()->addChild("<expression>");
      myParentNode.top()->addChild(";");
      printParse(3);

      myParentNode.push(identNode);
      ExpressionRecord identifier;
      ident(identifier);

      match(Token::Type::AssignOp);

      myParentNode.push(expressionNode);
      ExpressionRecord expressionRecord;
      expression(expressionRecord);
      myGenerator.assign(identifier, expressionRecord);

      match(Token::Type::SemiColon);
    }
    break;

    case Token::Type::ReadSym:
    {
      myParentNode.top()->addChild("Read");
      myParentNode.top()->addChild("(");
      auto idListNode = myParentNode.top()->addChild("<idList>");
      myParentNode.top()->addChild(")");
      myParentNode.top()->addChild(";");
      printParse(4);

      myParentNode.push(idListNode);
      match(Token::Type::ReadSym);
      match(Token::Type::LParen);
      idList();
      match(Token::Type::RParen);
      match(Token::Type::SemiColon);
    }
    break;

    case Token::Type::WriteSym:
    {
      myParentNode.top()->addChild("Write");
      myParentNode.top()->addChild("(");
      auto exprListNode = myParentNode.top()->addChild("<exprList>");
      myParentNode.top()->addChild(")");
      myParentNode.top()->addChild(";");
      printParse(5);

      myParentNode.push(exprListNode);
      match(Token::Type::WriteSym);
      match(Token::Type::LParen);
      exprList();
      match(Token::Type::RParen);
      match(Token::Type::SemiColon);
    }
    break;

    default:
      myEWTracker.reportError(peekToken, 3, Token::Type::Id,
                              Token::Type::ReadSym, Token::Type::WriteSym);
    break;
  }
  myParentNode.pop();
}

//**********************
// Parser::statementList
//**********************
void Parser::statementList()
{
  printFunction("statementList");

  auto statementNode = myParentNode.top()->addChild("<statement>");
  printParse(2);

  myParentNode.push(statementNode);
  statement();

  Token peekToken(myScanner.peek());
  switch (peekToken.getToken())
  {
    case Token::Type::Id:
    case Token::Type::ReadSym:
    case Token::Type::WriteSym:
    {
      auto statmentListNode = myParentNode.top()->addChild("<statement list>");
      printParse(2);

      myParentNode.push(statmentListNode);
      statementList();
    }
    break;

    default:
      // Nothing to do here. Default needed to suppress warning.
      break;
  }
  myParentNode.pop();
}

//*******************
// Parser::systemGoal
//*******************
void Parser::systemGoal()
{
  printFunction("systemGoal");

  auto programNode = myParentNode.top()->addChild("<program>");
  myParentNode.top()->addChild("$");

  printParse(14, "<system goal>");

  myParentNode.push(programNode);
  program();
  match(Token::Type::EofSym);
  myGenerator.finish();

  myParentNode.pop();
}

//**************
// Parser::match
//**************
void Parser::match(const Token::Type &theToken) noexcept
{
  Token temp(theToken, 0, 0);
  std::string function{"match(" + temp.getTokenString() + ")"};
  printFunction(function);

  Token nextToken = myScanner.nextToken();
  if (nextToken.getToken() != theToken)
  {
    myEWTracker.reportError(nextToken, 1, theToken);
  }
}

//**********************
// Parser::printFunction
//**********************
void Parser::printFunction(const std::string &theFunction)
{
  std::cout << "Call " << std::setw(18) << std::left << theFunction
            << "   Remaining: "
            << myScanner.remainingSource() << std::endl;
}

//*******************
// Parser::printParse
//*******************
void Parser::printParse(uint32_t theProduction,
                        const char *theProductionName) const noexcept
{
  std::ostringstream output;
  output << std::setw(13) << theProductionName << " "
         << std::setw(2) << theProduction << " --> ";
  myASTRoot.traverse(output);
  // Uncomment to print out expansion of productions
  //std::cout << output.str() << std::endl;
}
