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
  systemGoal();
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
      match(Token::Type::PlusOp);
      myASTCurrent->addChild(new ASTNode{myScanner.getCurrentToken()});
      printParse(12);
      break;

    case Token::Type::MinusOp:
      match(Token::Type::MinusOp);
      myASTCurrent->addChild(new ASTNode{myScanner.getCurrentToken()});
      printParse(13);
      break;

    default:
      // TODO: syntax error
      break;
  }
}

//*******************
// Parser::expression
//*******************
void Parser::expression()
{
  auto newNode = new ASTNode{"<primary>"};
  myASTCurrent->addChild(newNode);
  auto savedCurrent = myASTCurrent;
  myASTCurrent = newNode;
  printParse(8);

  primary();
  myASTCurrent = savedCurrent;
  Token peekToken(myScanner.peek());
  if (peekToken.getToken() == Token::Type::PlusOp ||
      peekToken.getToken() == Token::Type::MinusOp)
  {
    auto addNode = new ASTNode{"<add op>"};
    myASTCurrent->addChild(addNode);
    auto exprNode = new ASTNode{"<expression>"};
    myASTCurrent->addChild(exprNode);
    printParse(8);
    myASTCurrent = addNode;
    addOp();
    myASTCurrent = exprNode;
    expression();
  }
}

//*****************
// Parser::exprList
//*****************
void Parser::exprList()
{
  expression();
  Token peekToken(myScanner.peek());
  if (peekToken.getToken() == Token::Type::Comma)
  {
    match(Token::Type::Comma);
    exprList();
  }
}

//**************
// Parser::ident
//**************
void Parser::ident()
{
  match(Token::Type::Id);

  myASTCurrent->addChild(new ASTNode{myScanner.getCurrentToken()});
  printParse(10);
}

//***************
// Parser::idList
//***************
void Parser::idList()
{
  auto newNode = new ASTNode("<ident>");
  myASTCurrent->addChild(newNode);
  myASTCurrent = newNode;
  printParse(6);

  ident();
  Token peekToken(myScanner.peek());
  if (peekToken.getToken() == Token::Type::Comma)
  {
    myASTCurrent->addChild(new ASTNode(","));
    auto newNode = new ASTNode("<idList>");
    myASTCurrent->addChild(newNode);
    myASTCurrent = newNode;
    printParse(6);

    match(Token::Type::Comma);
    idList();
  }
}

//****************
// Parser::primary
//****************
void Parser::primary()
{
  Token peekToken(myScanner.peek());
  switch(peekToken.getToken())
  {
    case Token::Type::LParen:
    {
      auto savedCurrent = myASTCurrent;
      myASTCurrent->addChild(new ASTNode("("));
      auto exprNode = new ASTNode{"<expression>"};
      myASTCurrent->addChild(exprNode);
      myASTCurrent->addChild(new ASTNode{")"});
      printParse(9);

      match(Token::Type::LParen);
      myASTCurrent = exprNode;
      expression();
      match(Token::Type::RParen);
      myASTCurrent = savedCurrent;
    }
    break;

    case Token::Type::Id:
      ident();
      break;

    case Token::Type::IntLiteral:
      myASTCurrent->addChild(new ASTNode(peekToken));
      printParse(11);
      match(Token::Type::IntLiteral);
      break;

    default:
      // TODO: syntax error
      break;
  }
}

//****************
// Parser::program
//****************
void Parser::program()
{
  myASTCurrent->addChild(new ASTNode("begin"));
  auto newNode = new ASTNode("<statement list>");
  myASTCurrent->addChild(newNode);
  myASTCurrent->addChild(new ASTNode("end"));
  myASTCurrent = newNode;
  printParse(1);

  match(Token::Type::BeginSym);
  statementList();
  match(Token::Type::EndSym);
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
      auto newNode1 = new ASTNode("<ident>");
      myASTCurrent->addChild(newNode1);
      myASTCurrent->addChild(new ASTNode(":="));
      auto newNode2 = new ASTNode("<expression>");
      myASTCurrent->addChild(newNode2);
      myASTCurrent->addChild(new ASTNode(";"));
      myASTCurrent = newNode1;
      printParse(3);

      ident();
      match(Token::Type::AssignOp);
      myASTCurrent = newNode2;
      expression();
      match(Token::Type::SemiColon);
    }
    break;

    case Token::Type::ReadSym:
    {
      myASTCurrent->addChild(new ASTNode("Read"));
      myASTCurrent->addChild(new ASTNode("("));
      auto newNode = new ASTNode("<idList>");
      myASTCurrent->addChild(newNode);
      myASTCurrent->addChild(new ASTNode(")"));
      myASTCurrent->addChild(new ASTNode(";"));
      myASTCurrent = newNode;
      printParse(4);

      match(Token::Type::ReadSym);
      match(Token::Type::LParen);
      idList();
      match(Token::Type::RParen);
      match(Token::Type::SemiColon);
    }
    break;

    case Token::Type::WriteSym:
    {
      myASTCurrent->addChild(new ASTNode("Write"));
      myASTCurrent->addChild(new ASTNode("("));
      auto newNode = new ASTNode("<exprList>");
      myASTCurrent->addChild(newNode);
      myASTCurrent->addChild(new ASTNode(")"));
      myASTCurrent->addChild(new ASTNode(";"));
      myASTCurrent = newNode;
      printParse(5);

      match(Token::Type::WriteSym);
      match(Token::Type::LParen);
      exprList();
      match(Token::Type::RParen);
      match(Token::Type::SemiColon);
    }
    break;

    default:
    {
      std::string error{"Expected "};
      error += "Id or WriteSym or ReadSym, instead found " +
        peekToken.getTokenString() + ".";
      myEWTracker.reportError(peekToken, error);
    }
    break;
  }  
}

//**********************
// Parser::statementList
//**********************
void Parser::statementList()
{
  auto newNode = new ASTNode("<statement>");
  myASTCurrent->addChild(newNode);
  myASTCurrent = newNode;
  auto savedCurrent = myASTCurrent;

  printParse(2);

  statement();

  Token peekToken(myScanner.peek());
  switch (peekToken.getToken())
  {
    case Token::Type::Id:
    case Token::Type::ReadSym:
    case Token::Type::WriteSym:
      // TOOD: needs work
      newNode = new ASTNode("<statement list>");
      savedCurrent->addChild(newNode);
      printParse(2);
      myASTCurrent = newNode;

      statementList();
      break;

    default:
      break;
  }
}

//*******************
// Parser::systemGoal
//*******************
void Parser::systemGoal()
{
  auto newNode = new ASTNode{"<program>"};
  myASTCurrent->addChild(newNode);
  myASTCurrent->addChild(new ASTNode{"$"});

  printParse(14, "<system goal>");

  myASTCurrent = newNode;

  program();
  match(Token::Type::EofSym);
}

//**************
// Parser::match
//**************
void Parser::match(const Token::Type &theToken) noexcept
{
  Token nextToken = myScanner.nextToken();
  if (nextToken.getToken() != theToken)
  {
    Token dummyToken{theToken, 0, 0};
    std::string error{"Expected "};
    error += dummyToken.getTokenString() + ", instead found " +
      nextToken.getTokenString() + ".";
    myEWTracker.reportError(nextToken, error);
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
