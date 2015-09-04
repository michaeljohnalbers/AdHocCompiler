/**
 * @file CodeGenerator.cpp
 * @brief Implementation of CodeGenerator class
 *
 * @author Michael Albers
 */

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "CodeGenerator.h"
#include "ErrorWarningTracker.h"
#include "ExpressionRecord.h"
#include "OperatorRecord.h"

//**************************************************
// CodeGenerator::CodeGenerator
//**************************************************
CodeGenerator::CodeGenerator(ErrorWarningTracker &theEWTracker) :
  myEWTracker(theEWTracker)
{
}

//**************************************************
// CodeGenerator::~CodeGenerator
//**************************************************
CodeGenerator::~CodeGenerator()
{
  std::cout << std::endl
            << "Complete Code" << std::endl
            << "-------------" << std::endl
            << myGeneratedCode;
}

//**************************************************
// CodeGenerator::assign
//**************************************************
void CodeGenerator::assign(const ExpressionRecord &theSource,
                           const ExpressionRecord &theDestination) noexcept
{
  std::cout << "Call assign" << std::endl;
  generate("Store", theSource.getValue(), theDestination.getValue());
}

//**************************************************
// CodeGenerator::checkId
//**************************************************
void CodeGenerator::checkId(const std::string &theIdentifier) noexcept
{
  std::cout << "Call checkId" << std::endl;
  if (false == lookUp(theIdentifier))
  {
    enter(theIdentifier);
    generate("Declare", theIdentifier, "Integer");
  }
}

//**************************************************
// CodeGenerator::enter
//**************************************************
void CodeGenerator::enter(const std::string theIdentifier) noexcept
{
  mySymbolTable.push_back(theIdentifier);
}

//**************************************************
// CodeGenerator::finish
//**************************************************
void CodeGenerator::finish()
{
  std::cout << "Call finish" << std::endl;
  generate("Halt");
}

//**************************************************
// CodeGenerator::generate
//**************************************************
void CodeGenerator::generate(const std::string &theInstruction) noexcept
{
  if (! myEWTracker.hasError())
  {
    std::ostringstream code;
    code << theInstruction << std::endl;
    printCode(code.str());
    myGeneratedCode += code.str();
  }
}

//**************************************************
// CodeGenerator::generate
//**************************************************
void CodeGenerator::generate(const std::string &theInstruction,
                             const std::string &theFirst,
                             const std::string &theSecond) noexcept
{
  if (! myEWTracker.hasError())
  {
    std::ostringstream code;
    code << theInstruction << " " << theFirst << ", " << theSecond
         << std::endl;
    printCode(code.str());
    myGeneratedCode += code.str();
  }
}

//**************************************************
// CodeGenerator::generate
//**************************************************
void CodeGenerator::generate(const std::string &theInstruction,
                             const std::string &theFirst,
                             const std::string &theSecond,
                             const std::string &theThird) noexcept
{
  if (! myEWTracker.hasError())
  {
    std::ostringstream code;
    code << theInstruction << " " << theFirst << ", " << theSecond << ", "
         << theThird << std::endl;
    printCode(code.str());
    myGeneratedCode += code.str();
  }
}

//**************************************************
// CodeGenerator::generateInfix
//**************************************************
ExpressionRecord CodeGenerator::generateInfix(
  const ExpressionRecord &theLeftOperand,
  const OperatorRecord &theOperator,
  const ExpressionRecord &theRightOperand) noexcept
{
  std::cout << "Call generateInfix" << std::endl;

  std::string tempName(getTemp());
  generate(theOperator.getInstruction(),
           theLeftOperand.getValue(), theRightOperand.getValue(), tempName);
  return ExpressionRecord(ExpressionRecord::Type::Temporary, tempName);
}

//**************************************************
// CodeGenerator::getTemp
//**************************************************
std::string CodeGenerator::getTemp() noexcept
{
  std::cout << "Call getTemp" << std::endl;

  ++myMaxTemp;
  std::string tempVariable{"Temp&"};
  tempVariable += std::to_string(myMaxTemp);
  checkId(tempVariable);
  return tempVariable;
}

//**************************************************
// CodeGenerator::lookUp
//**************************************************
bool CodeGenerator::lookUp(const std::string &theIdentifier) const noexcept
{
  auto location =
    std::find(mySymbolTable.begin(), mySymbolTable.end(), theIdentifier);
  bool found = (location != mySymbolTable.end());
  return found;
}

//**************************************************
// CodeGenerator::printCode
//**************************************************
void CodeGenerator::printCode(const std::string &theCode) const noexcept
{
  std::cout << std::setw(17) << "" << theCode;
}

//**************************************************
// CodeGenerator::processId
//**************************************************
ExpressionRecord CodeGenerator::processId(const std::string &theToken) noexcept
{
  std::cout << "Call processId" << std::endl;

  checkId(theToken);
  return ExpressionRecord(ExpressionRecord::Type::Id, theToken);
}

//**************************************************
// CodeGenerator::processLiteral
//**************************************************
ExpressionRecord CodeGenerator::processLiteral(const std::string &theLiteral)
  noexcept
{
  std::cout << "Call processLiteral" << std::endl;

  return ExpressionRecord(ExpressionRecord::Type::Literal, theLiteral);
}

//**************************************************
// CodeGenerator::processOperator
//**************************************************
OperatorRecord CodeGenerator::processOperator(const std::string &theOperator)
  noexcept
{
  std::cout << "Call processOperator" << std::endl;

  if ("+" == theOperator)
  {
    return OperatorRecord(Token::Type::PlusOp);
  }
  return OperatorRecord(Token::Type::MinusOp);
}

//**************************************************
// CodeGenerator::readId
//**************************************************
void CodeGenerator::readId(const ExpressionRecord &theIdentifier) noexcept
{
  std::cout << "Call readId" << std::endl;

  generate("Read", theIdentifier.getValue(), "Integer");
}

//**************************************************
// CodeGenerator::start
//**************************************************
void CodeGenerator::start() noexcept
{
  std::cout << "Call start" << std::endl;
  // myMaxTemp is initialized in the .h file
  // No symbol table maximum as a vector is used.
}

//**************************************************
// CodeGenerator::writeExpression
//**************************************************
void CodeGenerator::writeExpression(const ExpressionRecord &theExpression)
  noexcept
{
  std::cout << "Call writeExpression" << std::endl;

  generate("Write", theExpression.getValue(), "Integer");
}
