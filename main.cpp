#include <iostream>
#include <bitset>
#include <string>

size_t popcount(int);


size_t popcount(int x)
{
  std::bitset<16> A(x);
  return A.count();
}

int main()
{
  int* TABLE_1;
  int* COUNT_TABLE;
  int* MARKER;
  short num_of_vars;
  short num_of_minterms;

  std::string minterms_string;
  std::cout << "Quine McKluskey\n";

  std::cout << "Enter number of variables\n> ";
  while (std::cin >> num_of_vars && (num_of_vars < 1 || num_of_vars > 16))
    std::cout << "Number of vars must be within 1 to 16 (inclusive), try again\n> ";

  std::cout << "How many Minters?\n> ";
  while (std::cin >> num_of_minterms && num_of_minterms <= 0)
    std::cout << "Number of Minterms must be bigger than 0, try again\n> ";

  TABLE_1 = new int [(num_of_vars) * (num_of_vars * (num_of_vars - 1))];    //change this
  COUNT_TABLE = new int [num_of_vars];
  MARKER;
  for(int i = 0; i < num_of_minterms; i++)
  {

delete [] TABLE_1;
delete [] COUNT_TABLE;
}