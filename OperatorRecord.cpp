/**
 * @file OperatorRecord.cpp
 * @brief Implementation of OperatorRecord class
 *
 * @author Michael Albers
 */

#include <stdexcept>

#include "OperatorRecord.h"

//**************************************************
// OperatorRecord::OperatorRecord
//**************************************************
OperatorRecord::OperatorRecord(Type theOperator) :
  Token(theOperator, 0, 0)
{
  if (theOperator != Type::PlusOp && theOperator != Type::MinusOp)
  {
    throw std::invalid_argument(
      "Illegal token type provided to OperatorRecord, " + getTokenString() +
      ", must be either PlusOp or MinusOp.");
  }
}

//**************************************************
// OperatorRecord::getInstruction
//**************************************************
std::string OperatorRecord::getInstruction() const noexcept
{
  if (Type::PlusOp == getToken())
  {
    return "ADD";
  }
  return "SUB";
}
