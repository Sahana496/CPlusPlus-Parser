/*###########################
Parser Generator Tool
Grammar should follow the syntax as shown below:
All nonterminals should start with Capital letter
All terminals should be in lowercase
Epsilon is represented by @
Give space after every grammar symbol
DO NOT give any extra new lines after last production in grammar

Example:
Expression identifier
Expression constant
Expression Expression + Expression
Expression Expression - Expression

Here, Expression is a nonterminal
      identifier, constant, + ,- are terminals
Put the Grammar in the file called input
Enter the file name of source code during executing:
./a.out <filename>

Files included in this tool:
1. parser.cpp (Run this file using g++ compiler along with lex generated file
2. parserInput.h
3. struct.h
4. First.h
5. Follow.h
6. mapping.h
7. lex_file.l
8. lex_header.h
9. input (Sample Input file of grammar)
10. program (Sample source code)
11. program1 (")
12. program3 (")

Compile in this way:
$ lex lex_file.l
$ g++ parser.cpp lex.yy.c -o parser
$ ./parser program

here, program is assumed to be the source program to be parsed

###############################*/


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
#include "Follow.h"
#include "parseInput.h"
#include <iomanip>
using namespace std;

ofstream table;

int isUnique(vector<string> array,char* s) // Function to find if the string in an array is unique or not
{
	vector<string>::iterator it;
	it=find(array.begin(),array.end(),s);
	if(it==array.end())
		return 1;
	else
		return 0;

}

void split(string line, production * prod) // Helper function to populate the Production data structure in struct.h head file
{	
	char *p;
	int i=0;
        p=strtok((char *)line.c_str(),(const char *)" ");
	prod->head = p;
	if(isUnique(g.nonterminals,p))
		g.nonterminals.push_back(p);
        while(p!=NULL)
        {
		if(i++>0)
		{
                	prod->rhs.push_back(p);
			if(!isupper(p[0]) || ispunct(p[0]))
			{
				if(isUnique(g.terminals,p))
					g.terminals.push_back(p);
			}
		}
                p=strtok(NULL," "); 
        } 
	prod->len=prod->rhs.size();

}

