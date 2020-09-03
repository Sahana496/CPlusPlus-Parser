//#include "struct.h"
#include "lex_header.h"
#include "mapping.h"
#include <stack>
#include <queue>
#include <algorithm>
#include <fstream>
using namespace std;
extern int yylex(void);

extern FILE *yyin;

typedef actionTable :: iterator actionIter;
typedef gotoTable :: iterator gotoIter;
pair < gotoIter, gotoIter > keyRange2;
pair < actionIter, actionIter > keyRange1;


ofstream action;
void printStateStack(stack<int> stateStack)
{
	action<<"State Stack:\n";
	stack<int> dump,temp;
	dump=stateStack;
	while(!dump.empty())
	{
		temp.push(dump.top());
		dump.pop();
	}
	for(dump=temp;!dump.empty();dump.pop())
		action<<dump.top()<<" ";
	action<<"\n\n";

}
void printInputStack(queue<token> inputStack)
{
	action<<"Input Stack:\n";
	for(queue<token> dump=inputStack;!dump.empty();dump.pop())
	{
		action<<mapping1(dump.front().tokenNum)<<" ";
	}
	action<<"\n\n";
}
void printAction(string s,int x)
{
	action<<"Action: ";
	action<<s<<x<<endl<<endl;
}
int parseInput(string filename)
{
	lineno=1;
	int flag=0;
	int y;
	char in;
	token t;
	action.open("Parsing",ios::trunc);
	gotoIter giter;
	actionIter aiter,aiter2;
	ofstream error;
	error.open("Error.txt",ios::trunc);
		
	yyin=fopen(filename.c_str(),"r");
	stack <int> stateStack;
	queue <token> inputStack;
	if(yyin==NULL)
	{
		cout<<"\nFile Not Found";
		exit(0);
	}
	while(y=yylex())
	{	
		t.tokenNum=y;
		t.line=lineno;
		t.column=column;
		inputStack.push(t);
	}
	fclose(yyin);
	token init;
	init.tokenNum=36;
	init.line=t.line+1;
	init.column=t.column+1;
	inputStack.push(init);
	stateStack.push(0);
	int a = inputStack.front().tokenNum;
	printStateStack(stateStack); printInputStack(inputStack);
	//cout<<"Top of state stack: "<<stateStack.top()<<endl;
	//cout<<"Top of input stack: "<<mapping1(inputStack.front().tokenNum)<<endl;
	while (1)
	{
	  	back: keyRange1=ACTION.equal_range(stateStack.top()); //find all rows that have state 0
			
		for(aiter=keyRange1.first;aiter!=keyRange1.second;aiter++) // check all the rows that have state 0
		{
			string s=mapping1(a);	//get symbol on top of input stack
			
			if(aiter->second.first == s)	// if symbol for that state matches the input symbol
			{
				aiter2=aiter;
				aiter2++;
				for(;aiter2!=keyRange1.second;aiter2++) //Shift-reduce error
				{
					if(aiter2->second.first == s){
						
						if('s' == aiter2->second.second.first) aiter=aiter2;
				
					}
				}
				if(aiter->second.second.first== 's') //check if the symbol has shift or not
				{
					
					
						int x=aiter->second.second.second;
						printAction("Shift to state ",x);
						//cout<<"Action: Shift to state "<<x<<endl;
						stateStack.push(aiter->second.second.second); //Push that state on top of state stack
						if(inputStack.front().tokenNum == 36){
							a=inputStack.front().tokenNum; t=inputStack.front();
							//cout<<"Top of state stack: "<<stateStack.top()<<endl;
							//cout<<"Top of input stack: "<<mapping1(a)<<endl;
							
							printStateStack(stateStack); printInputStack(inputStack);
						}
						else
						{
							t=inputStack.front();
							inputStack.pop();	//pop the top input symbol
							a=inputStack.front().tokenNum; // a is the next input symbol
							printStateStack(stateStack); printInputStack(inputStack);
							//cout<<"Top of state stack: "<<stateStack.top()<<endl;
							//cout<<"Top of input stack: "<<mapping1(a)<<endl;
					
						}
						goto back;
					 
				}
				else 
				{	
					int productionNum = aiter->second.second.second; //find production number to reduce
					string head = p[productionNum].head;			// find head of production
					
					int size = inputStack.size();
					if(stateStack.top()==1 && inputStack.front().tokenNum == 36  ) //accepting state
					{
						printStateStack(stateStack); printInputStack(inputStack);
						if(flag==0){ action.close();
						return 1; }
						else{error.close(); action.close();
						return 0;}
					}
					for(int i=0;i<p[productionNum].rhs.size();i++)
					{
						stateStack.pop();		// pop x symbols from stateStack, where x is size of rhs of production
					}
			
					keyRange2  = GOTO1.equal_range(stateStack.top());
					for(giter=keyRange2.first;giter!=keyRange2.second;giter++)
					{
						if( giter->second.first == head )	// if Nonterminal found for this state, push on top of stack
						{
							
								
								stateStack.push(giter->second.second); 
								//cout<<"Reduced to production number: "<<productionNum<<endl;
								printAction("Reduce to production number ",productionNum);
								printStateStack(stateStack); printInputStack(inputStack);
								//cout<<"Action: Reduce to production Number "<<productionNum<<endl;
								//cout<<"Top of state stack: "<<stateStack.top()<<endl;
								//cout<<"Top of input stack: "<<mapping1(a)<<endl;
								goto back;
						}
					} 
				}
			}
		}
		if(aiter == keyRange1.second) //if no entry for the input symbol, error	
		{	
			//cout<<"Here";
			flag =1;
			error<<"Line no: "<<t.line<<" Column: "<<t.column+1<<endl;
			//Error Recovery

		next:	keyRange2  = GOTO1.equal_range(stateStack.top());
			for(giter=keyRange2.first;giter!=keyRange2.second;giter++)
			{
				//action<<giter->second.first;
				if( giter->second.first == "Statement" )	// if Nonterminal found for this state, push on top of stack
				{						
						//cout<<giter->second.second;
						stateStack.push(giter->second.second); 
						goto pop;
				}
			} 
			
			if(giter == keyRange2.second) // If there is no goto[state, Statement] then pop the state from stack
			{
				if(stateStack.size()>1)
				{
					stateStack.pop(); goto next;
				}
				else
				{	error.close();action.close();
					return 0;
				}
				
			}
		pop:	keyRange1=ACTION.equal_range(stateStack.top());			//Find action[state,inputSymbol], if not found then pop the input token until found
			for(aiter=keyRange1.first;aiter!=keyRange1.second;aiter++) 
			{	
				string s=mapping1(inputStack.front().tokenNum);	
				if(aiter->second.first == s)
				{
					a=inputStack.front().tokenNum;
					goto back;
				}
			}
			if(aiter==keyRange1.second)
			{
					if(inputStack.size()>1)
					{
						inputStack.pop();
						goto pop; 
					}
					else
					{	error.close(); action.close();
						return 0;
					}
				
			}

			if(inputStack.size() == 0 || stateStack.size() ==0)
			return 0;
		}
	} 
	
}
