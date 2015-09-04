#ifndef PARSER_H
#define PARSER_H

/**
 * @file Parser.h
 * @brief Defines the class used to parse tokens according to the Micro grammar
 *
 * @author Michael Albers
 */

#include <cstdint>
#include <stack>

#include "ASTNode.h"
#include "CodeGenerator.h"
#include "Scanner.h"

class ErrorWarningTracker;
class ExpressionRecord;
class OperatorRecord;

/**
 * Implements a recursive descent LL(1) parser.
 */
class Parser
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Default constructor.
   */
  Parser() = delete;

  /**
   * Copy constructor
   */
  Parser(const Parser &) = delete;

  /**
   * Move constructor
   */
  Parser(Parser &&) = delete;

  /**
   * Constructor
   *
   * @param theScanner
   *          scanner object
   * @param theGenerator
   *          code generator object
   * @param theEWTracker
   *          object to track and report errors and warnings
   */
  Parser(Scanner &theScanner,
         CodeGenerator &theGenerator,
         ErrorWarningTracker &theEWTracker);

  /**
   * Destructor
   */
  ~Parser() = default;

  /**
   * Copy assignment operator
   */
  Parser& operator=(const Parser &) = delete;

  /**
   * Move assignment operator
   */
  Parser& operator=(Parser &&) = delete;

  /**
   * Parses all the tokens from the provided scanner
   */
  void parse();
  
  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  // ************************************************************
  // Private
  // ************************************************************
  private:

  /*
   * Production functions
   */
  void addOp(OperatorRecord &theOperator);
  void expression(ExpressionRecord &theExpression);
  void exprList();
  void ident(ExpressionRecord &theIdentifier);
  void idList();
  void primary(ExpressionRecord &theExpression);
  void program();
  void statement();
  void statementList();
  void systemGoal();

  /*
   * Other functions
   */

  /**
   * Extracts the next token and matches it against the provided token. Syntax
   * error is reported if there is no match.
   *
   * @param theToken
   *          token to match against
   */
  void match(const Token::Type &theToken) noexcept;

  /**
   * Prints function being called and code remaining.
   *
   * @param theFunction
   *          function name
   */
  void printFunction(const std::string &theFunction);

  /**
   * Prints the current production number and the current set of terminals and
   * non-terminals corresponding to the parse thus far.
   */
  void printParse(uint32_t theProduction, const char *theProductionName = "")
    const noexcept;

  /** Root node of abstract syntax tree. */
  ASTNode myASTRoot;

  /** Code generator */
  CodeGenerator &myGenerator;

  /**
   * Tracks parent nodes, top of stack is the parent for the current
   * production. Each production function is responsible for calling 'pop'
   * before returning.
   */
  std::stack<ASTNode*> myParentNode;

  /** Object to track and report errors and warnings */
  ErrorWarningTracker &myEWTracker;

  /** Token scanner */
  Scanner &myScanner;
};

#endif
