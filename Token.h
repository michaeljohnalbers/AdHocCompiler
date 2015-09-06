#ifndef TOKEN_H
#define TOKEN_H

/**
 * @file Token.h
 * @brief Class which defines a token.
 *
 * @author Michael Albers
 */

#include <cstdint>
#include <map>
#include <ostream>
#include <string>

/**
 * This class defines all known token types as well as representing a single
 * token scanned from source code.
 */
class Token
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /** All tokens */
  enum class Type
  {
    BeginSym,
    EndSym,
    ReadSym,
    WriteSym,
    Id,
    IntLiteral,
    LParen,
    RParen,
    SemiColon,
    Comma,
    AssignOp,
    PlusOp,
    MinusOp,
    EqualOp,
    ExponentOp,
    EofSym,
  };

  /**
   * Default constructor.
   */
  Token() = default;

  /**
   * Copy constructor
   */
  Token(const Token &) = default;

  /**
   * Move constructor
   */
  Token(Token &&) = default;

  /**
   * Constructor (EofSym has no literal)
   *
   * @param theToken
   *          token
   * @param theLine
   *          line on which the token appears
   * @param theColumn
   *          column on which the token starts
   */
  Token(Type theToken,
        uint32_t theLine,
        uint32_t theColumn);

  /**
   * Constructor
   *
   * @param theToken
   *          token
   * @param theTokenLiteral
   *          literal token string from source
   * @param theLine
   *          line on which the token appears
   * @param theColumn
   *          column on which the token starts
   */
  Token(Type theToken,
        const std::string &theTokenLiteral,
        uint32_t theLine,
        uint32_t theColumn);

  /**
   * Destructor
   */
  virtual ~Token() = default;

  /**
   * Copy assignment operator
   *
   * @param theRHS
   *          object to copy
   * @return this object
   */
  Token& operator=(const Token &theRHS) = default;

  /**
   * Move assignment operator
   *
   * @param theRHS
   *          object to move
   * @return this object
   */
  Token& operator=(Token &&theRHS) = default;

  /**
   * Less than operator. There is no natural ordering to tokens, this is just
   * provided so this class can be used in ordered STL containers.
   *
   * @param theRHS
   *          right hand side of <
   * @return true if less than, false otherwise
   */
  bool operator<(const Token &theRHS) const noexcept;

  /**
   * Equality operator.
   *
   * @param theRHS
   *          right hand side of ==
   * @return true if equal, false otherwise
   */
  bool operator==(const Token &theRHS) const noexcept;

  /**
   * Insertion operator
   *
   * @param theOS
   *          stream in which to insert data
   * @param theToken
   *          token data
   * @return modified stream
   */
  friend std::ostream& operator<<(std::ostream &theOS,
                                  const Token &theToken);

  /**
   * Returns the column on which the token starts.
   *
   * @return the column on which the token starts.
   */
  uint32_t getColumn() const noexcept;

  /**
   * Returns the line on which the token appears.
   *
   * @return the line on which the token appears.
   */
  uint32_t getLine() const noexcept;

  /**
   * Returns the token string
   *
   * @return token string
   */
  std::string getLiteral() const noexcept;

  /**
   * Returns the token.
   *
   * @return token
   */
  Type getToken() const noexcept;

  /**
   * Returns a string representation of the token (i.e., "EofSym" for the
   * EofSym token).
   *
   * @return string representation of the token
   */
  std::string getTokenString() const noexcept;

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  // ************************************************************
  // Private
  // ************************************************************
  private:

  /** Column (character in line) on which the token starts. */
  uint32_t myColumn = 0;

  /** Line on which the token appears. */
  uint32_t myLine = 0;

  /** Token type */
  Type myToken = Type::EofSym;

  /** Token literal from source. */
  std::string myTokenLiteral;

  using TokenMap = std::map<Type, std::string>;

  /** Token-to-string mapping*/
  static TokenMap ourTokenDescriptions;

};

#endif
