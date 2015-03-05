#include <iostream>
#include <bitset>
#include <string>
#include <cmath>
#include <set>

size_t popcount(int);
int factorial(int);
int maxcombinations(int);
bool is_power_of_two(int);
bool work_on_table(int* [], int* [], short[], short [], int* [], int* [], bool [], int, int, std::set<std::string>&);
void clear_count_table(short [], int);
std::string stringify(int, int, int);

inline int factorial(int x)
{
  return (x == 1 ? x : x * factorial( x - 1 ));
}
inline int maxcombinations(int x)
{
  return factorial(x) / (factorial( x / 2) * factorial (x - x / 2));
}
inline size_t popcount(int x)
{
  std::bitset<16> A(x);
  return A.count();
}

inline bool is_power_of_two(int x)
{
  return (x < 0)? 0 : ( (x & (x - 1)) == 0 );    //return true for n = 0, function should never receive 0
}
bool work_on_table(int* TABLE_PRIM[],int* TABLE_SEC [], short COUNT_PRIM [], short COUNT_SEC [],
                   int* DIFFERENCE_PRIM[], int* DIFFERENCE_SEC[], bool MARKER[], int rows, int cols,
                   std::set<std::string>& minterms_string_set)
{
  bool done = true;
  for(int i = 0; i < rows - 1; i++)
    for(int j = 0; j < COUNT_PRIM[i]; j++)
      for(int z = 0; z < COUNT_PRIM[i+1]; z++)
      if ( is_power_of_two( TABLE_PRIM[i+1][z] - TABLE_PRIM[i][j]) && DIFFERENCE_PRIM[i][j] == DIFFERENCE_PRIM[i+1][z])
      {
        done = false;
        MARKER[i*cols + j] = 1;
        MARKER[(i+1)*cols + z] = 1;
        DIFFERENCE_SEC[i][COUNT_SEC[i]] = DIFFERENCE_PRIM[i][j] - TABLE_PRIM[i][j] + TABLE_PRIM[i+1][z];
        TABLE_SEC[i][COUNT_SEC[i]] = TABLE_PRIM[i][j] & TABLE_PRIM[i+1][z]; //might be wrong
        ++COUNT_SEC[i];
      }
  //for(int i = 0; i < rows; i++)
  //{
  //  for(int j = 0; j < COUNT_PRIM[i]; j++)
  //  {
  //    std::cout << MARKER[(i*cols) + j]<< "  ";
  //  }
  //  std::cout << std::endl;
  //}
  for(int i = 0; i < rows - 1; i++)
    for(int j = 0; j < COUNT_PRIM[i]; j++)
      if (MARKER[i * cols + j] == 0)
        minterms_string_set.insert(stringify( TABLE_PRIM[i][j], DIFFERENCE_PRIM[i][j], rows - 1 ));

  for(int i = 0; i < rows - 1; i++)
    for(int j = 0; j < COUNT_SEC[i]; j++)
      MARKER[i * cols + j] = 0;

  //for(int i = 0; i < rows; i++)   //Initialize marker array to 0
  //{
  //  for(int j = 0; j < COUNT_SEC[i]; j++)
  //  {
  //    std::cout << TABLE_SEC[(i*cols) + j]<< "  ";
  //  }
  //  std::cout << std::endl;
  //}

  //for(int i = 0; i < rows; i++)   //Initialize marker array to 0
  //{
  //  for(int j = 0; j < COUNT_SEC[i]; j++)
  //  {
  //    std::cout << DIFFERENCE_SEC[(i*cols) + j]<< "  ";
  //  }
  //  std::cout << std::endl;
  //}
  return done;

}
void clear_count_table(short COUNT_TABLE[], int rows)
{
  for (int i = 0; i < rows; i++)
    COUNT_TABLE[i] = 0;
}

std::string stringify(int lhs, int rhs, int size)
{
  std::string x = "";
  std::bitset<4> A(lhs);
  std::bitset<4> B(rhs);
  //std::cout <<A <<" " <<B <<std::endl;
  for(int i = size - 1 ; i >= 0; i--)
    if( A[i] == 0 && B[i] == 1)
      x.append(1, '-');
    else if (A[i] == 1)
      x.append(1, '1');
    else if (A[i] == 0)
      x.append(1, '0');
  //std::cout <<"x = " << x << std::endl;
  return x;
}


