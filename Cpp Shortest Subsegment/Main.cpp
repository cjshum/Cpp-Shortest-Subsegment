// ************ MUST INCLUDE FOR MEMORY DETECTION ************ //
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
// ************ MUST INCLUDE FOR MEMORY DETECTION ************ //

#include <algorithm>
#include <iostream>
#include <string>
#include <regex>
#include <list>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

std::vector<std::string> split(const std::string &text, char sep) {
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos) {
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(text.substr(start));
	return tokens;
}

template <typename ContentType, typename DictionaryType>
struct Data {
	ContentType content;
	DictionaryType dictionary;
};

class StandardInputToData {
public:
	vector<string> originalWords;
	vector<string> lowerCaseWords;
	map<string, int> wordsToMatch;

	StandardInputToData() {
		string sentence;
		getline(cin, sentence);
		regex filter("[^a-zA-Z\\s]");
		sentence = regex_replace(sentence, filter, "");
		originalWords = split(sentence, ' ');

		lowerCaseWords = vector<string>(originalWords);
		for (size_t i = 0; i<lowerCaseWords.size(); i++)
			transform(lowerCaseWords[i].begin(), lowerCaseWords[i].end(), lowerCaseWords[i].begin(), ::tolower);

		int numItemsToMatch;
		cin >> numItemsToMatch;

		wordsToMatch = map<string, int>();
		for (int i = 0; i<numItemsToMatch; i++) {
			string currWord;
			cin >> currWord;

			transform(currWord.begin(), currWord.end(), currWord.begin(), ::tolower);
			wordsToMatch.emplace(currWord, 0);
		}
	}
};

struct SubsegmentRange {
	int startIndex;
	int endIndex;
};

class ShortestSubsegment {
public:
	const StandardInputToData &data;
	SubsegmentRange shortestSubsegment;

	ShortestSubsegment(const StandardInputToData &data) : data(data) {
		shortestSubsegment.startIndex = 0;
		shortestSubsegment.endIndex   = data.lowerCaseWords.size();
	}


};

int main() {
	// ************ MUST INCLUDE FOR MEMORY DETECTION ************ //
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	// ************ MUST INCLUDE FOR MEMORY DETECTION ************ //



	StandardInputToData data;
	auto itemsToMatch   = data.wordsToMatch;														
	auto lowerCaseWords = data.lowerCaseWords;
	auto originalWords  = data.originalWords;

	// -------------- Find the shortest subsegent
	string startItem;
	auto numItemsToMatch   = itemsToMatch.size();
	auto bestStart         = lowerCaseWords.begin();
	auto bestEnd           = lowerCaseWords.end();
	auto bestLength        = distance(bestStart, bestEnd) + 1;
	auto currStartIterator = bestStart;
	auto numDiffItemsEnc   = (unsigned)0;
	auto currIterator      = currStartIterator;

	for (; currIterator != lowerCaseWords.end(); currIterator++) {
		if (itemsToMatch.count(*currIterator)) {
			itemsToMatch[*currIterator] = 1;
			startItem                   = *currIterator;
			bestStart                   = currIterator;
			bestLength                  = distance(currIterator, bestEnd) + 1;
			currStartIterator           = bestStart;
			numDiffItemsEnc             = 1;
			currIterator++;
			break;
		}
	}

	for (; currIterator != lowerCaseWords.end(); currIterator++) {
		bool matchedItem      = itemsToMatch.count(*currIterator) != 0;
		bool firstEncounter   = (matchedItem) ? 0 == itemsToMatch[*currIterator] : false;
		bool foundBetterStart = 0 == currIterator->compare(startItem);

		if (matchedItem && firstEncounter) {
			itemsToMatch[*currIterator] = 1;
			numDiffItemsEnc += 1;
		}

		else if (matchedItem && !firstEncounter && !foundBetterStart)
			itemsToMatch[*currIterator] += 1;

		else if (matchedItem && !firstEncounter && foundBetterStart) {
			auto newIterator = currStartIterator;
			newIterator++;

			for (; newIterator != currIterator; newIterator++) {
				currStartIterator = newIterator;
				startItem = *newIterator;
				int numEnc = itemsToMatch.count(*newIterator) ? itemsToMatch[*newIterator] : 0;

				if (numEnc > 1)
					itemsToMatch[*newIterator] -= 1;

				else if (numEnc == 1)
					break;
			}
		}

		bool foundAllItems = numDiffItemsEnc == numItemsToMatch;
		if (matchedItem && foundAllItems) {
			int newLength = distance(currStartIterator, currIterator) + 1;
			if (newLength < bestLength) {
				bestLength = newLength;
				bestStart = currStartIterator;
				bestEnd = currIterator;
			}

			if (bestLength == numItemsToMatch) break;
		}
	}

	// prints the shortest subsegment
	if (numDiffItemsEnc < numItemsToMatch) {
		cout << "NO SUBSEGMENT FOUND";
	}
	else {
		auto printStart = originalWords.begin();
		advance(printStart, distance(lowerCaseWords.begin(), bestStart));
		auto printEnd = originalWords.begin();
		advance(printEnd, distance(lowerCaseWords.begin(), bestEnd));

		stringstream ss;
		for (; printStart != printEnd; printStart++)
			ss << *printStart << " ";

		ss << *printStart;
		cout << ss.str() << endl;
	}

	return 0;
}