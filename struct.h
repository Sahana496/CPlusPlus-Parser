#include<stdio.h>
#include<fstream>
#include<algorithm>
#include<set>
#include<string>
#include<iostream>
#include<iterator>
#include<string.h>
#include<queue>
#include<vector>
#include<iterator>
#include<map>
using namespace std;

extern int yylex(void);
extern FILE *yyin;
int no=1;
int stateno = 0;

typedef map <string, vector<string> > Set;
typedef multimap <int, pair<string,pair<char,int> > > actionTable;
typedef multimap <int, pair<string,int > > gotoTable;

class Kernel;
class  Item;

vector<Kernel> kernelList;
vector <string>::iterator stringIter;
Set :: iterator mapIter;

actionTable ACTION;
gotoTable GOTO1;


void buildTable(int fromState,int toState,string symbol,char SorR = 'N')
{
	if(isupper(symbol[0]))
	{
		GOTO1.insert(make_pair(fromState,make_pair(symbol,toState)));
	}
	else
	{
		ACTION.insert(make_pair(fromState,make_pair(symbol,make_pair(SorR,toState))));
	}
}
typedef struct production
{
	int num;	
	string head;
	vector <string> rhs;
	int len;
}production;
production p[700];
typedef struct grammar
{
	vector <string> nonterminals;
	vector <string> terminals;
	Set first;
	Set follow;
} grammar;

grammar g;
class Item
{	
	public:
	int dotIndex;
	production prod;
	Item(production p,int pos=-1)
	{
		dotIndex=pos;
		prod=p;
		
	}
	vector <Item> newItemsAfterDot() //Returns the list of all Items that will be produced by the Non terminal after the dot. Eg: S' -> .S : return all productions derivable from S
	{
		vector <Item> itemList;
		string nonTerminalAfterDot;
		nonTerminalAfterDot = prod.rhs[dotIndex];
		for(int i=0;i<no;i++)
		{
			if (p[i].head==nonTerminalAfterDot)
			{ 
				production x;
				x=p[i];
				Item i(x,0);
				itemList.push_back(i); 
			}
		} 
		return itemList;
	}
	Item newItemAfterShift()	//Return the new Item after shifting the dot by one position to the right
	{
		if(dotIndex<prod.len && prod.rhs[dotIndex]!="@"){
			Item i(prod,dotIndex+1);
			return i; }	
		else{
			Item i(prod,-1);
			return i;
		}
	}
	int operator == (Item other)	//Operator overloading == for comparing two items
	{
		if (dotIndex == other.dotIndex)
		{
			
			if (prod.rhs.size() == other.prod.rhs.size())
			{		
				if(prod.head==other.prod.head)
				{
					
					for(int i=0;i<prod.rhs.size() && i<other.prod.rhs.size();i++)
					{ 
						if(prod.rhs[i]!=other.prod.rhs[i])
							return 0;
					} 
					return 1;
				
				}
				else
					return 0;
			}
			else 
				return 0;
		}
		else
			return 0;
	}
	
};

class Kernel
{
	public:
	int state;
	vector <Item> closureItems;
	vector <string> keys;
	Kernel()
	{
		
		closureItems.clear();
		keys.clear();
	}
	void updateClosure()
	{
		vector <Item> ::iterator iter1;
		for(int i=0;i<closureItems.size();i++)
		{
			int dotPos=closureItems[i].dotIndex;							//Find position of dot
			if(dotPos < closureItems[i].prod.rhs.size())  						//Only if dot position less than the productions rhs size
			{
				string x=closureItems[i].prod.rhs[dotPos];
				if ( isupper(x[0]) )								//Check if the first character after dot is a Nonterminal
				{
					vector <Item> newItems = closureItems[i].newItemsAfterDot(); 		//Find all productions that can be derived from this non Terminal
				
					for(int j=0;j<newItems.size();j++)
					{
						if(find(closureItems.begin(),closureItems.end(),newItems[j])==closureItems.end())
						{ 
							closureItems.push_back(newItems[j]);
						}

					}	
				}
			}
			int j=i;
			for(;j<closureItems.size();j++)
			{
				string key;
				int dot=closureItems[j].dotIndex;
				if(dot==closureItems[j].prod.rhs.size())
				{
					key="$";													
				}
				else
					key=closureItems[j].prod.rhs[dot];
				if(key!="@")
					if(find(keys.begin(),keys.end(),key)==keys.end())	//Insert the symbols visible after dot
						keys.push_back(key);
			} 

		}
	}

	Kernel gotos(string X)
	{
	 	Kernel z;
		
		for(int j=0;j<closureItems.size();j++)				 //For every item in closure set
		{
			
			int dot = closureItems[j].dotIndex;
			
			if(closureItems[j].prod.rhs[dot]==X){  			 // check whether symbol after dot is = X
				
				Item i= closureItems[j].newItemAfterShift(); 
				if (i.dotIndex<0);				 // if not valid goto next production do nothing
				else
				{
					if(find(closureItems.begin(),closureItems.end(),i)==closureItems.end())
					{ 
						z.closureItems.push_back(i); 
					}
				}
			}
			
		}
		if(z.closureItems.size()>0){
		z.updateClosure(); 
		return z; }
		else return (*this);
	}
	void display()				//To display the contents of the state
	{
		cout<<"Keys:\t";
		for(int i=0;i<keys.size();i++)
			cout<<keys[i]<<" ";
		cout<<"\nClosure:\n";
		for(int i=0;i<closureItems.size();i++)
		{
			cout<<closureItems[i].prod.head<<" "<<closureItems[i].dotIndex<<" ";
			for (int j=0;j<closureItems[i].prod.rhs.size();j++)
			{
				cout<<closureItems[i].prod.rhs[j];
			} 
			cout<<endl;
		} 
		cout<<endl;
	}
	bool operator == (Kernel k)		//Operator overloading == to compare two states
	{
		if(k.closureItems.size() == closureItems.size())
		{
			for(int i=0;i<k.closureItems.size() && i<closureItems.size();i++)
			{
				if(closureItems[i] == k.closureItems[i]);
				else return 0;
				
			}
			return 1;
		}
		else return 0;
	}
};

typedef struct token	//Lex token
{
	int tokenNum;
	int line;
	int column;
}token;




