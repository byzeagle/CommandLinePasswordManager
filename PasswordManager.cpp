#include "pch.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <boost/tokenizer.hpp>

#include "modes.h"
#include "aes.h"
#include "filters.h"

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
void add(const vector<string> & argsList, map<string, map<string, string>> & entries);

void encode(string & data, string Skey, string & CipherText)
{
	byte key[AES::DEFAULT_KEYLENGTH];
	memset(key, 0, AES::DEFAULT_KEYLENGTH);

	byte iv[AES::DEFAULT_KEYLENGTH];
	memset(iv, 0, AES::DEFAULT_KEYLENGTH);

	int i{ 0 };

	string theKey(Skey);
	theKey.append("turken007326974!@#$%%");

	while (i != 16)
	{
		key[i] = theKey[i];
		iv[i] = theKey[i];
		++i;
	}

	//Encryptor
	ECB_Mode <AES>::Encryption Encryptor(key, sizeof(key));
	//ECB_Mode <AES>::Encryption Encryptor(key, sizeof(key), iv);

	StringSource(data, true,
		new StreamTransformationFilter(Encryptor,
			new StringSink(CipherText)
		)
	);
}

void decode(string CT, string Skey, string & RText)
{
	byte key[AES::DEFAULT_KEYLENGTH];
	memset(key, 0, AES::DEFAULT_KEYLENGTH);

	byte iv[AES::DEFAULT_KEYLENGTH];
	memset(iv, 0, AES::DEFAULT_KEYLENGTH);

	int i{ 0 };

	string theKey(Skey);
	theKey.append("turken007326974!@#$%%");

	while (i != 16)
	{
		key[i] = theKey[i];
		iv[i] = theKey[i];
		++i;
	}

	//Decrpto
	ECB_Mode <AES>::Decryption Decryptor(key, sizeof(key));
	//ECB_Mode <AES>::Decryption Decryptor(key, sizeof(key), iv);

	StringSource(CT, true,
		new StreamTransformationFilter(Decryptor,
			new StringSink(RText)
		)
	);
}

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

void add(const vector<string> & argsList, map<string, map<string, string>> & entries)
{
	if (argsList.size() != 4)
	{
		cout << "Add command has to take exactly 3 elements" << endl;
		return;
	}
	string website = "", username = "", password = "";
	entries[argsList[1]][argsList[2]] = argsList[3];
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
	database.open(argsList[1], std::ios::binary);
	if (database.is_open())
	{
		// Read the file at once
		std::string content((std::istreambuf_iterator<char>(database)), (std::istreambuf_iterator<char>()));
		
		string key = "";
		string decryptedText = "";
		cout << "Please enter a key : ";
		getline(cin, key);

		decode(content, key, decryptedText);
		
		stringstream inputStream (decryptedText);
		string website = "", username = "", passwd = "";

		while (inputStream >> username >> website >> passwd)
			entries[username][website] = passwd;

		database.close();
		return;
	}
	cout << "File could not be opened" << endl;
}

// Encrypt the file here.
void save(const vector<string> & argsList, map<string, map<string, string>> & entries)
{
	if (argsList.size() != 2)
	{
		cout << "Save command takes only two arguments" << endl;
		return;
	}

	ofstream database;
	database.open(argsList[1], std::ios::binary);

	if (database.is_open())
	{
		stringstream res;

		// Continue reading values from the file into the map
		for (const auto & outerPair : entries)
			for (const auto & innerPair : outerPair.second)
				res << outerPair.first << " " << innerPair.first << " " << innerPair.second << '\n';

		string finalString = res.str();
		string encryptedString = "";
		string key = "";

		cout << "Please enter a key : ";
		getline(cin, key);

		// First Encode the string
		encode(finalString, key, encryptedString);
		
		// Then Write encoded string.
		database << encryptedString;

		// Close the database
		database.close();

		cout << "Password file is saved" << endl;

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
