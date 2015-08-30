/**
 * @file ErrorWarningTracker.cpp
 * @brief Implementation of ErrorWarningTracker class
 *
 * @author Michael Albers
 */

#include <cstdarg>
#include <iostream>
#include <sstream>

#include "ErrorWarningTracker.h"
#include "Token.h"

//*********************************
// ErrorWarningTracker::getHasError
//*********************************
ErrorWarningTracker::ErrorWarningTracker(const std::string &theFile) :
  myFile(theFile)
{
}

//*********************************
// ErrorWarningTracker::getHasError
//*********************************
bool ErrorWarningTracker::getHasError() const noexcept
{
  return myHasError;
}

//*********************************
// ErrorWarningTracker::reportError
//*********************************
void ErrorWarningTracker::reportError(Token &theErrorToken,
                                      uint32_t theNumberExpected,
                                      ...)
{
  std::ostringstream errorMessage;
  errorMessage << "Expected ";
  va_list args;
  va_start(args, theNumberExpected);
  for (uint32_t ii = 0; ii < theNumberExpected; ++ii)
  {
    Token tempToken(static_cast<Token::Type>(va_arg(args, int)), 0, 0);
    errorMessage << "" << tempToken.getTokenString();
    if (ii < theNumberExpected - 1)
    {
      errorMessage << " or ";
    }
  }
  va_end(args);
  errorMessage << ". Instead found " << theErrorToken.getTokenString() << ".";
  reportError(theErrorToken, errorMessage.str());
}

//*********************************
// ErrorWarningTracker::reportError
//*********************************
void ErrorWarningTracker::reportError(
  Token &theErrorToken, const std::string &theError) noexcept
{
  myHasError = true;
  // Output modeled off of g++.
  std::cerr << myFile << ":" << theErrorToken.getLine() << ":"
            << theErrorToken.getColumn() << ": error: " << theError
            << std::endl;
}
