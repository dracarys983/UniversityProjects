/* Artificial Intelligence - Spring '14
 * BackGammon AI program
 * Assignment 1 - Phase II
 * Team : Somya Mehdiratta (201201007)
 * 	  Kalpit Thakkar (201201071)
 * 
 */

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<unistd.h>
#include<cstring>
#include<time.h>

using namespace std;
// Funtion Decalrations

void initMoves(int x, int y, int a, int b, int board1[25]);
int generateDiceMoves(int x, int y, int a, int b, int board1[25]);
int calculateExpectedValue(int values[], int p[],int ind);
int evaluate(int board[], int a, int b);
int pips(int C[]); 
int vulPoints(int C[]);
int bar(int a, int b);
int twentyPointHolding(int C[]); //um
int barPointHolding(int C[]); //um
int holdingPoints(int C[]);
int oppVulPoints(int C[]); 
int opponent_pips(int C[]);
int Prime(int C[25]);
int keith_count(int C[25]);
int opp_keith_count(int C[25]);
void printBoard();
void printMove(int ind);
int utility(int start,int end, int b);
int play(int r, int b);
int BearOff(int bearState[25], int a,int b,int r1,int r2,int bearMoves);

// Structure Init
typedef struct Board
{
	int config[25];
	int bar_a, bar_b;
	float hValue;
	int s1, s2, e1, e2;
	int r1, r2;
}bd;

vector <bd> initialMoves(150);

// Global Variables
int d1, d2, board[25], aBar = 0, bBar = 0, moves = 0, heuristicValues[30], diceProbab[30], count = 0, finalMove, ghusa = 0;
float maximum = -10000;
int CD[25];

int main()
{
	/**** Seed the random generator *****/ 
	srand((time(NULL)));		
	/***********************************/

	for(int i = 1; i < 25; i++)
		cin>>board[i];
	char c; 
	c = getchar(); 
	while((c=getchar()) != '\n') 
	{ 
		if(c=='a') 
			++aBar; 
		else if(c=='b') 
			++bBar; 
	}
	cin>>d1>>d2;
	//	cout<< "INPUT bar and die rolls "<<aBar<<" " <<bBar<<" "<<d1<<" "<<d2<<endl; 

	/********* Always start with higher roll *********/
	if(d1<d2) {
		int temp  = d1;
		d1 = d2;
		d2 = temp;
	}
	/***********************************************/

	if(BearOff(board, aBar, bBar, d1, d2, 0))
	{}
	else
	{
		initMoves(d1, d2, aBar, bBar, board);
		for(int i = 0; i < moves; i++)
		{
			for(int j = 1; j <= 6; j++)
			{
				count = 0;
				for(int k = j; k <= 6; k++)				// Rolling doubles allowed
				{
					heuristicValues[count] = generateDiceMoves(j, k, initialMoves[i].bar_a, initialMoves[i].bar_b, initialMoves[i].config);
					if(j == k)
						diceProbab[count] = 1/36;
					else
						diceProbab[count] = 1/18;
					count++;
				}
			}
			int temp = calculateExpectedValue(heuristicValues, diceProbab, i);
			if(temp)
				finalMove = temp;
		}
		printMove(finalMove);
	}
	//	printBoard();
}

void printMove(int ind)
{
	if(initialMoves[ind].s1 == 37)
		cout<<"Z"<<" "<<initialMoves[ind].e1<<endl;
	else
	{
		if(initialMoves[ind].s1 != -1 && initialMoves[ind].s1 != 0)
			cout<<initialMoves[ind].s1<<" "<<initialMoves[ind].e1<<endl;
		else
			cout<<"pass"<<endl;
	}
	if(initialMoves[ind].s2 == 37)
		cout<<"Z"<<" "<<initialMoves[ind].e2<<endl;
	else
	{
		if(initialMoves[ind].s2 != -1 && initialMoves[ind].s2 != 0)
		{
			cout<<initialMoves[ind].s2<<" "<<initialMoves[ind].e2<<endl;
		}
		else
		{
			cout<<"pass"<<endl;
		}
	}
}

void printBoard(){
	for(int i = 0; i < moves; i++) {
		cout<<"Board state : \n";
		for(int j = 1; j < 25; j++)
			cout<<initialMoves[i].config[j]<<" ";
		cout<<endl;
		//		cout<<"bar variables : "<<initialMoves[i].bar_a<<" "<<initialMoves[i].bar_b<<endl<<endl;
	}
}

