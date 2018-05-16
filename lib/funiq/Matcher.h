#ifndef _FUNIQ_MATCHER_
#define _FUNIQ_MATCHER_

#include <algorithm>
#include <string>
#include <iostream>
#include <iomanip>
#include <functional>
#include <cctype>
#include <map>

#include "Settings.h"

typedef std::vector<std::string> StringList;
typedef std::map< std::string, StringList* > StringListMap;

//todo: FastMatcher class
struct fast_state {
	std::string	prev;
	std::string	label;
	int			count;
};

class Matcher{
public:
	Matcher(Settings& settings);
	~Matcher();
	void add(std::string line);
	void show(std::ostream* line);
	void fast_add(std::string line, std::ostream* output);
	void fast_end(std::ostream* output);
private:
	struct fast_state fast;
	Settings& _settings;
	StringListMap* matchMap;
	void lowercase(std::string& s);
	void removeNonAlphaNumeric(std::string& s);
	void normalize(std::string& s);
	bool is_match(std::string a, std::string b);
	void fast_print_first(std::string line, std::ostream* output);
	void fast_print_match(std::string line, std::ostream* output);
	void fast_print_summary(std::ostream* output);
	unsigned int levenshteinDistance(const std::string& s1, const std::string& s2);
};

Matcher::Matcher(Settings& settings):_settings(settings) {
	matchMap = new StringListMap();
}

Matcher::~Matcher() {
	delete(matchMap);
}

bool Matcher::is_match(std::string a, std::string b) {
	return (levenshteinDistance(a, b) <= _settings.maxEditDistance);
}

void Matcher::fast_print_first(std::string line, std::ostream* output) {
	if (_settings.showAllMatches)
		*output << line << std::endl;
}

void Matcher::fast_print_match(std::string line, std::ostream* output) {
	if (_settings.showAllMatches)
		*output << "\t" << line << std::endl;
}

void Matcher::fast_print_summary(std::ostream* output) {
	if (_settings.showAllMatches) {
		if(_settings.showTotals)
			*output << "Count: " << fast.count;
	} else {
		if(_settings.showTotals)
			*output <<
				std::setw(_settings.totalsFieldWidth) <<
				fast.count << " ";
		*output << fast.label;
	}
	*output << std::endl;
}

void Matcher::fast_add(std::string line, std::ostream* output) {
	std::string cur = line;
	normalize(cur);

	if(is_match(cur, fast.prev)) {
		fast_print_match(line, output);
		fast.count++;
	} else {
		if (! fast.prev.empty()) {
			fast_print_summary(output);
		}
		fast_print_first(line, output);
		fast.count = 1;
		fast.label = line;

		/*
		Always match against the first member of a newly-discovered cluster. If matching
		against the most recent member of a cluster, each subsequent line could up to
		double the potential distance of a newly-added cluster member from the founding
		member, growing clusters much larger than intended.
		*/
		fast.prev = cur;
	}
	
}

void Matcher::fast_end(std::ostream* output) {
	if (! fast.prev.empty()) {
		fast_print_summary(output);
	}
}


void Matcher::add(std::string line) {
	bool matchFound = false;
	normalize(line);
	for(auto matchPair : *matchMap) {
		std::string key = matchPair.first;
		normalize(key);
		StringList* matchList = matchPair.second;	
				
		if(is_match(line, key)) {
			matchFound = true;
			matchList->push_back(line);
			continue;
		}
	}

	if(!matchFound) {
		StringList* matchList = new StringList(0);
		matchList->push_back(line);
		(*matchMap)[line] = matchList;
	}
}

// todo: count max width necessary
void Matcher::show(std::ostream* output) {
	for(auto matchPair : *matchMap) {
		StringList v = *matchPair.second;
		bool first = true;
		if ( !(_settings.onlyOutputDuplicates || _settings.onlyOutputUniques) ||
				(_settings.onlyOutputDuplicates && v.size() > 1) ||
				(_settings.onlyOutputUniques && v.size() == 1)) {
			for(std::string matchItem : v) {
				if(first || _settings.showAllMatches) {
					if(first && _settings.showTotals)
						*output << 
							std::setw(_settings.totalsFieldWidth) <<
							v.size() << " "; // space for compatibility with GNU uniq
					if(!first) *output << "\t";
					*output << matchItem;
					first = false;
				}
				*output << std::endl;
			}
		}
	}
}

void Matcher::lowercase(std::string& s) {
	transform(s.begin(), s.end(), s.begin(), ::tolower);
}

void Matcher::removeNonAlphaNumeric(std::string& s) {
	s.erase(std::remove_if(s.begin(), s.end(), [](const char& c){
                return !std::isalnum(c);
              }), s.end());
}

void Matcher::normalize(std::string& s) {
	if(_settings.caseInsensitive) lowercase(s);
	if(_settings.ignoreNonAlphaNumeric) removeNonAlphaNumeric(s);
}

unsigned int Matcher::levenshteinDistance(const std::string& s1, const std::string& s2) {
	
	unsigned int len1 = s1.size();
	unsigned int len2 = s2.size();
	std::vector<unsigned int> col(len2+1);
	std::vector<unsigned int> prevCol(len2+1);

	for (unsigned int i = 0; i < prevCol.size(); i++) {
		prevCol[i] = i;
	}
	for (unsigned int i = 0; i < len1; i++) {
		col[0] = i+1;
		for (unsigned int j = 0; j < len2; j++) {
			col[j+1] = std::min(
				std::min( 1 + col[j], 1 + prevCol[1 + j]),
				prevCol[j] + (s1[i]==s2[j] ? 0 : 1)
			);
		}
		col.swap(prevCol);
	}
	return prevCol[len2];
}

#endif