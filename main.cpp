#include <iostream>
#include <bitset>
#include <string>
#include <cmath>
#include <set>

std::string valuefy (std::string, std::string);


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
  return factorial(x) / ( factorial(x / 2) * factorial (x - x / 2) );
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
        minterms_string_set.insert( stringify( TABLE_PRIM[i][j], DIFFERENCE_PRIM[i][j], rows - 1 ) );

  for(int i = 0; i < rows - 1; i++)
    for(int j = 0; j < COUNT_SEC[i]; j++)
      MARKER[i * cols + j] = 0;
  
  for(int i = 0; i < rows; i++)   //Initialize marker array to 0
  {
    for(int j = 0; j < COUNT_SEC[i]; j++)
    {
      std::cout << TABLE_SEC[i][j]<< "  ";
    }
    std::cout << std::endl;
  }

  for(int i = 0; i < rows; i++)   //Initialize marker array to 0
  {
    for(int j = 0; j < COUNT_SEC[i]; j++)
    {
      std::cout << DIFFERENCE_SEC[i][j]<< "  ";
    }
    std::cout << std::endl;
  }

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
  std::bitset<16> A(lhs);
  std::bitset<16> B(rhs);

  std::cout <<A <<" " <<B <<std::endl;

  for(int i = size - 1 ; i >= 0; i--)
    if( A[i] == 0 && B[i] == 1)
      x.append(1, '-');
    else if (A[i] == 1)
      x.append(1, '1');
    else if (A[i] == 0)
      x.append(1, '0');

  std::cout <<"x = " << x << std::endl;
  return x;
}

std::string valuefy (std::string source, std::string out)
{
  bool flag = true;
  std::string temp = "";
  for(int i = 0; i < source.length(); i++)
    if (source[i] == '-')
    {
      flag = false;
      break;
    }

  if (flag)
  {
    int x = stoi(source,nullptr, 2);
    out.append(std::to_string(x));
    out.append(1, ',');
    return out;
  }

  for(int i = 0; i < source.length(); i++)
  {
    if (source[i] == '-')
    {
      std::string temp1 = temp;
      temp.append(1, '0');
        for(int j = i + 1; j < source.length(); j++)
          temp.append(1, source[j]);
      out = (valuefy(temp, out));
      temp1.append(1, '1');
      for(int j = i + 1; j < source.length(); j++)
          temp1.append(1, source[j]);
      out = (valuefy(temp1, out));
      return out;
    }
    temp.append(1, source[i]);
  }
}


int main()
{
  int** TABLE_1;
  int** TABLE_2;
  short* COUNT_TABLE_1;
  short* COUNT_TABLE_2;
  int** DIFFERENCE_BITS_1;
  int** DIFFERENCE_BITS_2;
  int* MINTERMS_ARRAY;
  bool* MARKER;
  int rows;
  int cols;
  int input_minterm;    //should be combined to single input var;
  int input_dont_care;  //should be combined to single input var;
  bool done = false;
  bool alternate = false;
  size_t popcount_val;
  short num_of_vars;
  short num_of_minterms;
  short num_of_dont_cares;
  std::set<std::string> minterms_string_set;

  std::cout << "Quine McKluskey\n";

  std::cout << "Enter number of variables\n> ";
  while (std::cin >> num_of_vars && (num_of_vars < 1 || num_of_vars > 16))
    std::cout << "Number of vars must be within 1 to 16 (inclusive), try again\n> ";


  //Initialize rows, cols and arrays
  rows = num_of_vars + 1;
  cols = (num_of_vars > 1 )? maxcombinations(num_of_vars) : 1;
  TABLE_1 = new int* [rows];
  TABLE_2 = new int* [rows];
  COUNT_TABLE_1 = new short [rows];
  COUNT_TABLE_2 = new short [rows];
  DIFFERENCE_BITS_1 = new int* [rows];
  DIFFERENCE_BITS_2 = new int* [rows];
  MARKER = new bool [rows * cols];

  for(int i = 0; i < rows; ++i)
  {
    TABLE_1[i] = new int [cols];
    TABLE_2[i] = new int [cols];
    DIFFERENCE_BITS_1[i] = new int [cols];
    DIFFERENCE_BITS_2[i] = new int [cols];
  }

  clear_count_table(COUNT_TABLE_1, rows);
  clear_count_table(COUNT_TABLE_2, rows);


  std::cout << "How many Minterms?\n> ";
  while (std::cin >> num_of_minterms && (num_of_minterms <= 0 || num_of_minterms > pow(2, num_of_vars)) )
    std::cout << "Number of Minterms must be bigger than 0, try again\n> ";

  MINTERMS_ARRAY = new int [num_of_minterms];

  std::cout <<"Please enter you minterms (in decimal)\n> ";

  for(int i = 0; i < num_of_minterms; i++)    //Assumes no duplicates are inserted
  {
    while (std::cin >> input_minterm && (input_minterm < 0 || input_minterm > pow(2, num_of_vars) ))
      std::cout << "Wrong input, number must be between 0 and " << pow(2, num_of_vars) <<", try again\n> ";

    popcount_val = popcount(input_minterm);
    TABLE_1[popcount_val][COUNT_TABLE_1[popcount_val]] = input_minterm;
    ++COUNT_TABLE_1[popcount_val];
    MINTERMS_ARRAY[i] = input_minterm;
  }

  std::cout << "How many Don't Cares?\n> ";
  while (std::cin >> num_of_dont_cares && (num_of_dont_cares < 0 || num_of_dont_cares > pow(2, num_of_vars)) )
    std::cout << "Number of Don't Cares must be bigger than -1, try again\n> ";

  if (num_of_dont_cares != 0)
  {
    std::cout <<"Please enter your Don't Cares (in decimal)\n> ";
    for(int i = 0; i < num_of_dont_cares; i++)    //Assumes no duplicates are inserted
      {
        while (std::cin >> input_dont_care && (input_dont_care < 0 || input_dont_care > pow(2, num_of_vars) ))
          std::cout << "Wrong input, number must be between 0 and " << pow(2, num_of_vars) <<", try again\n> ";

        popcount_val = popcount(input_dont_care);
        TABLE_1[popcount_val][COUNT_TABLE_1[popcount_val]] = input_dont_care;
        ++COUNT_TABLE_1[popcount_val];
      }
  }

  //Initialize marker array to 0
  for(int i = 0; i < rows; i++)
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
  {
    std::string output ="";
    std::cout <<valuefy(prime, output) << std::endl;
  }
for(int i = 0; i < rows; ++i)
{
    delete [] TABLE_1[i];
    delete [] TABLE_2[i];
    delete [] DIFFERENCE_BITS_1[i];
    delete [] DIFFERENCE_BITS_2[i];
}

delete [] TABLE_1;
delete [] TABLE_2;
delete [] DIFFERENCE_BITS_1;
delete [] DIFFERENCE_BITS_2;
delete [] MINTERMS_ARRAY;
delete [] MARKER;
delete [] COUNT_TABLE_1;
delete [] COUNT_TABLE_2;
}