void initMoves(int x, int y, int a, int b, int board1[25])
{
	bd temp;
	temp.s1 = -1;
	temp.e1 = -1;
	temp.s2 = -1;
	temp.e2 = -1;
	temp.r1 = x;
	temp.r2 = y;
	temp.bar_a = a;
	temp.bar_b = b;
	int done_moves = 0, flag_x = 0, flag_y = 0, flag = 0;
	if(!a)
	{
		for(int i = 1; i < 25; i++)
		{
			flag_x = 0, flag_y = 0, flag = 0;
			done_moves = 0;
			int sum = x + y;
			if(board1[i] > 0 && board1[i + x] >= -1 && i + x < 25)
			{
				if(board1[i + x] == -1)
				{
					b++;
					flag_x = 2;
					board1[i + x] = 1;
					temp.bar_b = b--;
				}
				else
					board1[i + x]++;
				board1[i]--;
				done_moves++;
				temp.s1 = i;
				temp.e1 = i + x;
				sum -= x;
				flag_x = 1;
			}
			else if(board1[i] > 0 && board1[i + y] >= -1 && i + y < 25)
			{
				if(board1[i + y] == -1)
				{
					b++;
					flag_y = 2;
					board1[i + y] = 1;
					temp.bar_b = b--;
				}
				else
					board1[i + y]++;
				board1[i]--;
				done_moves++;
				temp.s1 = i;
				temp.e1 = i + y;
				sum -= y;
				flag_y = 1;
			}
			if(sum < x + y)
			{
				for(int j = 1; j < 25; j++)
				{
					flag = 0;
					if(board1[j] > 0 && board1[j + sum] >= -1 && (j + sum) < 25)
					{
						if(board1[j + sum] == -1)
						{
							b++;
							flag = 2;
							board1[j + sum] = 1;
							temp.bar_b = b--;
						}
						else
							board1[j + sum]++;
						board1[j]--;
						done_moves++;
						temp.s2 = j;
						temp.e2 = j + sum;
						for(int k = 1; k < 25; k++)
						{
							temp.config[k] = board1[k];
						}
						if(done_moves >= 1)
						{
							ghusa = 1;	
							initialMoves[moves++] = temp;
						}
						board1[j]++;
						if(flag == 2)
							board1[j + sum] = -1;
						else
							board1[j + sum]--;
					}
				}
				board1[i]++;
				if(flag_x == 1)
					board1[i + x]--;	
				else if(flag_y == 1)
					board1[i + y]--;
				else if(flag_x == 2)
					board1[i + x] = -1;
				else if(flag_y == 2)
					board1[i + y] = -1;

			}
		}
	}
	else 
	{
		done_moves = 0, flag_x = 0, flag_y = 0;
		int sum = x + y;
		if(a >= 2)
		{
			if(board1[x] >= -1)
			{
				if(board1[x] == -1)
				{
					b++;
					flag_x = 2;
					board1[x] = 1;
				}
				else
				{
					board1[x]++;
					flag_x = 1;
				}
				a--;
				temp.s1 = 37;
				temp.e1 = x;
				done_moves++;
				temp.bar_a = a++;
				temp.bar_b = b--;
			}
			if(board1[y] >= -1)
			{
				if(board1[y] == -1)
				{
					b++;
					flag_y = 2;
					board1[y] = 1;
				}
				else
				{
					board1[y]++;
					flag_y = 1;
				}
				a--;
				if(done_moves == 0)
				{
					temp.s1 = 37;
					temp.e1 = y;
				}
				else if(done_moves == 1)
				{
					temp.s2 = 37;
					temp.e2 = y;
				}
				done_moves++;
				temp.bar_a = a++;
				temp.bar_b = b--;
			}
			for(int k = 1; k < 25; k++)
			{
				temp.config[k] = board1[k];
			}
			if(done_moves >= 1)
			{
				ghusa = 1;
				initialMoves[moves++] = temp;
			}
			if(flag_x == 2 && flag_y == 2)
			{
				board1[x] = -1;
				board1[y] = -1;
			}
			else if(flag_x == 2)
				board1[x] = -1;
			else if(flag_y == 2)
				board1[y] = -1;
			else if(flag_x == 1)
				board1[x]--;
			else if(flag_y == 1)
				board1[x]--;
		}
		else if(a == 1)
		{
			if(board1[x] >= -1)
			{
				if(board1[x] == -1)
				{
					b++;
					flag_x = 2;
					board1[x] = 1;
				}
				else
				{
					board1[x]++;
					flag_x = 1;
				}
				a--;
				temp.s1 = 37;
				temp.e1 = x;
				done_moves++;
				temp.bar_a = a++;
				temp.bar_b = b--;
				sum -= x;
			}
			else if(board1[y] >= -1)
			{
				if(board1[y] == -1)
				{
					b++;
					flag_y = 2;
					board1[y] = 1;
				}
				else
				{
					board1[y]++;
					flag_y = 1;
				}
				a--;
				temp.s1 = 37;
				temp.e1 = y;
				done_moves++;
				temp.bar_a = a++;
				temp.bar_b = b--;
				sum -= y;
			}
			if(sum < x + y)
			{
				for(int j = 1; j < 25; j++)
				{
					flag = 0;
					if(board1[j] > 0 && board1[j + sum] >= -1 && j + sum < 25)
					{
						if(board1[j + sum] == -1)
						{
							b++;
							board1[j + sum] = 1;
							flag = 2;
							temp.bar_b = b--;
						}
						else
							board1[j + sum]++;
						board1[j]--;
						done_moves++;
						temp.s2 = j;
						temp.e2 = j + sum;
						for(int k = 1; k < 25; k++)
						{
							temp.config[k] = board1[k];
						}
						if(done_moves >= 1)
						{
							ghusa = 1;
							initialMoves[moves++] = temp;
						}
						board1[j]++;
						if(flag == 2)
							board1[j + sum] = -1;
						else
							board1[j + sum]--;
					}
				}
				if(flag_x == 2)
					board1[x] = -1;
				else if(flag_y == 2)
					board1[y] = -1;
				else if(flag_x == 1)
					board1[x]--;
				else if(flag_y == 1)
					board1[y]--;
			}
		}
	}
	if(ghusa == 0)
		initialMoves[moves++] = temp;
}

