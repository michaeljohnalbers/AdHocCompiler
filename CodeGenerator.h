#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

/**
 * @file CodeGenerator.h
 * @brief Defines the class used to generate code from the parse.
 *
 * @author Michael Albers
 */

#include <cstdint>
#include <string>
#include <vector>

class ErrorWarningTracker;
class ExpressionRecord;
class OperatorRecord;

/**
 * 
 */
class CodeGenerator
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Default constructor
   */
  CodeGenerator() = delete;

  /**
   * Copy constructor
   */
  CodeGenerator(const CodeGenerator &) = delete;

  /**
   * Move constructor
   */
  CodeGenerator(const CodeGenerator &&) = delete;

  /**
   * Constructor.
   *
   * @param theEWTracker
   *          error/warning tracker
   */
  CodeGenerator(ErrorWarningTracker &theEWTracker);

  /**
   * Destructor
   */
  ~CodeGenerator();

  /**
   * Copy assignment operator
   */
  CodeGenerator& operator=(const CodeGenerator &) = delete;

  /**
   * Move assignment operator
   */
  CodeGenerator& operator=(const CodeGenerator &&) = delete;

  /**
   * Generator code for a variable assignment.
   *
   * @param theSource
   *          source expression
   * @param theDestination
   *          variable being assigned
   */
  void assign(const ExpressionRecord &theSource,
              const ExpressionRecord &theDestination) noexcept;

  /**
   * If the given identifier is not in the symbol table, it
   * is added and code is generated to allocate space for it.
   *
   * @param theIdentifier
   *          identifer to chec on
   */
  void checkId(const std::string &theIdentifier) noexcept;

  /**
   * Generates code to terminate a program
   */
  void finish();

  /**
   * Generates code for an infix calculation (i.e., A + B)
   *
   * @param theLeftOperand
   *          operand on left side of operator
   * @param theOperator
   *          operator
   * @param theRightOperand
   *          operand on right side of operator
   * @return identifier holding infix calculation result
   */
  ExpressionRecord generateInfix(const ExpressionRecord &theLeftOperand,
                                 const OperatorRecord &theOperator,
                                 const ExpressionRecord &theRightOperand)
    noexcept;

  /**
   * Returns the name of a new temporary variable and generates
   * code to allocate space for the temporary.
   *
   * @return name of temporary variable
   */
  std::string getTemp() noexcept;

  /**
   * Declare Id, enter it into the semantic table, and return a corresponding
   * semantic record
   *
   * @param theToken
   *          token from source
   * @return record for token
   */
  ExpressionRecord processId(const std::string &theToken) noexcept;

  /**
   * Returns a semantic record for the given literal
   *
   * @param theLiteral
   *          literal from source
   * @return record for literal
   */
  ExpressionRecord processLiteral(const std::string &theLiteral) noexcept;

  /**
   * Returns an operator record for the given operator.
   *
   * @param theOperator
   *          operator from source
   * @return record for operator
   */
  OperatorRecord processOperator(const std::string &theOperator) noexcept;

  /**
   * Generate code to read a value into the given identifier
   *
   * @param theIdentifier
   *          identifier to read in to
   */
  void readId(const ExpressionRecord &theIdentifier) noexcept;

  /**
   * Initializes symbol table.
   */
  void start() noexcept;

  /**
   * Generate code to write the value of an expression
   *
   * @param theExpression
   *          expression to write
   */
  void writeExpression(const ExpressionRecord &theExpression) noexcept;

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  /**
   * Unconditionally adds the identifier to the symbol table.
   *
   * @param theIdentifier
   *          identifier to add
   */
  void enter(const std::string theIdentifier) noexcept;

  /**
   * Writes a no-operand instruction.
   *
   * @param theInstruction
   *          instruction
   */
  void generate(const std::string &theInstruction) noexcept;

  /**
   * Writes a 2 operand instruction.
   *
   * @param theInstruction
   *          instruction
   * @param theFirst
   *          first argument to instruction
   * @param theSecond
   *          second argument to instruction
   */
  void generate(const std::string &theInstruction,
                const std::string &theFirst,
                const std::string &theSecond) noexcept;
  /**
   * Writes a 3 operand instruction.
   *
   * @param theInstruction
   *          instruction
   * @param theFirst
   *          first argument to instruction
   * @param theSecond
   *          second argument to instruction
   * @param theThird
   *          third argument to instruction
   */
  void generate(const std::string &theInstruction,
                const std::string &theFirst,
                const std::string &theSecond,
                const std::string &theThird) noexcept;

  /**
   * Checks if the given identifier is in the symbol table.
   * The search is Case sensitive.
   *
   * @param theIdentifier
   *          identifier to look for
   * @return true if the identifier is in the symbol table,
   *         otherwise false
   */
  bool lookUp(const std::string &theIdentifier) const noexcept;

  /**
   * Prints a line of code to stdout.
   *
   * @param theCode
   *          line of code
   */
  void printCode(const std::string &theCode) const noexcept;

  // ************************************************************
  // Private
  // ************************************************************
  private:

  /** All code generated. */
  std::string myGeneratedCode;

  /** Temporary variable id */
  uint32_t myMaxTemp = 0;

  /** Known symbols */
  std::vector<std::string> mySymbolTable;

  /** Tracker of errors and warnings during scanning and parsing. */
  ErrorWarningTracker &myEWTracker;

};

#endif
