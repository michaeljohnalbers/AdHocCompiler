#ifndef SCANNER_H
#define SCANNER_H

/**
 * @file Scanner.h
 * @brief Defines the class used to scan input for tokens
 *
 * @author Michael Albers
 */

#include <cstdint>
#include <fstream>
#include <map>
#include <string>

#include "Token.h"

/**
 * Scans an input file returning all of the tokens contained therein. Use
 * getNextToken to find the next token in the file. Use getCurrentToken*
 * to retrieve information about the last scanned token.
 */
class Scanner
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Default constructor.
   */
  Scanner() = delete;

  /**
   * Copy constructor
   */
  Scanner(const Scanner &) = default;

  /**
   * Move constructor
   */
  Scanner(Scanner &&) = default;

  /**
   * constructor
   *
   * @param theFile
   *          file to scan/tokenize
   * @throw std::runtime_error
              on error opening input file
   */
  Scanner(const std::string &theFile);

  /**
   * Destructor
   */
  ~Scanner() = default;

  /**
   * Returns the last scanned token.
   *
   * @return token from file
   */
  Token getCurrentToken() const;

  /**
   * Find and return the next token in the file. Updates current token/token
   * type.
   *
   * @return next token
   * @throw std::runtime_error
   *          on syntax error
   */
  Token getNextToken();

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  // ************************************************************
  // Private
  // ************************************************************
  private:

  /**
   * Adds the given character to the token buffer.
   *
   * @param theCharacter
   *          character to add
   */
  void addCharacterToBuffer(char theCharacter);

  /**
   * Reads and discards current character in file.
   */
  void advance();

  /**
   * Clears the token buffer.
   */
  void clearBuffer();

  /**
   * Returns the token type of an identifier.
   *
   * @param theTokenLiteral
   *          token literal from source
   * @param theTokenStartColumn
   *          starting column of the token
   * @return Id or specific reserved word token
   */
  Token idOrReserved(const std::string &theTokenLiteral,
                     uint32_t theTokenStartColumn);

  /**
   * Returns current character in the input file. Doesn't advance. Essentially
   * a 'peek' operation.
   *
   * @return current character
   */
  char inspect();

  /**
   * Sets line/column counters when a newline is encountered.
   */
  void newLine();

  /**
   * read next character in the file and advance
   *
   * @return next charcter
   */
  char read();

  /**
   * Throws an exception for a syntax error
   *
   * @param theError
   *          error description
   * @throws always throws a std::runtime_error
   */
  void syntaxError(const std::string &theError);

  using ReservedWordMap = std::map<std::string, Token::Type>;

  /** Reserved words */
  static ReservedWordMap ourReservedWords;

  /** Current column */
  uint32_t myColumn = 0;

  /** Last scanned token. */
  Token myCurrentToken;

  /** Token literal being assembled */
  std::string myBuffer;

  /** Input file name */
  std::string myFile;

  /** Input file */
  std::ifstream myInputStream;

  /** Current line of input file */
  uint32_t myLine = 1;
};

#endif