int generateDiceMoves(int x, int y, int a, int b, int board1[25])
{
	int value = 10000;
	bd temp;
	temp.r1 = x;
	temp.r2 = y;
	temp.bar_a = a;
	temp.bar_b = b;
	int done_moves = 0, flag_x = 0, flag_y = 0, flag = 0;
	if(!b)
	{
		for(int i = 24; i > 0; i--)
		{
			flag_x = 0, flag_y = 0, flag = 0;
			done_moves = 0;
			int sum = x + y;
			int last_flag = 0;
			if(i - x > 0)
			{
				if(board1[i] < 0 && board1[i - x] <= 1)
				{
					if(board1[i - x] == 1)
					{
						a++;
						flag_x = 2;
						board1[i - x] = -1;
						temp.bar_a = a--;
					}
					else{
						board1[i - x]--;
						flag_x = 1;
					}
					board1[i]++;
					done_moves++;
					last_flag = 1;
					temp.s1 = i;
					temp.e1 = i - x;
					sum -= x;
				}
			}
			if(i - y > 0 && last_flag == 0) {
				if(board1[i] < 0 && board1[i - y] <= 1 && i - y > 0)
				{
					if(board1[i - y] == 1)
					{
						a++;
						flag_y = 2;
						board1[i - y] = -1;
						temp.bar_a = a--;
					}
					else{
						board1[i - y]--;
						flag_y = 1;
					}
					board1[i]++;
					done_moves++;
					temp.s1 = i;
					temp.e1 = i - y;
					sum -= y;
				}
			}
			if(sum < x + y)
			{
				for(int j = 24; j > 0 ; j--)
				{
					flag = 0;
					if(j - sum > 0)
					{
						if(board1[j] < 0 && board1[j - sum] <= 1 && (j - sum) > 0)
						{
							if(board1[j - sum] == 1)
							{
								a++;
								flag = 2;
								board1[j - sum] = -1;
								temp.bar_a = a--;
							}
							else
								board1[j - sum]--;
							board1[j]++;
							done_moves++;
							temp.s2 = j;
							temp.e2 = j - sum;
							for(int k = 1; k < 25; k++)
								temp.config[k] = board1[k];
							int h = evaluate(temp.config, temp.bar_a, temp.bar_b);
							if(value > h)
								value = h;
							board1[j]--;
							if(flag == 2)
								board1[j - sum] = 1;
							else
								board1[j - sum]++;
						}
					}
				}
				board1[i]--;
				if(flag_x == 1)
					board1[i - x]++;	
				else if(flag_y == 1)
					board1[i - y]++;
				else if(flag_x == 2)
					board1[i - x] = 1;
				else if(flag_y == 2)
					board1[i - y] = 1;

			}
		}
	}
	else 
	{
		done_moves = 0, flag_x = 0, flag_y = 0;
		int sum = x + y;
		if(b >= 2)
		{
			if(board1[25-x] <= 1)
			{
				if(board1[25-x] == 1)
				{
					a++;
					flag_x = 2;
					board1[25-x] = -1;
				}
				else
				{
					board1[25 - x]--;
					flag_x = 1;
				}
				b--;
				temp.s1 = 37;
				temp.e1 = 25 - x;
				done_moves++;
				temp.bar_a = a--;
				temp.bar_b = b++;
			}
			if(board1[25 - y] <= 1)
			{
				if(board1[25 - y] == 1)
				{
					a++;
					flag_y = 2;
					board1[25 - y] = -1;
				}
				else
				{
					board1[25 - y]--;
					flag_y = 1;
				}
				b--;
				if(done_moves == 0)
				{
					temp.s1 = 37;
					temp.e1 = 25- y;
				}
				else if(done_moves == 1)
				{
					temp.s2 = 37;
					temp.e2 = 25 - y;
				}
				done_moves++;
				temp.bar_a = a--;
				temp.bar_b = b++;
			}
			for(int k = 1; k < 25; k++)
				temp.config[k] = board1[k];
			int h = evaluate(temp.config, temp.bar_a, temp.bar_b);
			if(value > h)
				value = h;
			if(flag_x == 2 && flag_y == 2)
			{
				board1[25 - x] = 1;
				board1[25 - y] = 1;
			}
			else if(flag_x == 2)
				board1[25 - x] = 1;
			else if(flag_y == 2)
				board1[25 - y] = 1;
			else if(flag_x == 1)
				board1[25 - x]++;
			else if(flag_y == 1)
				board1[25 - x]++;
		}
		else if(b == 1)
		{
			if(board1[25 - x] <= 1)
			{
				if(board1[25 - x] == 1)
				{
					a++;
					flag_x = 2;
					board1[25 - x] = -1;
				}
				else
				{
					board1[25 - x]--;
					flag_x = 1;
				}
				b--;
				temp.s1 = 37;
				temp.e1 = 25 - x;
				done_moves++;
				temp.bar_a = a--;
				temp.bar_b = b++;
				sum -= x;
			}
			else if(board1[25 - y] <= 1)
			{
				if(board1[25 - y] == 1)
				{
					a++;
					flag_y = 2;
					board1[25 - y] = -1;
				}
				else
				{
					board1[25 - y]--;
					flag_y = 1;
				}
				b--;
				temp.s1 = 37;
				temp.e1 = 25 - y;
				done_moves++;
				temp.bar_a = a--;
				temp.bar_b = b++;
				sum -= y;
			}
			if(sum < x + y)
			{
				for(int j = 24; j > 0; j--)
				{
					flag = 0;
					if(j - sum > 0)
					{
						if(board1[j] < 0 && board1[j - sum] <= 1 && j - sum >0)
						{
							if(board1[j - sum] == 1)
							{
								a++;
								board1[j - sum] = -1;
								flag = 2;
								temp.bar_a = a--;
							}
							else
								board1[j - sum]--;
							board1[j]++;
							done_moves++;
							temp.s2 = j;
							temp.e2 = j - sum;
							for(int k = 1; k < 25; k++)
								temp.config[k] = board1[k];
							int h = evaluate(temp.config, temp.bar_a, temp.bar_b);
							if(value > h)
								value = h;
							board1[j]--;
							if(flag == 2)
								board1[j - sum] = 1;
							else
								board1[j - sum]++;
						}
					}
				}
				if(flag_x == 2)
					board1[25 - x] = 1;
				else if(flag_y == 2)
					board1[25 - y] = 1;
				else if(flag_x == 1)
					board1[25 - x]++;
				else if(flag_y == 1)
					board1[25 - y]++;
			}
		}
	}
	return value;
}