int main()
{
  int** TABLE_1;
  int** TABLE_2;
  short* COUNT_TABLE_1;
  short* COUNT_TABLE_2;
  int** DIFFERENCE_BITS_1;
  int** DIFFERENCE_BITS_2;
  bool* MARKER;
  int rows;
  int cols;
  int input_minterm;
  bool done = false;
  bool alternate = false;
  size_t popcount_val;
  short num_of_vars;
  short num_of_minterms;
  std::set<std::string> minterms_string_set;

  std::cout << "Quine McKluskey\n";

  std::cout << "Enter number of variables\n> ";
  while (std::cin >> num_of_vars && (num_of_vars < 1 || num_of_vars > 16))
    std::cout << "Number of vars must be within 1 to 16 (inclusive), try again\n> ";

  std::cout << "How many Mintemrs?\n> ";
  while (std::cin >> num_of_minterms && (num_of_minterms <= 0 || num_of_minterms > pow(2, num_of_vars)) )
    std::cout << "Number of Minterms must be bigger than 0, try again\n> ";

  std::cout <<"Please enter you minterms (in decimal)\n> ";

  rows = num_of_vars + 1;
  cols = (num_of_vars > 1 )? maxcombinations(num_of_vars) : 1;


  TABLE_1 = new int* [rows];
  TABLE_2 = new int* [rows];
  COUNT_TABLE_1 = new short [rows];
  COUNT_TABLE_2 = new short [rows];
  DIFFERENCE_BITS_1 = new int* [rows];
  DIFFERENCE_BITS_2 = new int* [rows];

  for(int i = 0; i < rows; ++i)
  {
    TABLE_1[i] = new int [cols];
    TABLE_2[i] = new int [cols];
    DIFFERENCE_BITS_1[i] = new int [cols];
    DIFFERENCE_BITS_2[i] = new int [cols];
  }

  MARKER = new bool [rows * cols];

  clear_count_table(COUNT_TABLE_1, rows);
  clear_count_table(COUNT_TABLE_2, rows);

  for(int i = 0; i < num_of_minterms; i++)    //Assumes no duplicates are inserted
  {
    while (std::cin >> input_minterm && (input_minterm < 0 || input_minterm > pow(2, num_of_vars) ))
      std::cout << "Wrong input, number must be between 0 and " << pow(2, num_of_vars) <<", try again\n> ";

    popcount_val = popcount(input_minterm);
    TABLE_1[popcount_val][COUNT_TABLE_1[popcount_val]] = input_minterm;
    //std::cout << TABLE_1[popcount_val * cols + COUNT_TABLE_1[popcount_val]]<< "   ";
    ++COUNT_TABLE_1[popcount_val];
  }

  for(int i = 0; i < rows; i++)   //Initialize marker array to 0
    for(int j = 0; j < COUNT_TABLE_1[i]; j++)
    {
      DIFFERENCE_BITS_1[i][j] = 0;
      DIFFERENCE_BITS_2[i][j] = 0;
      MARKER[i * cols + j] = 0;
    }

  //for(int i = 0; i < rows; i++)   //Initialize marker array to 0
  //{
  //  for(int j = 0; j < COUNT_TABLE_1[i]; j++)
  //  {
  //    std::cout << TABLE_1[(i*cols) + j]<< "  ";
  //  }
  //  std::cout << std::endl;
  //}

  while (!done)
  {
    if(!alternate)
    {
      clear_count_table(COUNT_TABLE_2, rows);
      done = work_on_table(TABLE_1, TABLE_2, COUNT_TABLE_1, COUNT_TABLE_2, DIFFERENCE_BITS_1, DIFFERENCE_BITS_2, MARKER, rows, cols, minterms_string_set);
    }
    else
    {
      clear_count_table(COUNT_TABLE_1, rows);
      done = work_on_table(TABLE_2, TABLE_1, COUNT_TABLE_2, COUNT_TABLE_1, DIFFERENCE_BITS_2,DIFFERENCE_BITS_1, MARKER, rows, cols, minterms_string_set);
    }
    alternate = !alternate;
  }

  for( std::string prime : minterms_string_set)
    std::cout <<prime <<std::endl;

for(int i = 0; i < rows; ++i)
{
    delete [] TABLE_1[i];
    delete [] TABLE_2[i];
    delete [] DIFFERENCE_BITS_1;
    delete [] DIFFERENCE_BITS_2;
}

delete [] TABLE_1;
delete [] TABLE_2;
delete [] DIFFERENCE_BITS_1;
delete [] DIFFERENCE_BITS_1;
delete [] MARKER;
delete [] COUNT_TABLE_1;
delete [] COUNT_TABLE_2;
}