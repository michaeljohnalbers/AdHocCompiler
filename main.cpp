/**
 * @file main.cpp
 * @brief Entry point of program
 *
 * @author Michael Albers
 */

#include <iostream>

#include "ErrorWarningTracker.h"
#include "Scanner.h"
#include "Parser.h"

int main(int argc, char **argv)
{
  try
  {
    if (argc == 1)
    {
      throw std::runtime_error("No input file provided.");
    }

    std::string file(argv[1]);

    ErrorWarningTracker ewTracker(file);

    Scanner scanner(file);
    Parser parser(scanner, ewTracker);
    parser.parse();
  }
  catch (const std::exception &exception)
  {
    std::cerr << argv[0] << ": error: " << exception.what() << std::endl;
    return 1;
  }

  return 0;
}
