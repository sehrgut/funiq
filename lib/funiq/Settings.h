#ifndef _FUNIQ_SETTINGS_
#define _FUNIQ_SETTINGS_

struct Settings
{
	Settings() :
		maxEditDistance(3),
		caseInsensitive(false),
		showAllMatches(false),
		showTotals(false),
		onlyOutputDuplicates(false),
		onlyOutputUniques(false),
		ignoreNonAlphaNumeric(false),
		matchModeFast(false),
		totalsFieldWidth(7){} // count-field width 7 used by GNU uniq

	unsigned int maxEditDistance;
	bool caseInsensitive;
	bool showAllMatches;
	bool showTotals;
	bool onlyOutputDuplicates;
	bool onlyOutputUniques;
	bool ignoreNonAlphaNumeric;
	bool matchModeFast;
	int totalsFieldWidth;
};

#endif
