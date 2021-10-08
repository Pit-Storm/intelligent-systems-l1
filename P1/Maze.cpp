#include "Maze.h"

Maze::Maze()
{
 numrows=numcols=0;
 start.first=start.second=0;
 listgoals.clear();
}

Maze::Maze(unsigned nrows,unsigned ncols)
{
 if (nrows>MAX_SIDE)
 {
  cerr << "Error in Maze::Maze(unsigned nrows,unsigned ncols): too many rows (max. " << MAX_SIDE << ")\n";
  exit(1);
 }
 if (ncols>MAX_SIDE)
 {
  cerr << "Error in Maze::Maze(unsigned nrows,unsigned ncols): too many cols (max. " << MAX_SIDE << ")\n";
  exit(1);
 }
 numrows=nrows;
 numcols=ncols;
 start.first=start.second=0;
 listgoals.clear();
 for (unsigned r=0;r<numrows;r++)
  for (unsigned c=0;c<numcols;c++)
   board[r][c]=false;
}

void Maze::ExtractThreeParts(ifstream &f,string &first,string &second,string &third)
{
 char ch;
 unsigned i=0;
 string ldesc1="";
 do
 {
  f >> ch;
  ldesc1.push_back(ch);
  i++;
 }
 while ((ch!='.') && (i<MAX_DESC_LEN));

 if (i>=MAX_DESC_LEN)
 {
  cerr << "Error in Maze::Maze(string filename): too many characters read from file without finding a dot.\n";
  exit(1);
 }

 string ldesc;
 for (i=1;i<ldesc1.size()-1;i++)
  if ((ldesc1[i]!=' ') && (ldesc1[i]!='\t') && (ldesc1[i]!='.'))
   ldesc.push_back(ldesc1[i]);
   
 first="";
 i=1;
 do
 {
  first.push_back(ldesc[i]);
  i++;
 }
 while ((ldesc[i]!=']') || (ldesc[i+1]!=']'));
 i++;
 first.push_back(ldesc[i]);
 
 i+=2;
 second="";
 while (ldesc[i]!=']')
 {
  while ((ldesc[i]==' ') || (ldesc[i]=='\t'))
   i++;
  second.push_back(ldesc[i]);
  i++;
 }
 second.push_back(ldesc[i]);
 i++;

 third="";
 i+=2;
 do
 {
  third.push_back(ldesc[i]);
  i++;
 }
 while ((ldesc[i]!=']') || (ldesc[i+1]!=']'));
 third.push_back(ldesc[i]);

 i=0;
 while (i<first.size())
 {
  if (first[i]=='[')
   numrows++;
  i++;
 }
 i=1;
 while (first[i]!=']')
 {
  if (first[i]==',')
   numcols++;
  i++;
 }
 numcols++;

 if ((numrows>=MAX_SIDE) || (numcols>=MAX_SIDE))
 {
  cerr << "Error: too big maze.\n";
  exit(1);
 }

}

void Maze::FillBoard(string allrows)
{
 size_t pos=0,previous_pos;
 list<string> rows;
 unsigned r=0;
 while (r<numrows)
 {
  previous_pos=(r==0) ? pos+1 : pos+3;
  pos=allrows.find("]",previous_pos);
  rows.push_back(allrows.substr(previous_pos,pos-previous_pos));

  r++;
 }
 
 r=0;
 for (list<string>::iterator it=rows.begin();it!=rows.end();it++)
 {
  string row=(*it);
  if (row.size()!=2*numcols-1)
  {
   cerr << "Error parsing row " << r << " of the maze: it has " << row.size() << " characters instead of the " << 2*numcols << " expected.\n";
   exit(1);
  }
  for (unsigned c=0;c<numcols;c++)
  {
   char s=row[2*c];
   switch (s)
   {
    case '0': SetValue(position(r,c),false); break;
    case '1': SetValue(position(r,c),true); break;
    default:
    {
     cerr << "Error parsing row " << r << ": found character at place " << 2*c << " which is " << s << " instead of '0' or '1'\n";
     exit(1);
    }
   }
  }
  r++;
 }
}

void Maze::MarkStart(string sp)
{
 string n1="",n2="";
 int r=0;
 while ((sp[r]<'0') || (sp[r]>'9'))
  r++;
 while ((sp[r]>='0') && (sp[r]<='9'))
 {
  n1.push_back(sp[r]);
  r++;
 }
 while ((sp[r]<'0') || (sp[r]>'9'))
  r++;
 while ((sp[r]>='0') && (sp[r]<='9'))
 {
  n2.push_back(sp[r]);
  r++;
 }
 SetStart(position((unsigned)atoi(n1.c_str()),(unsigned)atoi(n2.c_str())));
}

void Maze::MarkGoals(string glist)
{
 unsigned r=0;
 while (r<glist.size()-1)
 {
  string n1="",n2="";
  while ((glist[r]<'0') || (glist[r]>'9'))
   r++;
  if (r<glist.size())
  {
   while ((glist[r]>='0') && (glist[r]<='9'))
   {
    n1.push_back(glist[r]);
    r++;
   }
   while ((glist[r]<'0') || (glist[r]>'9'))
    r++;
   while ((glist[r]>='0') && (glist[r]<='9'))
   {
    n2.push_back(glist[r]);
    r++;
   }
   AddGoal(position((unsigned)atoi(n1.c_str()),(unsigned)atoi(n2.c_str())));
  }
 }
}

