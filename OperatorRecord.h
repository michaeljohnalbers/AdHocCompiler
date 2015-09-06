#ifndef OPERATORRECORD_H
#define OPERATORRECORD_H

/**
 * @file OperatorRecord.h
 * @brief Class which hold operator data.
 *
 * @author Michael Albers
 */

#include "Token.h"

/**
 * This class tracks which operator is used.
 */
class OperatorRecord : public Token
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Default constructor.
   */
  OperatorRecord() = delete;

  /**
   * Copy constructor
   */
  OperatorRecord(const OperatorRecord &) = default;

  /**
   * Move constructor
   */
  OperatorRecord(OperatorRecord &&) = default;

  /**
   * Constructor
   *
   * @param theOperator
   *          token (must be either plus or minus)
   * @throws std::invalid_argument on invalid token type
   */
  OperatorRecord(Type theOperator);

  /**
   * Destructor
   */
  ~OperatorRecord() = default;

  /**
   * Copy assignment operator
   *
   * @param theRHS
   *          object to copy
   * @return this object
   */
  OperatorRecord& operator=(const OperatorRecord &theRHS) = default;

  /**
   * Move assignment operator
   *
   * @param theRHS
   *          object to move
   * @return this object
   */
  OperatorRecord& operator=(OperatorRecord &&theRHS) = default;

  /**
   * Returns the instruction for this operator.
   *
   * @return the instruction for this operator.
   */
  std::string getInstruction() const noexcept;

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  // ************************************************************
  // Private
  // ************************************************************
  private:
};

#endif

