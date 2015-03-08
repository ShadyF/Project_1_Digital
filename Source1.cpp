#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <iomanip>
using namespace std;

string S;
int** A;
vector <int> rV;
vector <int> cV;

int set_elements[] = {4,5,6,8,9,10,13};
string set_elementStrArr[] = {"0,4", "0,8", "8,9", "8,10", "9,13", "4,5,6,7", "5,7,13,15"};
int N = (int)sizeof(set_elementStrArr) / sizeof(string);


void createCoverageChart();
void drawCoverageChart();
void RemoveEssn();
void Extract();
bool removeCol();
bool removeDomCol();
bool removeRow();


int main()
{
	cout<<"This Program takes the prime minterms found from Table 1, and prints it in this table to find the Essential Prime minterms."<<endl;
	cout<<"Press any key to continue.";
	getch();
	cout<<endl<<endl;
	A = new int*[N];        //N*N? works only in this case
	for (int y = 0;y<N;y++)
		A[y] = new int[N];
	createCoverageChart();
	drawCoverageChart();
	cout<<endl<<endl;
	cout<<"This is the table of all X's printed with Minterms (rows) and Set-ON element (columns) before clearing."<<endl;
	cout<<"Press any key to start Erasing."<<endl;
	cout<<"Erase: Find any column with a single one X, erase the X's in its row and the columns of the X's."<<endl;
	getch();
	cout<<endl<<endl;
	RemoveEssn();
	drawCoverageChart();

  for (int y = 0;y<N;y++)
		delete [] A[y];
  delete [] A;

	return 0;
}

int setElementExists(int minTerm)		//This function checks if the element exists
{
	for (int i = 0; i<N;i++)        //N is size of set_elementStrArr not set_elements
		if (set_elements[i] == minTerm ) return i;

	return -1;
}

void createCoverageChart()				//Creating the chart with all minterms given
{
	//initialize n to the size of the array/string
	int n = (int)(sizeof(set_elementStrArr)/sizeof(string)), x = 0;
	char set_element;
	string set_elementStr;
	set_elementStr = set_elementStrArr[x];			//loop of all minterms divided in the array
  int index;
	while (x < (n-1)) {
		
		for(int i=0; i<N; i++)
		{
			for (int k = 0;k<set_elementStr.length(); k++) 
			{
				if (set_elementStr.at(k) == ',') continue;
				set_element = atoi(&set_elementStr.at(k));

				index = setElementExists(set_element) ;
				if (index != -1) {
					A[i][index] = 1;
				}
				else A[i][index] = 0;       //?????? index is -1 here which causes heap corruption
			}
			x = x + 1;
			if (x==n) break;    //why?
			set_elementStr = set_elementStrArr[x];
			cout<<"("<<set_elementStr<<") ";
		}
	}

}

void drawCoverageChart()				//draw the chart
{
	cout<<endl;
	for (int i=0;i<N;i++)
	{
		for (int j = 0;j<N;j++)
		{
			if (A[i][j] == 1)
				cout<<"  X  ";
			else 
			cout<<"  -  ";
		}
		cout<<endl;
	}
}


// Function that removes rows and Coloumns that has prime essential implicants
void RemoveEssn()       //only removes cols with single x, don't work otherwise
{
  bool done = false;
  while (!done)
    done = removeCol() || removeDomCol() || removeRow();
	bool found;
	for(int c=0; c<N; c++)
	{
		found=false;
		int r;
		for(r=0; r<N; r++)
			{
				if (A[r][c]==1)
					{
						if (!found)
						{
							rV.push_back(r);
							found = true;
						}
						else {
							rV.pop_back();break;
						}
					}
			}
	}
		cout<<"Common Prime implicants found and removed in Rows: ";
		for(int r=0; r<rV.size(); r++)
		{
			cout<<rV[r]<<" ";
			for(int c=0; c<N; c++)
			{

				if(A[rV[r]][c] == 1)
					{cV.push_back(c);
				A[rV[r]][c] = 0;}
			}
		}

		cout<<endl;
		cout<<"Essential Prime Implicants found and removed in Columns: ";
		for(int c=0; c<cV.size(); c++)
		{
			cout<<cV[c]<<" ";
			for(int r=0; r<N; r++)
				A[r][cV[c]]=0;
		}
	}