Maze::Maze(string filename)
{
 numrows=numcols=0;

 ifstream f(filename.c_str());
 if (!f.is_open())
 {
  cerr << "Error in Maze::Maze(string pipename): cannot open file " << filename << endl;
  exit(1);
 }
 
 string rowlist,startpoint,goallist;
 
 ExtractThreeParts(f,rowlist,startpoint,goallist);

 FillBoard(rowlist);
 MarkStart(startpoint);
 MarkGoals(goallist);
}

bool Maze::IsGoal(position p)
{
 bool found=false;

 lp::iterator it=listgoals.begin();
 while ((!found) && (it!=listgoals.end()))
 {
  found=((*it)==p);
  it++;
 }

 return found;
}

void Maze::PrintContent(ostream &outst,position p,lp path)
{
 if (IsGoal(p))
 {
  if (Member(p,path))
   outst << "\033[" << FYellow << ";" << BGreen << "m*\033[0m";
  else
   outst << "\033[" << FGreen << ";" << BBlack << "m*\033[0m";
  return;
 }
 if (p==start)
 {
  if (Member(p,path))
   outst << "\033[" << FYellow << ";" << BBlue << "mo\033[0m";
  else
   outst << "\033[" << FBlue << ";" << BBlack << "mo\033[0m";
  return;
 }
 if (Member(p,path))
 {
  if (board[p.first][p.second])
   outst << "\033[" << FRed << ";" << BYellow << ";5mX\033[0m";
  else
   outst << "\033[" << FBlack << ";" << BYellow << "m \033[0m";
  return;
 }
 
 if (board[p.first][p.second])
 {
  if (Member(p,path))
   outst << "\033[" << FRed << ";" << BYellow << "mX\033[0m";
  else
   outst << "\033[" << FRed << ";" << BBlack << "mX\033[0m";
 }
 else
  outst << ' ';
}

void Maze::Print(ostream &outst,lp path)
{
 outst << "Number of rows   : " << numrows << endl;
 outst << "Number of columns: " << numcols << endl;
 outst << "Start at         : [" << start.first << "," << start.second << "]\n";
 outst << "Goal(s) at       : [ ";
 for (lp::iterator it=listgoals.begin(); it != listgoals.end(); it++)
  outst << "[" << it->first << "," << it->second << "] ";
 outst << "]\n";
 
 if (!path.empty())
 {
  outst << "The currently found path is:\n                   [ ";
   for (lp::iterator it=path.begin(); it != path.end(); it++)
    outst << "[" << it->first << "," << it->second << "] ";
  outst << "]\n";
 }
 outst << "\nRepresentation of the maze (o: Start; *: Goal; X: obstacle; Yellow: path; Blinking: collision).\n\n";
 
 outst << "    |";
 for (unsigned c=0;c<numcols;c++)
  if (c<10)
   outst << (COMPACT_REP ? " |" : "   |");
  else
   outst << (COMPACT_REP ? "" : " ") << c/10 << (COMPACT_REP ? "|" : " |");
 outst <<"\n    |";
 for (unsigned c=0;c<numcols;c++)
  outst << (COMPACT_REP ? "" : " ") << c%10 << (COMPACT_REP ? "|" : " |");
 outst << endl;
 for (unsigned r=0;r<numrows;r++)
 {
  outst << "    ";
  for (unsigned c=0;c<numcols;c++)
   outst << (COMPACT_REP ? "+-" : "+---");
  outst << "+\n";
  if (r<10)
   outst << "   " << r;
  else
   outst << "  " << r;
  for (unsigned c=0;c<numcols;c++)
  {
   outst << (COMPACT_REP ? "|" : " | ");
   PrintContent(outst,position(r,c),path);
  }
  outst << (COMPACT_REP ? "|\n": " |\n");
 }
 outst << "    ";
 for (unsigned c=0;c<numcols;c++)
  outst << (COMPACT_REP ? "+-" : "+---");
 outst << "+\n";
}

void Maze::SetStart(position p)
{
 if (p.first>numrows)
 {
  cerr << "Error in Maze::SetStart(position p): row " << p.first << " outside the limits.\n";
  exit(1);
 }
 if (p.second>numcols)
 {
  cerr << "Error in Maze::SetStart(position p): col " << p.second << " outside the limits.\n";
  exit(1);
 }
 start=p;
}

void Maze::AddGoal(position p)
{
 if (p.first>numrows)
 {
  cerr << "Error in Maze::AddGoal(unsigned row,unsigned col): row " << p.first << " outside the limits.\n";
  exit(1);
 }
 if (p.second>numcols)
 {
  cerr << "Error in Maze::AddGoal(unsigned row,unsigned col): col " << p.second << " outside the limits.\n";
  exit(1);
 }
 lp::iterator it;
 for (it=listgoals.begin(); it!=listgoals.end(); it++)
 {
  if ((*it)==p)
   return;
 }
 listgoals.push_back(p);
}

void Maze::SetValue(position p,bool value)
{
 if (p.first>numrows)
 {
  cerr << "Error in Maze::SetValue(position p): row " << p.first << " outside the limits.\n";
  exit(1);
 }
 if (p.second>numcols)
 {
  cerr << "Error in Maze::SetValue(position p): col " << p.second << " outside the limits.\n";
  exit(1);
 }
 board[p.first][p.second]=value;
}

bool Member(position p,lp set)
{
 lp::iterator it=set.begin();
 bool found=false;
 while (!found && it!=set.end())
 {
  found = ((*it)==p);
  it++;
 }
 return found;
}

