#ifndef ERRORWARNINGTRACKER_H
#define ERRORWARNINGTRACKER_H

/**
 * @file ErrorWarningTracker.h
 * @brief Defines the class used to print and track compile errors and warnings.
 *
 * @author Michael Albers
 */

#include <string>

class Token;

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
  bool getHasError() const noexcept;

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
