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
#include "struct.h"
using namespace std;

void addToResultSet(vector <string> &Result,string val,string head,int x)
{
	int k;
	
	for(k=0;k<Result.size();k++)
		if(Result[k]==val)
			return; 
	Result.push_back(val);
	mapIter=g.first.find(head);
	vector<string> ::iterator w;
	if ((w=find(mapIter->second.begin(),mapIter->second.end(),val))==mapIter->second.end())
	g.first[head].push_back(val);

		
}
void FIRST(vector <string> &Result, string c)
{
	int i,k,j;
	vector <string> subResult;
	subResult.clear();
	Result.clear();
	int foundEpsilon;
	if(!(isupper(c[0])))
	{
		addToResultSet(Result,c,"X",0); // Beacause map doesn't contain terminals as the key
		return;
	}
	
	for(i=0;i<no;i++)
	{
		
		if(p[i].head==c)
		{
			if(p[i].rhs[0]=="@")
			{
				addToResultSet(Result,"@",p[i].head,0);	
				break;
				
			}
		
		 	else
			{
				j=0;
				while(j<p[i].rhs.size())
				{
					foundEpsilon=0;
					
					if(p[i].rhs[j]==p[i].head)
					{
						mapIter=g.first.find(p[i].head);
						for(stringIter=mapIter->second.begin();stringIter!=mapIter->second.end();stringIter++)
						{						
							string t=*stringIter;							
							addToResultSet(Result,t,p[i].head,0);													
						}
						for(stringIter=mapIter->second.begin();stringIter!=mapIter->second.end();stringIter++)
						{
							if(*stringIter=="@")
							{
								foundEpsilon=1; 
								break; 
							}
						}
						if(!foundEpsilon)
		            				 break;
						else 
						{
							 j++; continue;
						}
						
					}
					foundEpsilon=0;
					FIRST(subResult,p[i].rhs[j]);
					
					for(k=0;k<subResult.size();k++)
					{
						addToResultSet(Result,subResult[k],p[i].head,0);												
					}
					
					for(k=0;k<subResult.size();k++)
						if(subResult[k]=="@")
		            	 		{
		                 			foundEpsilon=1;
		                 			break;
						}                       

		        		if(!foundEpsilon)
		            			 break;

		         		j++;
				}
			}
		}
	}
	
	return;
}
