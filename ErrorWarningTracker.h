#ifndef ERRORWARNINGTRACKER_H
#define ERRORWARNINGTRACKER_H

/**
 * @file ErrorWarningTracker.h
 * @brief Defines the class used to print and track compile errors and warnings.
 *
 * @author Michael Albers
 */

#include <string>

#include "Token.h"

/**
 * This class handles compiler errors and warnings. It should be used to report
 * all errors and warnings encountered during compilation.
 */
class ErrorWarningTracker
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Default constructor.
   */
  ErrorWarningTracker() = delete;

  /**
   * Copy constructor
   */
  ErrorWarningTracker(const ErrorWarningTracker &) = default;

  /**
   * Move constructor
   */
  ErrorWarningTracker(ErrorWarningTracker &&) = default;

  /**
   * Constructor.
   */
  ErrorWarningTracker(const std::string &theFile);

  /**
   * Destructor
   */
  ~ErrorWarningTracker() = default;

  /**
   * Copy assignment operator
   */
  ErrorWarningTracker& operator=(const ErrorWarningTracker &) = default;

  /**
   * Move assignment operator
   */
  ErrorWarningTracker& operator=(ErrorWarningTracker &&) = default;

  /**
   * Returns if the file has an error.
   *
   * @return true if an error was reported, false otherwise
   */
  bool hasError() const noexcept;

  /**
   * Reports a syntax error.
   *
   * @param theErrorToken
   *          token on which the error occurred
   * @param theError
   *          error to report
   */
  void reportError(Token &theErrorToken, const std::string &theError) noexcept;

  /**
   * Reports a syntax error that the expected token was not read and lists
   * the possible legal token(s). This function is really a convenience
   * wrapper around the other reportError.
   *
   * @param theErrorToken
   *          token on which the error occurred
   * @param theNumberExpected
   *          number of variable arguments
   * @param ...
   *          variable number of Token::Type values. These are the legal
   *          tokens which were expected.  Number given must match the value
   *          passed as theNumberExpected
   */
  void reportError(Token &theErrorToken,
                   uint32_t theNumberExpected,
                   ...);

  /**
   * Reports a warning.
   *
   * @param theWarning
   *          warngin to report
   */
  void reportWarning(const std::string &theWarning) noexcept;

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  // ************************************************************
  // Private
  // ************************************************************
  private:

  /** File being compiled. */
  const std::string myFile;

  /** Does the program have an error? */
  bool myHasError = false;
};

#endif
