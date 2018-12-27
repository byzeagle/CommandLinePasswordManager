// PasswordManager.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Argument Parser
// Database create
// Set Password for database
// Update password for database
// Open database and read in information
// commandline argumetns
// shell commands

/*
						Not yet Encryption Added
*/

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/tokenizer.hpp>
#include <unordered_map>
#include <map>

using namespace std;
using namespace boost;

void RunShell();
string readLine();
void LookUp(const vector<string> & argsList, map<string, string> &);
vector<string> splitLine(const string & line);
void open(string filename, map<string, string> &);
void list(map<string, string> &);
void save(const string & filename, map<string, string> & entries);


int main()
{
	ios_base::sync_with_stdio(false);
	RunShell();
	return 0;
}

void RunShell()
{
	int x;
	std::string line = "";
	map<string, string> entries;

	do {
		cout << "> ";
		std::getline(std::cin, line);
		vector<string> inputs = splitLine(line);
		if (inputs[0] == "exit" || inputs[0] == "quit") break;
		LookUp(inputs, entries);
	} while (true);
}

// Splits arguments
vector<string> splitLine(const string & line)
{
	char_separator<char> sep(" \n\t");
	tokenizer<char_separator<char>> tok(line, sep);
	return vector<string>(tok.begin(), tok.end());
}

// put commands that are likely to be executed the most first.
void LookUp(const vector<string> & argsList, map<string, string> & entries)
{
	unordered_map<string, int> hashTable = 
	{
		{"open", 1},
		{"add",  2},
		{"show", 3},
		{"list", 3},
		{"save", 4}
	};

	switch (hashTable[argsList[0]])
	{
		case 1: open(argsList[1], entries); break;
		case 2: entries[argsList[1]] = argsList[2];; break;
		case 3: ::list(entries); break;
		case 4: save(argsList[1], entries); break;
		default: cout << "Not recognized command" << endl; break;
	}

}

void open(string filename, map<string, string> & entries)
{
	ifstream database;
	database.open(filename);
	if (database.is_open())
	{
		string userName;
		string password;

		cout << "Password file is fed into memory." << endl;
		while (database >> userName >> password)
		{
			entries[userName] = password;
		}

		database.close();
		return;
	}
	
	cout << "File could not be opened" << endl;
}

void show(const map<string, string> & entries)
{

}

void list(map<string, string> & entries)
{
	for (const auto & pair : entries)
	{
		cout << pair.first << '\t' << pair.second << endl;
	}
}

void save(const string & filename, map<string, string> & entries)
{
	ofstream database;
	database.open(filename);
	if (database.is_open())
	{
		cout << "Password file is saved" << endl;
		// Continue reading values from the file into the map
		for (const auto & pair : entries)
		{
			database << pair.first << " " << pair.second << '\n';
		}

		database.close();
		return;
	}
	cout << "File could not be opened" << endl;
}
