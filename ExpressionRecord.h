#ifndef EXPRESSIONRECORD_H
#define EXPRESSIONRECORD_H

/**
 * @file ExpressionRecord.h
 * @brief Class which hold expression information
 *
 * @author Michael Albers
 */

#include <string>

/**
 * This class holds expression data.
 */
class ExpressionRecord
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  enum class Type
  {
    Id,
    Literal,
    Temporary
  };

  /**
   * Default constructor.
   */
  ExpressionRecord();

  /**
   * Copy constructor
   */
  ExpressionRecord(const ExpressionRecord &) = default;

  /**
   * Move constructor
   */
  ExpressionRecord(ExpressionRecord &&) = default;

  /**
   * Constructor
   *
   * @param theType
   *          type of expression
   * @param theValue
   *          expression value
   */
  ExpressionRecord(Type theType, const std::string &theValue);

  /**
   * Destructor
   */
  ~ExpressionRecord() = default;

  /**
   * Copy assignment expression
   *
   * @param theRHS
   *          object to copy
   * @return this object
   */
  ExpressionRecord& operator=(const ExpressionRecord &theRHS) = default;

  /**
   * Move assignment expression
   *
   * @param theRHS
   *          object to move
   * @return this object
   */
  ExpressionRecord& operator=(ExpressionRecord &&theRHS) = default;

  /**
   * Returns the expression type.
   *
   * @return expression type
   */
  Type getType() const noexcept;

  /**
   * Returns expression value.
   *
   * @return expression value.
   */
  std::string getValue() const noexcept;

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  // ************************************************************
  // Private
  // ************************************************************
  private:

  /** Expression type. */
  Type myType;

  /** Value of expression (identifier, value, etc.) */
  std::string myValue;
};

#endif

