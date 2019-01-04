#include "pch.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <boost/tokenizer.hpp>
#include <unordered_map>
#include <map>

using namespace std;
using namespace boost;
using namespace CryptoPP;

void RunShell();
void LookUp(const vector<string> &, map<string, map<string, string>> &);
vector<string> splitLine(const string &);
void open(const vector<string> &, map<string, map<string, string>> &);
void list(map<string, map<string, string>> &);
void save(const vector<string> &, map<string, map<string, string>> &);
void get(const vector<string> & argsList, map<string, map<string, string>> & entries);

string openFileName = "";

int main()
{
	ios_base::sync_with_stdio(false);
	RunShell();
	return 0;
}

void RunShell()
{
	std::string line = "";
	map<string, map<string, string>> entries;

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

void LookUp(const vector<string> & argsList, map<string, map<string, string>> & entries)
{
	// Hash Strings
	unordered_map<string, int> hashTable =
	{
		{"open", 1},
		{"add",  2},
		{"show", 3},
		{"list", 3},
		{"l",	 3},
		{"ls",   3},
		{"save", 4},
		{"close", 5},
		{"clear", 5},
		{"get",   6}
	};

	switch (hashTable[argsList[0]])
	{
		case 1: open(argsList, entries); break;
		case 2: entries[argsList[1]][argsList[2]] = argsList[3]; break;
		case 3: ::list(entries); break;
		case 4: save(argsList, entries); break;
		case 5: entries.clear(); openFileName = ""; break; // Clear password in memory
		case 6: get(argsList, entries); break;
		default: cout << "Not recognized command" << endl;
	}
}

void get(const vector<string> & argsList, map<string, map<string, string>> & entries)
{
	if (argsList.size() != 2)
	{
		cout << "Get command takes only two arguments" << endl;
		return;
	}

	for (const auto & pair : entries[argsList[1]])
	{
		cout << argsList[1] << "\t\t" << pair.first << "\t\t" << pair.second << '|' << endl;
	}
}

// Decrypt the file here.
void open(const vector<string> & argsList, map<string, map<string, string>> & entries)
{
	if (argsList.size() != 2)
	{
		cout << "Open command takes only three arguments" << endl;
		return;
	}

	openFileName = argsList[1];

	ifstream database;
	database.open(argsList[1]);
	if (database.is_open())
	{
		string website;
		string userName;
		string password;

		cout << "Password file \"" << openFileName << "\" is fed into memory." << endl;
		while (database >> website >> userName >> password)
			entries[website][userName] = password;
		
		database.close();
		return;
	}
	cout << "File could not be opened" << endl;
}

// Encrypt the file here.
void save(const vector<string> & argsList, map<string, map<string, string>> & entries)
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
		for (const auto & outerPair : entries)
		{
			for (const auto & innerPair : outerPair.second)
			{
				database << outerPair.first << " " << innerPair.first << " " << innerPair.second << '\n';
			}
		}

		database.close();
		return;
	}
	cout << "File could not be opened" << endl;
}

void list(map<string, map<string, string>> & entries)
{
	cout << "Open file is : " << openFileName << endl;
	cout << "-----------------------------------------------------------------------------" << endl;
	for (const auto & outerPair : entries)
	{
		for (const auto & innerPair : outerPair.second)
		{
			cout << outerPair.first << "\t\t" << innerPair.first << "\t\t" << innerPair.second << '|' << endl;
			cout << "-----------------------------------------------------------------------------" << endl;
		}
	}
}