int calculateExpectedValue(int values[], int p[], int ind)
{
	float exp = 0;
	for(int i = 0; i < count; i++)
		exp += p[i]*values[i];
	initialMoves[ind].hValue = exp;
	if(maximum < exp)
	{
		maximum = exp;
		return ind;
	}
	return 0;
}

int keith_count(int C[25]){

	int dist = 0; 
	for(int i = 1; i < 25; i++)
	{
		if(C[i] > 0){
			dist += C[i]*(25-i);
			if (i == 24 && C[i]>1 )
				dist += (2*(C[i]-1));
			if(i == 23 && C[i]>1)
				dist += C[i]-1;
			if(i == 22 && C[i]>3)
				dist += C[i]-3;
			if(i == 21 && C[i] ==0)
				dist+=1;
			if(i == 20 && C[i] ==0)
				dist+=1;
			if(i == 19 && C[i] ==0)
				dist+=1;
		}
	}
	if(dist == 0 )
		return -1000; //WINS

	dist = dist + dist/7;
	int opp = opp_keith_count(C);
	if(dist - opp <=4)
		dist*= 2;
	if(dist - opp<=3)
		dist*= 2;
	return dist;   //less the better
}


int opp_keith_count(int C[25]){
	int dist = 0; 
	for(int i = 1; i < 25; i++)
	{
		if(C[i] < 0){
			C[i] = -C[i]; 
			dist += C[i]*(i);
			if (i == 1 && C[i]>1 )
				dist += (2*(C[i]-1));
			if(i == 2 && C[i]>1)
				dist += C[i]-1;
			if(i == 3 && C[i]>3)
				dist += C[i]-3;
			if(i == 4 && C[i] ==0)
				dist+=1;
			if(i == 5 && C[i] ==0)
				dist+=1;
			if(i == 6 && C[i] ==0)
				dist+=1;
		}
	}
	dist = dist + dist/7;
	return dist;   //less the better
}

