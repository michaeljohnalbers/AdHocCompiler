/**
 * @file Scanner.cpp
 * @brief Implementation of Scanner class
 *
 * @author Michael Albers
 */

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <iostream>

#include "Scanner.h"

// Static Variables
Scanner::ReservedWordMap Scanner::ourReservedWords{
  {"begin", Token::Type::BeginSym},
  {"end", Token::Type::EndSym},
  {"read", Token::Type::ReadSym},
  {"write", Token::Type::WriteSym}};


//*****************
// Scanner::Scanner
//*****************
Scanner::Scanner(const std::string &theFile) :
  myFile(theFile)
{
  myInputStream.open(myFile, std::ios::in);
  if (! myInputStream)
  {
    auto localErrno = errno;
    throw std::runtime_error("Failed to open '" + myFile + "': " +
                             std::strerror(localErrno));
  }
}

//******************************
// Scanner::addCharacterToBuffer
//******************************
void Scanner::addCharacterToBuffer(char theCharacter)
{
  myBuffer += theCharacter;
}

//*********************
// Scanner::clearBuffer
//*********************
void Scanner::advance()
{
  myInputStream.get();
  ++myColumn;
}

//*********************
// Scanner::clearBuffer
//*********************
void Scanner::clearBuffer()
{
  myBuffer.clear();
}

//*************************
// Scanner::getCurrentToken
//*************************
Token Scanner::getCurrentToken() const
{
  return myCurrentToken;
}

//**********************
// Scanner::getNextToken
//**********************
Token Scanner::getNextToken()
{
  clearBuffer();

  while (true)
  {
    char currentChar = read();
    if (! myInputStream.eof())
    {
      // Start of (potential) new token

      uint32_t tokenStartColumn = myColumn;

      if (std::isspace(currentChar))
      {
        if ('\n' == currentChar)
        {
          newLine();
        }
      }
      else if (std::isalpha(currentChar))
      {
        addCharacterToBuffer(currentChar);
        while (true)
        {
          currentChar = inspect();
          if (std::isalnum(currentChar) || '_' == currentChar)
          {
            addCharacterToBuffer(currentChar);
            advance();
          }
          else
          {
            myCurrentToken = idOrReserved(myBuffer, tokenStartColumn);
            auto tokenLength = myCurrentToken.getLiteral().size();
            if (Token::Type::Id == myCurrentToken.getToken() &&
                tokenLength > 32)
            {
              syntaxError("Invalid length of " +
                          std::to_string(tokenLength) +
                          " characters for identifier '" +
                          myCurrentToken.getLiteral() +
                          "'. Identifiers can be at most 32 characters.");
            }

            return myCurrentToken;
          }
        }
      }
      else if (std::isdigit(currentChar))
      {
        addCharacterToBuffer(currentChar);
        while (true)
        {
          currentChar = inspect();
          if (std::isdigit(currentChar))
          {
            addCharacterToBuffer(currentChar);
            advance();
          }
          else
          {
            myCurrentToken = Token(
              Token::Type::IntLiteral, myBuffer, myLine, tokenStartColumn);
            return myCurrentToken;
          }
        }
      }
      else if ('(' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        myCurrentToken = Token(Token::Type::LParen, myBuffer, myLine, tokenStartColumn);
        return myCurrentToken;
      }
      else if (')' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        myCurrentToken = Token(Token::Type::RParen, myBuffer, myLine, tokenStartColumn);
        return myCurrentToken;
      }
      else if (';' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        myCurrentToken = Token(Token::Type::SemiColon, myBuffer, myLine, tokenStartColumn);
        return myCurrentToken;
      }
      else if (',' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        myCurrentToken = Token(Token::Type::Comma, myBuffer, myLine, tokenStartColumn);
        return myCurrentToken;
      }
      else if ('+' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        myCurrentToken = Token(Token::Type::PlusOp, myBuffer, myLine, tokenStartColumn);
        return myCurrentToken;
      }
      else if ('=' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        myCurrentToken = Token(Token::Type::EqualOp, myBuffer, myLine, tokenStartColumn);
        return myCurrentToken;
      }
      else if ('*' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        currentChar = inspect();
        if ('*' == currentChar)
        {
          addCharacterToBuffer(currentChar);
          advance();
          myCurrentToken = Token(Token::Type::ExponentOp, myBuffer, myLine, tokenStartColumn);
          return myCurrentToken;
        }
        else
        {
          std::string error {"Expected '*' after '*'. Instead found '"};
          error += currentChar + std::string{"'."};
          syntaxError(error);
        }
      }
      else if (':' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        currentChar = inspect();
        if ('=' == currentChar)
        {
          addCharacterToBuffer(currentChar);
          advance();
          myCurrentToken = Token(Token::Type::AssignOp, myBuffer, myLine, tokenStartColumn);
          return myCurrentToken;
        }
        else
        {
          std::string error {"Expected '=' after ':'. Instead found '"};
          error += currentChar + std::string{"'."};
          syntaxError(error);
        }
      }
      else if ('-' == currentChar)
      {
        char nextChar = inspect();
        if ('-' == nextChar)
        {
          // Comment, read past it.
          currentChar = nextChar;
          while (currentChar != '\n')
          {
            currentChar = read();
          }
          newLine();
        }
        else
        {
          addCharacterToBuffer(currentChar);
          myCurrentToken = Token(Token::Type::MinusOp, myBuffer, myLine, tokenStartColumn);
          return myCurrentToken;
        }
      }
      else
      {
        std::string error{"Read unexpected character '"};
        error += currentChar + std::string("' (ASCII decimal ") +
          std::to_string(currentChar) + ").";
        syntaxError(error);
      }
    }
    else
    {
      myCurrentToken = Token(Token::Type::EofSym, myLine, 0);
      break;
    }
  }
  return myCurrentToken;
}

//**********************
// Scanner::idOrReserved
//**********************
Token Scanner::idOrReserved(const std::string &theTokenLiteral,
                            uint32_t theTokenStartColumn)
{
  std::string token {theTokenLiteral};
  std::transform(token.begin(), token.end(), token.begin(), ::tolower);

  Token idOrReserved(
    Token::Type::Id, theTokenLiteral, myLine, theTokenStartColumn);

  auto reservedIter = ourReservedWords.find(token);
  if (reservedIter != ourReservedWords.end())
  {
    idOrReserved = Token(
      reservedIter->second, theTokenLiteral, myLine, theTokenStartColumn);
  }

  return idOrReserved;
}

//*****************
// Scanner::inspect
//*****************
char Scanner::inspect()
{
  return myInputStream.peek();
}

//*****************
// Scanner::newLine
//*****************
void Scanner::newLine()
{
  ++myLine;
  myColumn = 0;
}

//**************
// Scanner::read
//**************
char Scanner::read()
{
  auto character = inspect();
  advance();
  return character;
}

//*********************
// Scanner::syntaxError
//*********************
void Scanner::syntaxError(const std::string &theError)
{
  std::string error;
  error += myFile + ":" + std::to_string(myLine) + ":" +
    std::to_string(myColumn) + ": error: " + theError;
  throw std::runtime_error{error};
}
