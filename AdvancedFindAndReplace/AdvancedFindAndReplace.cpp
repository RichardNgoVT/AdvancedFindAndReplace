// AdvancedFindAndReplace.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <utility>      // std::pair, std::make_pair
#include <string>       // std::string
#include <fstream>

using namespace std;

char CaseIgnore(char c, bool CaseSpec)
{
	if (c >= 'A' && c <= 'Z' && CaseSpec == false)
	{
		c = c + ' ';
	}
	return c;
};


int main()
{
	ifstream in("input.txt");
	ofstream out("output.txt");
	char varBegin;
	char varEnd;
	string Ender;
	int endCount = 0;//if char == Ender[endCount], endCount++
	string linemover;
	bool exists = false;
	bool ignorer = false;
	bool CaseSpec = false;

	//Options (INCLUDE CASE SENSITIVE OPTION, function that converts to lowercase if bool argument is true
	getline(in, linemover);
	getline(in, linemover);


	varBegin = in.get();
	in.ignore();
	varEnd = in.get();
	in.ignore();
	getline(in, linemover);
	getline(in, Ender);
	getline(in, linemover);

	string textFile;
	getline(in, textFile);
	if (textFile.length() > 1)
	{
		exists = true;
	}
	ifstream inT(textFile);
	if (exists == true && !inT)
	{
		cout << "Could not open file for reading.\n";
		return 0;
	}
	getline(in, linemover);
	if (linemover[15] == '1' || linemover[16] == '1')
	{
		CaseSpec = true;
	}
	getline(in, linemover);
	if (linemover[19] == '1' || linemover[20] == '1')
	{
		ignorer = true;
	}

	while (linemover != "TEXT:")
	{
		getline(in, linemover);
	}
						   //Get Text
	char c;
	string textStore = "";
	string holder = "";
	int wordIndex = 0;
	if (exists == true)
	{
		getline(inT, holder);
		while (!inT.fail())//done instead of getline() in order to include '\n'
		{
			textStore = textStore + holder + '\n';
			getline(inT, holder);
		}
		getline(in, linemover);
		while (linemover != Ender)//just in case newline is gone, get past to ender
		{
			getline(in, linemover);
		}
	}
	else
	{
		getline(in, holder);
		while (holder != Ender)
		{
			textStore = textStore + holder + '\n';
			getline(in, holder);
		}
	}

	holder = "";
	getline(in, linemover);
	//Find
	Ender = '\n' + Ender + '\n';//Ignores last end line in find
	vector<string> find;
	find.push_back("");

	while (wordIndex < Ender.length())
	{
		c = in.get();

		if (varBegin == c)
		{
			find[find.size() - 1] = find[find.size() - 1] + holder;
			holder = "";
			find.push_back("");
			c = in.get();//skip variable opener
			while (varEnd != c)
			{
				find[find.size() - 1] = find[find.size() - 1] + c;
				c = in.get();
				if (in.fail())
				{
					cout << "Error, no " << varEnd << " found to mark the end of the variable \n";
					return 0;
				}
			}
			find.push_back("");

		}
		else//ignores variables
		{
			holder = holder + c;
		}
		if (Ender[wordIndex] == c)//format potential
		{
			wordIndex++;
		}
		else //potential lost
		{
			wordIndex = 0;
			if (Ender[wordIndex] == c)//holder should keep the last letter, and not give it to input break out the substring?...
			{
				wordIndex++;
				holder = holder.substr(0, holder.length() - 1);  //gets rid of last char
				find[find.size() - 1] = find[find.size() - 1] + holder;
				holder = "";
				holder = holder + c;// equals to last char
			}
			else
			{
				find[find.size() - 1] = find[find.size() - 1] + holder; //holder comfirmed as unreplaced output
				holder = "";
			}

		}
	}
	if (find[0] == "")//incase find starts with variable
	{
		find[0] = "\n";
	}
	if (find[find.size()-1] == "")//incase find ends with variable
	{
		find[find.size() - 1] = "\n";
	}

	holder = "";
	wordIndex = 0;
	getline(in, linemover);

	//Replace
	vector<string> replace;
	replace.push_back("");

	while (wordIndex < Ender.length() - 1)//-1 to compensate for added end line to Ender
	{
		c = in.get();

		if (varBegin == c)
		{
			replace[replace.size() - 1] = replace[replace.size() - 1] + holder;
			holder = "";
			replace.push_back("");
			c = in.get();
			while (varEnd != c)
			{
				replace[replace.size() - 1] = replace[replace.size() - 1] + c;
				c = in.get();
				if (in.fail())
				{
					cout << "Error, no " << varEnd << " found to mark the end of the variable \n";
					return 0;
				}
			}
			replace.push_back("");

		}
		else
		{
			holder = holder + c;
		}
		if (Ender[wordIndex] == c)//format potential
		{
			wordIndex++;
		}
		else //potential lost
		{

			wordIndex = 0;
			if (Ender[wordIndex] == c)//holder should keep the last letter, and not give it to input break out the substring?...
			{
				wordIndex++;
				holder = holder.substr(0, holder.length() - 1);  //gets rid of last char
				replace[replace.size() - 1] = replace[replace.size() - 1] + holder;
				holder = "";
				holder = holder + c;// equals to last char
			}
			else
			{
				replace[replace.size() - 1] = replace[replace.size() - 1] + holder; //holder comfirmed as unreplaced output
				holder = "";
			}


		}
	}
	holder = "";
	wordIndex = 0;
	
	//find and replace
	vector<pair <string, string>> varStore;//stores variables
	pair <string, string> vectorGrower("", "");
	int varSize = 0;
	string varHolder = "";
	int i = 0;

	int findIndex = 0;
	int reset = 0;
	string output = "";
	string resetHolder = "";

	//add check for first find if stuck on fake variable
	for (int i = 0; i < textStore.length(); i++)
	{

		holder = holder + textStore[i];//fake variable check


		if (findIndex == 0)//matching format
		{
			if (CaseIgnore(textStore[i], CaseSpec) == CaseIgnore(find[findIndex][wordIndex], CaseSpec))//format potential
			{
				
				wordIndex++;
			}
			else //potential lost UPDATE
			{
				wordIndex = 0;
				findIndex = 0;//delete
				varSize = 0;//delete
				if (CaseIgnore(textStore[i], CaseSpec) == CaseIgnore(find[findIndex][wordIndex], CaseSpec))//holder should keep the last letter, and not give it to input break out the substring?...
				{
					wordIndex++;
					holder = holder.substr(0, holder.length() - 1);
					output = output + holder;
					holder = "";
					holder = holder + textStore[i];
				}
				else
				{
					output = output + holder; //holder comfirmed as unreplaced output
					holder = "";
					varHolder = "";//delete
				}
			}
		}
		else//HOLDER SHOULD BE CLEARED BEFORE ENTERING HERE
		{
			if (ignorer == false)
			{
				resetHolder = resetHolder + textStore[i];//in case need to reset

				if (CaseIgnore(textStore[i], CaseSpec) == CaseIgnore(find[0][reset], CaseSpec))//NOTE, THIS WON'T WORK IF FIND STARTS WITH VARIABLE, ADD option to overwrite this step if later first finds are supposed to be part of variable
				{
					reset++;
				}
				else //potential lost
				{
					reset = 0;
					if (CaseIgnore(textStore[i], CaseSpec) == CaseIgnore(find[0][reset], CaseSpec))
					{
						reset++;//reset = 1
					}
				}
				if (reset == find[0].length())//reset back to square one
				{
					wordIndex = 0;
					findIndex = 2;
					varSize = 0;
					output = output + resetHolder; //holder comfirmed as unreplaced output
					resetHolder = "";
					holder = "";
					varHolder = "";
					reset = 0;
					continue;//INCREMENTS i, just incase this letter is start of next format 
				}
			}

			if (CaseIgnore(textStore[i], CaseSpec) == CaseIgnore(find[findIndex][wordIndex], CaseSpec))//format potential
			{
				wordIndex++;
			}
			else//CHECK TYPES LIKE THESE FOR UPDATE
			{
				wordIndex = 0;
				varHolder = varHolder + holder;//holder confirmed as part of variable
				holder = "";
			}


		}
		if (wordIndex == find[findIndex].length())//end of current find text
		{
			if (findIndex > 0)//was recording a variable
			{
				
				if (varSize == varStore.size())
				{
					varStore.push_back(vectorGrower);
				}
				varStore[varSize].first = find[findIndex - 1];
				varStore[varSize].second = varHolder;
				varSize++;//nessesary because of reset
			}
			varHolder = "";//keep stored variable up to date
			holder = "";
			findIndex = findIndex + 2;
			wordIndex = 0;

			if (findIndex >= find.size())//FORMAT HAS BEEN FOUND
			{
				//ADD REPLACE VECTOR CONTENTS TO OUTPUT STRING
				for (int j = 0; j < replace.size(); j++)
				{
					if (j % 2 == 0)//not variable
					{
						output = output + replace[j];
					}
					else
					{
						for (int k = 0; k < varSize; k++)//ERROR: check for variables called the same thing in find
						{
							if (replace[j] == varStore[k].first)
							{
								output = output + varStore[k].second;
								break;//hope break keeps else if from happening, it should...
							}
							else if (k == varSize - 1)
							{
								output = output + replace[j];
							}
						}
					}
				}
				resetHolder = "";
				varSize = 0;
				findIndex = 0;
			}

		}
	}
	output = output + holder;

	out << output;
	cout << output;
	/*
	cout << endl << "THE LAB \n";
	cout << textStore << endl;
	cout << "Find Vector: " << find.size() << " \n";
	for (int i = 0; i < find.size(); i++)
	{
		cout << find[i] << endl;
	}
	cout << "Replace Vector: " << replace.size() << " \n";
	for (int i = 0; i < replace.size(); i++)
	{
		cout << replace[i] << endl;
	}
	cout << "DONE";
	*/
	return 0;
	
}