/**
 * @file Token.cpp
 * @brief Implementation of Token
 *
 * @author Michael Albers
 */

#include "Token.h"

// Static Variables
Token::TokenMap Token::ourTokenDescriptions{
  {Type::BeginSym, "BeginSym"},
  {Type::EndSym, "EndSym"},
  {Type::ReadSym, "ReadSym"},
  {Type::WriteSym, "WriteSym"},
  {Type::Id, "Id"},
  {Type::IntLiteral, "IntLiteral"},
  {Type::LParen, "LParen"},
  {Type::RParen, "RParen"},
  {Type::SemiColon, "SemiColon"},
  {Type::Comma, "Comma"},
  {Type::AssignOp, "AssignOp"},
  {Type::PlusOp, "PlusOp"},
  {Type::MinusOp, "MinusOp"},
  {Type::EqualOp, "EqualOp"},
  {Type::ExponentOp, "ExponentOp"},
  {Type::EofSym, "EofSym"},
};

//*************
// Token::Token
//*************
Token::Token(Type theToken,
             uint32_t theLine,
             uint32_t theColumn) :
  Token(theToken, "", theLine, theColumn)
{
}

//*************
// Token::Token
//*************
Token::Token(Type theToken,
             const std::string &theTokenLiteral,
             uint32_t theLine,
             uint32_t theColumn) :
  myColumn(theColumn),
  myLine(theLine),
  myToken(theToken),
  myTokenLiteral(theTokenLiteral)
{
}

//*****************
// Token::getColumn
//*****************
uint32_t Token::getColumn() const noexcept
{
  return myColumn;
}

//***************
// Token::getLine
//***************
uint32_t Token::getLine() const noexcept
{
  return myLine;
}

//******************
// Token::getLiteral
//******************
std::string Token::getLiteral() const noexcept
{
  return myTokenLiteral;
}

//****************
// Token::getToken
//****************
Token::Type Token::getToken() const noexcept
{
  return myToken;
}

//**********************
// Token::getTokenString
//**********************
std::string Token::getTokenString() const noexcept
{
  return ourTokenDescriptions[getToken()];
}

//*****************
// Token::operator<
//*****************
bool Token::operator<(const Token &theRHS) const noexcept
{
  return getToken() < theRHS.getToken();
}

//******************
// Token::operator==
//******************
bool Token::operator==(const Token &theRHS) const noexcept
{
  return getToken() == theRHS.getToken();
}

//***********
// operator<<
//***********
std::ostream& operator<<(std::ostream &theOS, const Token &theToken)
{
  theOS << theToken.getTokenString()
        << " (\"" << theToken.getLiteral() << "\") on "
        << theToken.getLine() << ":" << theToken.getColumn();
  return theOS;
}