int vulPoints(int C[25]) //our vulnerable points - can be killed easily.
{
	int nBlots = 0;
	for(int i = 1; i < 25; i++)
	{
		if(C[i] == 1)
			nBlots++;
	}
	return nBlots;  //less the better
}

int oppVulPoints(int C[25]){
	int temp;
	for(int i=1; i<25; i++)
		if(C[i] == -1){
			if(C[i-1] >=1 || C[i-2] >=1 || C[i-3] >=1 || C[i-4]>=1 || C[i-5]>=1 || C[i-6]>=1) //opp's blot exposed;attackable next move
				temp -= 5;
			else temp -= 2; //opponent's blot exposed.
		}
	return temp;
}

int holdingPoints(int c[25]){
	int temp = 0;
	for (int i = 1; i < 25; ++i)
	{
		if (c[i] == 2)
			-- temp; //less the better
	}
	return temp;
}

int bar(int a,int b) // THE CHECKER IS ON THE BAR!
{
	if(a > 0 && b == 0)
		return 200;
	else if(a>0 && b>0)
		return 150;
	return 0;  
}

int Prime(int C[25]){
	for(int i = 19; i <= 22; i++)		
		if(C[i]>=2 && C[i+1]>=2 && C[i+2]>=2)
			return -25;
	return 0;
}

int twentyPointHolding(int C[25])
{
	if(C[19] >= 2)
		return 1;
	else if(C[19] == 1)
		return 5;
	else 
		return 8;
}

int barPointHolding(int C[25])
{
	if(C[18] >= 2)
		return -20;
	else if(C[18] == 1)
		return 20;
	return 8;
}

int bar1PointHolding(int C[25])
{
	if(C[7] >= 2)
		return -20;
	else if(C[7] == 1)
		return 20;
	return 8;
}

int evaluate(int C[24],int a,int b)
{
	int p1 = 1, p2 = 2, p3 = 3;
	//p4=4,
	int p5 = 5, p6 = 6, p8 = 8;
	//p7=7
	int value1, value2, value3, value4, value5,value6,value7,value8, value9;

	value1 = keith_count(C); 

	value2 = vulPoints(C);

	value3 = bar(a,b);

	value4 = twentyPointHolding(C); //um

	value5 = barPointHolding(C); //um

	value6 = holdingPoints(C);

	value7=  oppVulPoints(C); 

	value8 = Prime(C);

	value9 = bar1PointHolding(C);

	float p = (float)rand()/(float)(RAND_MAX);
	if(p <= 0.4)
		return p6*value1 + p2*value2 + p8*value3 + p1*value4 + p1*value5 + p3*value6 + p8*value7 + p2*value8 + p2*value9; //attack.
	else
		return p8*value1 + p3*value2 + p8*value3 + p1*value4 + p1*value5 + p3*value6 + p5*value7 + p3*value8 + p3*value9; //all round
}

