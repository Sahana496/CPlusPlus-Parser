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
#include "First.h"

#include <ctype.h>
using namespace std;
Set :: iterator mapIter1;
vector<string>::iterator stringIter1;
string headflag;
Set followCheck;
void first1(production *,int,string);
void FOLLOW(string,string);
void addToResult(string val,string head)
{
	int k;
	mapIter1=g.follow.find(head);	//Find the Non terminal in the grammar's follow set
	
	for(stringIter1=mapIter1->second.begin();stringIter1!=mapIter1->second.end();stringIter1++){ //Checking whether this terminal (val) already exists in the follow set
		if(*stringIter1==val)
			return; }
	g.follow[head].push_back(val); // Push the terminal on to the follow set

		
}

void first1(production *p,int j,string head)
{
	int k,flag=0;
	while(j<p->rhs.size())
	{
		flag=0;
		string c=p->rhs[j];
		
		if(!(isupper(c[0])) ){ //If the next symbol is a terminal, then add to the follow set and break
		
		addToResult(c,head); break; }
		Set :: iterator mapIter=g.first.find(c); // Otherwise, find all the symbols in the First set 
		vector<string>::iterator stringIter;
		
		stringIter=mapIter->second.begin();
		
		
		for(;stringIter!=mapIter->second.end();stringIter++){ 
			
			if(*stringIter=="@") // If epsilon found, flag it
			{
				flag=1;
			}
			else
			{
				addToResult(*stringIter,head); // add symbol to follow set
			} 
			
		}
		if(flag)
		{	
			if(!(j+1==p->rhs.size()))   // if epsilon found, then again find the first of next symbol in grammar
			{
				p->rhs[++j];
				string c=p->rhs[j];
			}
			
		}
		else break; 
	} 
	
	if(j==p->rhs.size()) // if epsilon present in all non terminals, then find follow of head of production
		FOLLOW(p->head,head);
}

void FOLLOW(string c,string head)
{
	
	if(p[0].head==c){ 
		addToResult("$",head); return;
	 }
		
	for(int i=0;i<no;i++)
	{
		
	next:	for(int j=0;j<p[i].rhs.size();j++)
		{
			
			if(p[i].rhs[j]==c)
			{
				
				if(!(j+1==p[i].rhs.size())) { 
					first1(&p[i],j+1,head); } // if there is a symbol after the non terminal for which follow is to be found, call first 
			
				if(j+1==p[i].rhs.size() && c!=p[i].head){ 
					mapIter1=followCheck.find(head);
					for(stringIter1=mapIter1->second.begin();stringIter1!=mapIter1->second.end();stringIter1++){ 
					if(*stringIter1==p[i].head) {
						i++; goto next; }}
					followCheck[head].push_back(p[i].head);
					
					FOLLOW(p[i].head,head); //Otherwise find the follow of the head of production
					 }
			}
		}
	}
	


}

