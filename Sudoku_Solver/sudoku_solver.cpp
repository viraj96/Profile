/*
  ___________________________________________
/ This code below solves a sudoku of any      \
| level except the one which may be 	       |
| invalid, duh!. I have assumed the puzzle to  |
| be of the size 9*9.					       |
\ 						    -- Viraj Parimi   /
 --------------------------------------------
        \    ,-^-.
         \   !oYo!
          \ /./=\.\______
               ##        )\/\
                ||-----w||
                ||      ||
*/


#include <bits/stdc++.h>
using namespace std;
void solve(int row, int col);
void step(int row, int col);
void print();
int check(int x, int y, int z);
int arr[100][100]={0};	//Made the array global
int main()
{
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			int k;
			cin>>k;
			arr[i][j] = k;
		}
	}
	solve(0,0);
	return 0;
}
void solve(int row, int col)	//Driver program -> Solves the array depending on the value returned by check()
{
	if(row>8)
	{
		print();
		exit(0);
	}
	else
	{
		if(arr[row][col] != 0)
		{
			step(row,col);
		}
		else
		{	
			for(int val=1;val<10;val++)
			{
				if(check(row,col,val))
				{
					arr[row][col] = val;
					step(row,col);
				}
			}
			arr[row][col] = 0;
		}
	}
}
void step(int row, int col)		//Helps in traversing through the array
{
	if(col<8)
	{
		solve(row,col+1);
	}
	else
	{
		solve(row+1,0);
	}
}
void print()		//Prints the filled array
{
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			cout<<" "<<arr[i][j];
		}
		cout<<"\n";
	}
	cout<<"\n";
}
int check(	int x, int y, int z)		//main function over which my program depends
{
	//Check for all row horizontal
	for(int i=0;i<9;i++)
	{
		if(arr[x][i] == z)
			return 0;
	}

	//Check for all row vertical
	for(int i=0;i<9;i++)
	{
		if(arr[i][y] == z)
			return 0;
	}

	//Check for the block in which row,col reside
	x = (x/3)*3;
	y = (y/3)*3;
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(arr[x+i][y+j] == z)
			{
				return 0;
			}
		}
	}
	return 1;
}