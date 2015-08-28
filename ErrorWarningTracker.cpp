/**
 * @file ErrorWarningTracker.cpp
 * @brief Implementation of ErrorWarningTracker class
 *
 * @author Michael Albers
 */

#include <iostream>

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
void ErrorWarningTracker::reportError(
  Token &theErrorToken, const std::string &theError) noexcept
{
  myHasError = true;
  // Output modeled off of g++.
  std::cerr << myFile << ":" << theErrorToken.getLine() << ":"
            << theErrorToken.getColumn() << ": error: " << theError
            << std::endl;
}