void printTable() //Function to print the Parse Table
{
	table.open("ParseTable",ios::trunc);
	actionTable :: iterator iter1;
	gotoTable :: iterator iter2;
	iter1=ACTION.begin();
	iter2=GOTO1.begin();
	table<<"\n ACTION TABLE \n";
	table<<"From state\t\tSymbol\t\tShift/Reduce\t\tTo state\n";
	for(;iter1!=ACTION.end();iter1++)
	{
		table<<iter1->first<<"\t\t";
		table<<iter1->second.first<<"\t\t\t\t"<<iter1->second.second.first<<"\t\t"<<iter1->second.second.second<<endl;
	}
	table<<endl<<endl;
	table<<"\n GOTO TABLE\n";
	table<<"From state\t\tSymbol\t\t\t\tTo state\n";
	for(;iter2!=GOTO1.end();iter2++)
	{
		table<<iter2->first<<"\t\t";
		table<<iter2->second.first<<"\t\t\t\t"<<iter2->second.second<<endl;
	}
}
int main(int argc,char *argv[])
{
	
	string line;
	p[0].num=0;
	p[0].head='Z';
	p[0].len=1;
	g.nonterminals.push_back("Z"); // Push initial production S' -> S (S' is Z here)
	vector <string> result;
	result.clear();
	ifstream input,error;
	ofstream firstSet,followSet;
	firstSet.open("FirstSet",ios::trunc);
	followSet.open("FollowSet",ios::trunc);
	input.open("input");
	error.open("Error.txt");
	int i=1;
	cout<<"Compiling....\n";
	if(input.is_open()){
	while(getline(input,line)) // Get all the production from the Grammar file (input) and pass it to helper function split
	{
		split(line, &p[i]);
		
		if(i==1)
			p[0].rhs.push_back(p[i].head);
		
		p[i].num+=no;
		i++;no++;
	}
	input.close();
	}

	for(int j=0;j<g.nonterminals.size();j++)  //Find FIRST set of all nonterminals in the grammar
	{	
		g.first[g.nonterminals[j]].clear();
		FIRST(result,g.nonterminals[j]);
		mapIter=g.first.find(g.nonterminals[j]);
		firstSet<<"FIRST("<<g.nonterminals[j]<<"):\n";
		for(stringIter=mapIter->second.begin();stringIter!=mapIter->second.end();stringIter++)		//Printing first set
		{				
			string t=*stringIter;
			firstSet<<t<<endl;
		} 
	
	} 

	for(int j=0;j<g.nonterminals.size();j++){ //Find FOLLOW set of all nonterminals in the grammar
		
		g.follow[g.nonterminals[j]].clear();
		FOLLOW(g.nonterminals[j],g.nonterminals[j]);
		mapIter=g.follow.find(g.nonterminals[j]);
		followSet<<"FOLLOW("<<g.nonterminals[j]<<"):\n";
		for(stringIter=mapIter->second.begin();stringIter!=mapIter->second.end();stringIter++)	 //Printing follow set	
		{				
			string t=*stringIter;
			followSet<<t<<endl;
		}
	} 

	Item init(p[0],0);	//Initialize 1st item S'->.S
	Kernel k;		//Create the first state of DFA
	
	k.closureItems.push_back(init);
	
	k.updateClosure(); //Find closure set of state 0
	
	k.state=stateno;
	int j=0;
	vector <Kernel> kernelList;
	kernelList.push_back(k); //List containing all the states of the DFA
	cout<<"Constructing LR(0) Item Set...\n";
	while(j<kernelList.size())
	{
	
		k=kernelList[j]; 
		for(int i=0;i<k.keys.size();i++) //For all sysmbols that can be seen from the state
		{
			
			string x;
			x= k.keys[i];
			
			if(x!="$") // if not end of production
			{
				vector <Kernel> :: iterator iter;
				Kernel z ;
				z= (k.gotos(x)); //Find the next state
				iter= find(kernelList.begin(),kernelList.end(),z);
				if( iter== kernelList.end())				//check if state already present, if not push it to the list
				{	
					z.state= ++stateno;
					kernelList.push_back(z); 
				
					buildTable(k.state,z.state,x,'s');	//Simultaneously update the parse table (Here shift to new state)
				}
				else
				{	
					buildTable(k.state,iter->state,x,'s'); //If state already present, then just update table
				
				}

			}
			else 
			{
				int n=0;
				for(;n<k.closureItems.size();n++)
				{
					string key;
					int dot=k.closureItems[n].dotIndex;
					if(dot==k.closureItems[n].prod.rhs.size())
					{					
						Set :: iterator followIter;
						vector <string> :: iterator stringIter1;
					
						followIter = g.follow.find(k.closureItems[n].prod.head); 		// Find follow set of head of production
						for(stringIter1=followIter->second.begin();stringIter1!=followIter->second.end();stringIter1++) //For all items in the follow set 
						{
							buildTable(k.state,k.closureItems[n].prod.num,*stringIter1,'r'); //Add the reduction to parse Table
						}																			
					}
				
				} 
			}
		
		}  
			j++;
		

	}
	cout<<"Building Parse Table...\n";
	printTable(); //Print parse table
	char c='y';
	string filename;
	while(1){
	fflush(stdout);
	cout<<"Filename: ";
	cin>>filename;
	if(parseInput(filename)) //Call function to Parse input
	{
		cout<<"Compiled Successfully\n"; error.close();
	}
	else
	{
		error.open("Error.txt");
		error.seekg(0, ios::beg);
		error.clear();
		cout<<"Syntax Error at: \n";
		if(error.is_open()){
			cout<<error.rdbuf();error.close();}
	}
	cout<<"\nAnother file(y/n): ";
	cin>>c;
	if(c=='n') break;
	}
	
}