int BearOff(int bearState[25], int a, int b, int r1, int r2, int bearMoves)
{

	for(int i =1;i<=24; i++)
		CD[i] = bearState[i];

	int temp = 0; 
	for(int i=1;i<=18;i++)
	{
		if(CD[i] >= 0)
			temp += CD[i];
	}
	if(a != 0 || temp != 0)
		return 0;  //can't bearoff
	// if bearoff possible ---->
	int bsum = r1 + r2;
	for(int i=19;i<=24;i++){
		if(CD[i]>=1 && i+r1==25 && bearMoves <= 1){
			bearMoves++;
			bsum -= r1;
			CD[i]-= 1;
			cout<<i<<" O"<<endl;		
		}

		if(CD[i]>=1 && i+r2==25 && bearMoves <= 1){
			bearMoves++;	
			CD[i]-= 1;
			bsum -= r2;
			cout<<i<<" O"<<endl;
		}	
		if(CD[i]>=1 && i+r2+r1 == 25 && bearMoves <= 1 && (CD[r1]> -2 || CD[r2]>-2)){
			bearMoves +=2;	
			bsum = 0;
			if(CD[r1] > -2 )
			cout<<i<<" "<<i+r1<<"\n"<<i+r1<<" O"<<endl;
			else if(CD[r2] > -2 )
			cout<<i<<" "<<i+r2<<"\n"<<i+r2<<" O"<<endl;
			CD[i]-= 1;
		}	


	}

	if(bearMoves == 2)
		return 1; //all done.

	// note - no bar variables here.

	if(bearMoves != 2) {

		if(bsum == r1 + r2)
		{
			int temp = play(r1,b);
			bearMoves += temp;
			if (temp == 1)
				bsum -= r1;
		}
		else if(bsum != 0)
		{
			int temp = play(bsum, b);
			bearMoves += temp;
			if(temp == 1)
				bsum = 0;
		}
	}

	if(bearMoves != 2) {

		if(bsum == r1 + r2)
		{
			int temp = play(r2,b);
			bearMoves += temp;
			if (temp == 1)
				bsum -= r2;
		}
		else if(bsum != 0)
		{
			int temp = play(bsum, b);
			bearMoves += temp;
			if(temp == 1)
				bsum = 0;
		}
	}

	if(bearMoves != 2) {

		if(bsum == r1 + r2)
		{
			for(int i = 19;i <= 24; i++) {

				if(CD[i]>=1 && i+r1>=25 && bearMoves <= 1) {
					bearMoves ++;
					CD[i]-= 1;
					cout<<i<<" O"<<endl;		
				}

				if(CD[i]>=1 && i+r2>=25 && bearMoves <= 1) {
					bearMoves++;	
					CD[i]-= 1;
					cout<<i<<" O"<<endl;
				}	

			}
		}		
		else if(bsum != 0)
		{
			for(int i = 19;i <= 24; i++) {

				if(CD[i]>=1 && i+bsum>=25 && bearMoves <= 1) {
					bearMoves ++;
					CD[i]-= 1;
					cout<<i<<" O"<<endl;		
				}
			}
		}
	}


	if(bearMoves == 1)
		cout<<"pass"<<endl;
	else if(bearMoves == 0)
		cout<<"pass\npass\n";

	return 1;
}



int play(int r, int b) {

	int x , y , u = -10000, e;

	for (int i = 19; i <= 24; ++i)
	{
		e = i + r;
		if( CD[e] >= -1 && CD[i] >=1 && e<=24 ){ //legal check
			int temp = utility(i,e,b);
			if(temp > u){
				u=temp;  
				x = i; y =e;
			}
		}
	}

	if(u != -10000){
		cout<<x<<" "<<y<<endl;
		CD[x]--;
		CD[y]++;
		return 1;
	}
	return 0;
}

int utility(int start,int end, int b){
	
	int temp = 0;
	if(end == 24 || end == 23)
		temp -= 5;
	if(end >=23 && CD[end] >= 2)
		temp -= 8; 
	if(CD[start] == 2 && b >=1)
		temp -= 50;
	else if(CD[start] == 2 )
		temp -= 10;

	if(CD[end] == 1)
		temp += 10;

	if(CD[end] ==1){
		if(end -1 >=19 && CD[end-1] >=2)
			temp += 15;
		if(end + 1 <=24)
			if(CD[end+1] >= 2)
				temp += 15;
	}

	if(end>=22 && CD[end] >= 2)
		temp -= 2;
	else if(end>=22 && CD[end] >= 3)
		temp -= 5;
	else if(end>=22 && CD[end] >= 4)
		temp -= 15;

	if(CD[end] >= 3)
		temp -= 10;

	return temp;
}
