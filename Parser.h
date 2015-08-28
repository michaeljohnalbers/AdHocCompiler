#ifndef PARSER_H
#define PARSER_H

/**
 * @file Parser.h
 * @brief Defines the class used to parse tokens according to the Micro grammar
 *
 * @author Michael Albers
 */

#include <cstdint>

#include "ASTNode.h"
#include "Scanner.h"

class ErrorWarningTracker;

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
   * @param theEWTracker
   *          object to track and report errors and warnings
   */
  Parser(Scanner &theScanner,
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
  void addOp();
  void expression();
  void exprList();
  void ident();
  void idList();
  void primary();
  void program();
  void statement();
  void statementList();
  void systemGoal();

  /*
   * Helper functions
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
   * Prints the current production number and the current set of terminals and
   * non-terminals corresponding to the parse thus far.
   */
  void printParse(uint32_t theProduction, const char *theProductionName="")
    const noexcept;

  ASTNode myASTRoot;

  ASTNode *myASTCurrent = &myASTRoot;

  /** Object to track and report errors and warnings */
  ErrorWarningTracker &myEWTracker;

  /** Token scanner */
  Scanner &myScanner;
};

#endif
