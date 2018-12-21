// PasswordManager.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Argument Parser
// Database create
// Set Password for database
// Update password for database
// Open database and read in information
// commandline argumetns
// shell commands

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;

void RunShell();
string readLine();
void LookUp(const vector<string> & argsList);
vector<string> splitLine(const string & line);

int main()
{
	ios_base::sync_with_stdio(false);
	RunShell();
	return 0;
}

void RunShell()
{
	int x;
	std::string line;
	bool status = true;

	do {
		cout << "> ";
		line = readLine();
		vector<string> inputs = splitLine(line);
		LookUp(inputs);

	} while (status);
}

// Reads given input
// open <filename>
// list - when a file is already opened, list user names
// show - when username is given show password
// add - add user name and password
// update - only a password of an existing entry can be updated
// delete - delete an entry, ask before continung
// exit - leave the shell
string readLine()
{
	string line;
	std::getline(std::cin, line);
	return line;
}

// Splits arguments
vector<string> splitLine(const string & line)
{
	vector<string> list;
	tokenizer<> tok(line);
	for (auto iter = tok.begin(); iter != tok.end(); ++iter)
	{
		//cout << *iter << "\n----------------\n";
		list.push_back(*iter);
	}
	return list;
}

void LookUp(const vector<string> & argsList)
{
	const string & command = argsList[0];
	if (command == "open")
	{
		cout << "file path is : " << argsList[1] << endl;
	}
	else if (command == "save")
	{

	}
	else if (command == "exit")
	{
		exit(EXIT_SUCCESS);
	}
	else
	{
		cout << "Not recognized Command" << endl;
	}
}