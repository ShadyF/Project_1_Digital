#include <iostream>
#include <string>
int main()
{
  int num_of_vars;
  std::string minterms_string;
  std::cout << "Quine McKluskey\n";
  std::cout << "Enter number of variables\n> ";

  while (std::cin >> num_of_vars && (num_of_vars < 1 || num_of_vars > 16))
    std::cout << "Number of vars must be within 1 to 16 (inclusive), try again\n> ";

  std::cout << "Enter all your Minterms (seperated by spaces)\n> ";
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::getline(std::cin, minterms_string);
  std::cout << minterms_string;
}