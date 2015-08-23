/**
 * @file main.cpp
 * @brief Entry point of program
 *
 * @author Michael Albers
 */

#include <iostream>

#include "Scanner.h"

int main(int argc, char **argv)
{
  try
  {
    if (argc == 1)
    {
      throw std::runtime_error("No input file provided.");
    }

    Scanner scanner(argv[1]);
    while (true)
    {
      Token nextToken = scanner.getNextToken();
      std::cout << "Token: " << nextToken << std::endl;
      if (nextToken.getToken() == Token::Type::EofSym)
      {
        break;
      }
    }
  }
  catch (const std::exception &exception)
  {
    std::cerr << argv[0] << ": error: " << exception.what() << std::endl;
    return 1;
  }

  return 0;
}
