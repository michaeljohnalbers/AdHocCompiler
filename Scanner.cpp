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
#include <iostream>
#include <stdexcept>

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

  // Just for Assignment 3, can remove
  try
  {
    while (true)
    {
      Token token{readToken()};
      myTokens.push_back(token);
      if (token.getToken() == Token::Type::EofSym)
      {
        break;
      }
    }
  }
  catch (...)
  {
    // Let syntax errors be found by the user when calling getToken.
  }
  myInputStream.clear();
  myInputStream.seekg(0);
  myColumn = 0;
  myLine = 1;
  myPeekTokenPtr = nullptr;
  // Code in nextToken to remove
  // End of Assignment 3 code
}

//*****************
// Scanner::advance
//*****************
void Scanner::advance()
{
  myInputStream.get();
  ++myColumn;
}

//*************************
// Scanner::getCurrentToken
//*************************
Token Scanner::getCurrentToken() const
{
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

//**************************
// Scanner::inspectCharacter
//**************************
char Scanner::inspectCharacter()
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

//*******************
// Scanner::nextToken
//*******************
Token Scanner::nextToken()
{
  myCurrentToken = peek();
  myPeekToken = readToken();
  myTokens.erase(myTokens.begin()); // TODO: assignment 3 code
  return myCurrentToken;
}

//**************
// Scanner::peek
//**************
Token Scanner::peek()
{
  if (nullptr == myPeekTokenPtr)
  {
    myPeekToken = readToken();
    myPeekTokenPtr = &myPeekToken;
  }
  return *myPeekTokenPtr;
}

//***********************
// Scanner::readCharacter
//***********************
char Scanner::readCharacter()
{
  auto character = inspectCharacter();
  advance();
  return character;
}

//*******************
// Scanner::readToken
//*******************
Token Scanner::readToken()
{
  std::string thisBuffer;

  auto addCharacterToBuffer = [&](char theCharacter)
  {
    thisBuffer += theCharacter;
  };

  Token nextToken;
  while (true)
  {
    char currentChar = readCharacter();
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
          currentChar = inspectCharacter();
          if (std::isalnum(currentChar) || '_' == currentChar)
          {
            addCharacterToBuffer(currentChar);
            advance();
          }
          else
          {
            nextToken = idOrReserved(thisBuffer, tokenStartColumn);
            auto tokenLength = nextToken.getLiteral().size();
            if (Token::Type::Id == nextToken.getToken() &&
                tokenLength > MAX_ID_LENGTH)
            {
              syntaxError("Invalid length of " +
                          std::to_string(tokenLength) +
                          " characters for identifier '" +
                          nextToken.getLiteral() +
                          "'. Identifiers can be at most " +
                          std::to_string(MAX_ID_LENGTH) + " characters.");
            }

            return nextToken;
          }
        }
      }
      else if (std::isdigit(currentChar))
      {
        addCharacterToBuffer(currentChar);
        while (true)
        {
          currentChar = inspectCharacter();
          if (std::isdigit(currentChar))
          {
            addCharacterToBuffer(currentChar);
            advance();
          }
          else
          {
            nextToken = Token(Token::Type::IntLiteral, thisBuffer,
                              myLine, tokenStartColumn);
            return nextToken;
          }
        }
      }
      else if ('(' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        nextToken = Token(Token::Type::LParen, thisBuffer,
                          myLine, tokenStartColumn);
        return nextToken;
      }
      else if (')' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        nextToken = Token(Token::Type::RParen, thisBuffer,
                          myLine, tokenStartColumn);
        return nextToken;
      }
      else if (';' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        nextToken = Token(Token::Type::SemiColon, thisBuffer,
                          myLine, tokenStartColumn);
        return nextToken;
      }
      else if (',' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        nextToken = Token(Token::Type::Comma, thisBuffer,
                          myLine, tokenStartColumn);
        return nextToken;
      }
      else if ('+' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        nextToken = Token(Token::Type::PlusOp, thisBuffer,
                          myLine, tokenStartColumn);
        return nextToken;
      }
      else if ('=' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        nextToken = Token(Token::Type::EqualOp, thisBuffer,
                          myLine, tokenStartColumn);
        return nextToken;
      }
      else if ('*' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        currentChar = inspectCharacter();
        if ('*' == currentChar)
        {
          addCharacterToBuffer(currentChar);
          advance();
          nextToken = Token(Token::Type::ExponentOp, thisBuffer,
                            myLine, tokenStartColumn);
          return nextToken;
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
        currentChar = inspectCharacter();
        if ('=' == currentChar)
        {
          addCharacterToBuffer(currentChar);
          advance();
          nextToken = Token(Token::Type::AssignOp, thisBuffer,
                            myLine, tokenStartColumn);
          return nextToken;
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
        char nextChar = inspectCharacter();
        if ('-' == nextChar)
        {
          // Comment, read past it.
          currentChar = nextChar;
          while (currentChar != '\n')
          {
            currentChar = readCharacter();
          }
          newLine();
        }
        else
        {
          addCharacterToBuffer(currentChar);
          nextToken = Token(Token::Type::MinusOp, thisBuffer,
                            myLine, tokenStartColumn);
          return nextToken;
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
      nextToken = Token(Token::Type::EofSym, myLine, 0);
      break;
    }
  }
  return nextToken;
}

//*************************
// Scanner::remainingSource
//*************************
std::string Scanner::remainingSource() const noexcept
{
  std::string remaining;
  for (auto &token : myTokens)
  {
    remaining += token.getLiteral() + " ";
  }
  return remaining;
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
