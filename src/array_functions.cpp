/*
 * functionstocomplete.cpp
 *
 *  Created on: Sep 10, 2017
 *      Author: keith
 */

//============================================================================
//	TODO add necessary includes here
//============================================================================
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>

#include "array_functions.h"
#include "constants.h"
#include "utilities.h"
using namespace constants;
using namespace std;
//============================================================================
//	stuff you will need
//============================================================================
//TODO define a structure to track words and number of times they occur
struct entry {
	string word;
	int number_of_occurences;
} emptyEntry;
//TODO add a global array of entry structs (global to this file)
entry* myEntryArray = new entry[1];
//TODO add variable to keep track of next available slot in array
int available_slot = 0;
//TODO define all functions in header file

//TODO look in utilities.h for useful functions, particularly strip_unwanted_chars!

//zero out array that tracks words and their occurrences
void clearArray() {
	delete[] myEntryArray;
	myEntryArray = new entry[1];
	available_slot = 0;
}

//how many unique words are in array
int getArraySize() {
	return available_slot;
}

//get data at a particular location
std::string getArrayWordAt(int i) {
	return myEntryArray->word;
}
int getArrayWord_NumbOccur_At(int i) {
	return myEntryArray->number_of_occurences;
}

/*loop through whole file, one line at a time
 * call processLine on each line
 * returns false: myfstream is not open
 *         true: otherwise*/
bool processFile(std::fstream &myfstream) {
	if (!myfstream.is_open()) {
		return false;
	}
	string line;
	while(!myfstream.eof()) {
		getline(myfstream, line);
		processLine(line);
	}

	return true;
}

/*take 1 line and extract all the tokens from it
feed each token to processToken for recording*/
void processLine(std::string &myString) {
	stringstream ss(myString);
	string tempToken;
	while(getline(ss, tempToken, CHAR_TO_SEARCH_FOR)) {
		processToken(tempToken);
	}

}

/*Keep track of how many times each token seen*/
void processToken(std::string &token) {
	strip_unwanted_chars(token);
	if (token.length() == 0) {
		return;
	}

	entry newToken;
	newToken.number_of_occurences = 1;
	newToken.word = token;

	if (available_slot == 0) {
		myEntryArray[0] = newToken;
		available_slot += 1;
	}
	else {
		string tToken = token;
		toUpper(tToken);
		for (int i = 0; i < available_slot+1; i++) {
			string temp = getArrayWordAt(i);
			toUpper(temp);
			if (token == myEntryArray[i].word || tToken == temp) {
				myEntryArray[i].number_of_occurences += 1;
				return;
			}
		}

		entry* temp  = new entry[available_slot+1];
		for (int i = 0; i < getArraySize(); i++) {
			temp[i] = myEntryArray[i];
		}

		myEntryArray = temp;
		myEntryArray[available_slot-1] = newToken;
		available_slot+= 1;
		}

}

/*if you are debugging the file must be in the project parent directory
  in this case Project2 with the .project and .cProject files*/
bool openFile(std::fstream& myfile, const std::string& myFileName,
		std::ios_base::openmode mode /*= std::ios_base::in*/) {
	myfile.open(myFileName);
	return myfile.is_open();
}

/*iff myfile is open then close it*/
void closeFile(std::fstream& myfile) {
	myfile.close();
}

/* serializes all content in myEntryArray to file outputfilename
 * returns  FAIL_FILE_DID_NOT_OPEN if cannot open outputfilename
 * 			FAIL_NO_ARRAY_DATA if there are 0 entries in myEntryArray
 * 			SUCCESS if all data is written and outputfilename closes OK
 * */
int writeArraytoFile(const std::string &outputfilename) {
	if (getArraySize() == 0) {
		return FAIL_NO_ARRAY_DATA;
	}
	std::ofstream outputfile;
	outputfile.open(outputfilename);

	if (!outputfile.is_open()) {
		return FAIL_FILE_DID_NOT_OPEN;
	}
	for (int i = 0; i > getArraySize(); i++) {
		outputfile << myEntryArray[i].word <<
				myEntryArray[i].number_of_occurences << " ";
	}
	return SUCCESS;

}

/*
 * Sort myEntryArray based on so enum value.
 * The presence of the enum implies a switch statement
 */
void sortArray(constants::sortOrder so) {
	entry temp = myEntryArray[0];
	switch(so) {
	case NONE:
		break;
	case ASCENDING:
		for (int i = 0; i < getArraySize()-1; i++) {
			for (int j = i+1; j < getArraySize(); j++) {
				if (getArrayWordAt(i) > getArrayWordAt(j)) {
					temp = myEntryArray[i];
					myEntryArray[i] = myEntryArray[j];
					myEntryArray[j] = temp;
				}
			}
		}
		break;
	case DESCENDING:
		for (int i = 0; i < getArraySize()-1; i++) {
					for (int j = i+1; j < getArraySize(); j++) {
						if (getArrayWordAt(i) < getArrayWordAt(i)) {
							temp = myEntryArray[i];
							myEntryArray[i] = myEntryArray[j];
							myEntryArray[j] = temp;
						}
					}
				}
		break;
	case NUMBER_OCCURRENCES:
		for (int i = 0; i < getArraySize()-1; i++) {
					for (int j = i+1; j < getArraySize(); j++) {
						if (getArrayWord_NumbOccur_At(i) >
							getArrayWord_NumbOccur_At(i)) {
							temp = myEntryArray[i];
							myEntryArray[i] = myEntryArray[j];
							myEntryArray[j] = temp;
						}
					}
				}
		break;
	}
}
