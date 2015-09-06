/**
 * @file ExpressionRecord.cpp
 * @brief Implementation of ExpressionRecord class
 *
 * @author Michael Albers
 */

#include "ExpressionRecord.h"

//**************************************************
// ExpressionRecord::ExpressionRecord
//**************************************************
ExpressionRecord::ExpressionRecord() :
  ExpressionRecord(Type::Id, "")
{
}

//**************************************************
// ExpressionRecord::ExpressionRecord
//**************************************************
ExpressionRecord::ExpressionRecord(Type theType, const std::string &theValue) :
  myType(theType),
  myValue(theValue)
{
}

//**************************************************
// ExpressionRecord::getType
//**************************************************
ExpressionRecord::Type ExpressionRecord::getType() const noexcept
{
  return myType;
}

//**************************************************
// ExpressionRecord::getValue
//**************************************************
std::string ExpressionRecord::getValue() const noexcept
{
  return myValue;
}
