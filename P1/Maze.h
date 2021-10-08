#ifndef _MAZE_H
#define _MAZE_H

#include <iostream>
#include <fstream>
#include <string>

#include "structures.h"

#define MAX_SIDE 1024
#define MAX_DESC_LEN 65536
#define COMPACT_REP true

using namespace std;


enum FCol { FBlack=30, FRed, FGreen, FYellow, FBlue, FMagenta, FCyan, FWhite };
enum BCol { BBlack=40, BRed, BGreen, BYellow, BBlue, BMagenta, BCyan, BWhite };

const int Brigth=60;

bool Member(position p,lp set);

class Maze
{
 public: 
	Maze(string pipename);
	
	unsigned GetNumRows() { return numrows; };
	unsigned GetNumCols() { return numcols; };
	
	position GetStart()   { return start; };
	
	bool IsGoal(position p);
	
	lp GetGoals() { return listgoals; };
	
	bool IsObstacle(position p)                { return board[p.first][p.second]; };
	
	void Print(ostream &outst,lp path);
	
	void Print(ostream &outst)	{ lp emptylp; Print(outst,emptylp); };
	
 private:
	unsigned numrows;
	unsigned numcols;
	bool board[MAX_SIDE][MAX_SIDE];
	position start;
	list<position> listgoals;
	
	Maze();
	Maze(unsigned nrows,unsigned ncols);
	
	void ExtractThreeParts(ifstream &f,string &first,string &second,string &third);
	void FillBoard(string allrows);
	void MarkStart(string spoint);
	void MarkGoals(string glist);
	
	void SetValue(position p,bool value);
	
	void AddGoal(position p);
	void SetStart(position p);
	
	void PrintContent(ostream &outst,position p,lp path);
};

#endif

