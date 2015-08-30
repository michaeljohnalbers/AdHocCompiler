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
#include "Parser.h"

//***************
// Parser::Parser
//***************
Parser::Parser(Scanner &theScanner,
               ErrorWarningTracker &theEWTracker) :
  myASTRoot(std::string("<system goal>")),
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
void Parser::addOp()
{
  Token peekToken(myScanner.peek());
  switch (peekToken.getToken())
  {
    case Token::Type::PlusOp:
      myParentNode.top()->addChild(peekToken);
      printParse(12);
      match(Token::Type::PlusOp);
      break;

    case Token::Type::MinusOp:
      myParentNode.top()->addChild(peekToken);
      printParse(13);
      match(Token::Type::MinusOp);
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
void Parser::expression()
{
  auto primaryNode = myParentNode.top()->addChild("<primary>");
  printParse(8);

  myParentNode.push(primaryNode);
  primary();

  Token peekToken(myScanner.peek());
  if (peekToken.getToken() == Token::Type::PlusOp ||
      peekToken.getToken() == Token::Type::MinusOp)
  {
    auto addNode = myParentNode.top()->addChild("<add op>");
    auto expressionNode = myParentNode.top()->addChild("<expression>");
    printParse(8);

    myParentNode.push(addNode);
    addOp();
    myParentNode.push(expressionNode);
    expression();
  }
  myParentNode.pop();
}

//*****************
// Parser::exprList
//*****************
void Parser::exprList()
{
  auto expressionNode = myParentNode.top()->addChild("<expression>");
  printParse(7);

  myParentNode.push(expressionNode);
  expression();

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
void Parser::ident()
{
  match(Token::Type::Id);

  myParentNode.top()->addChild(new ASTNode{myScanner.getCurrentToken()});
  printParse(10);
  myParentNode.pop();
}

//***************
// Parser::idList
//***************
void Parser::idList()
{
  auto identNode = myParentNode.top()->addChild("<ident>");
  printParse(6);

  myParentNode.push(identNode);
  ident();

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
void Parser::primary()
{
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
      expression();
      match(Token::Type::RParen);
    }
    break;

    case Token::Type::Id:
    {
      auto identNode = myParentNode.top()->addChild("<ident>");
      printParse(9);
      myParentNode.push(identNode);
      ident();
    }
    break;

    case Token::Type::IntLiteral:
    {
      myParentNode.top()->addChild(new ASTNode(peekToken));
      printParse(11);
      match(Token::Type::IntLiteral);
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
  myParentNode.top()->addChild("begin");
  auto statementListNode = myParentNode.top()->addChild("<statement list>");
  myParentNode.top()->addChild("end");
  printParse(1);

  myParentNode.push(statementListNode);
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
      ident();
      match(Token::Type::AssignOp);
      myParentNode.push(expressionNode);
      expression();
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
  auto programNode = myParentNode.top()->addChild("<program>");
  myParentNode.top()->addChild("$");

  printParse(14, "<system goal>");

  myParentNode.push(programNode);
  program();
  match(Token::Type::EofSym);

  myParentNode.pop();
}

//**************
// Parser::match
//**************
void Parser::match(const Token::Type &theToken) noexcept
{
  Token nextToken = myScanner.nextToken();
  if (nextToken.getToken() != theToken)
  {
    myEWTracker.reportError(nextToken, 1, theToken);
  }
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
  std::cout << output.str() << std::endl;
}
