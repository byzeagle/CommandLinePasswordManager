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
#include <unordered_map>
#include <map>

#include "modes.h"
#include "aes.h"
#include "filters.h"

using namespace std;
using namespace boost;
using namespace CryptoPP;

void RunShell();
void LookUp(const vector<string> & argsList, map<string, string> &);
vector<string> splitLine(const string & line);
void open(const vector<string> & argsList, map<string, string> & entries);
void list(map<string, string> &);
void save(const vector<string> & argsList, map<string, string> & entries);

int main()
{
	ios_base::sync_with_stdio(false);
	RunShell();
	return 0;
}

void RunShell()
{
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

vector<string> splitLine(const string & line)
{
	char_separator<char> sep(" \n\t");
	tokenizer<char_separator<char>> tok(line, sep);
	return vector<string>(tok.begin(), tok.end());
}

void LookUp(const vector<string> & argsList, map<string, string> & entries)
{
	// Hash Strings
	unordered_map<string, int> hashTable =
	{
		{"open", 1},
		{"add",  2},
		{"show", 3},
		{"list", 3},
		{"save", 4},
		{"close", 5},
		{"clear", 5}
	};

	switch (hashTable[argsList[0]])
	{
		case 1: open(argsList, entries); break;
		case 2: entries[argsList[1]] = argsList[2]; break;
		case 3: ::list(entries); break;
		case 4: save(argsList, entries); break;
		case 5: entries.clear(); break; // Clear password in memory
		default: cout << "Not recognized command" << endl;
	}
}

// Decrypt the file here.
void open(const vector<string> & argsList, map<string, string> & entries)
{
	if (argsList.size() != 2)
	{
		cout << "Open command takes only three arguments" << endl;
		return;
	}

	string password = "";
	cout << "Master password: ";
	std::getline(cin, password);
	cout << "Your password is : " << password << endl;

	ifstream database;
	database.open(argsList[1]);
	if (database.is_open())
	{
		string userName;
		string password;

		cout << "Password file is fed into memory." << endl;
		while (database >> userName >> password)
			entries[userName] = password;
		
		database.close();
		return;
	}
	cout << "File could not be opened" << endl;
}

// Encrypt the file here.
void save(const vector<string> & argsList, map<string, string> & entries)
{
	if (entries.size() != 2)
	{
		cout << "Save command takes only two arguments" << endl;
		return;
	}

	ofstream database;
	database.open(argsList[1]);
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

void list(map<string, string> & entries)
{
	for (const auto & pair : entries)
	{
		cout << pair.first << '\t' << pair.second << endl;
	}
}
