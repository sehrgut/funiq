#include <algorithm>
#include <functional>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "tclap/CmdLine.h"
#include "funiq/Settings.h"
#include "funiq/Matcher.h"

void parseCommandLine(int argc, char** argv, std::string& filename, Settings& settings) {
	
		TCLAP::CmdLine cmd("funiq - Fuzzy Unique Filtering", ' ', "0.2");
	
	TCLAP::UnlabeledValueArg<std::string> filenameArg (
		"filename",
		"File to read. If omitted will read from stdin.",
		false, "", "filename");
	TCLAP::ValueArg<unsigned int> distanceArg(
		"d","distance",
		"Maximum edit distance between two strings to be considered a match.",
		false, 3, "integer");
	TCLAP::SwitchArg caseSwitch(
		"i","case-insensitive",
		"When active, case differences do not contribute to edit distance.");
	TCLAP::SwitchArg showAllSwitch(
		"a","show-all",
		"Will show all found duplicates.");
	TCLAP::SwitchArg showTotalsSwitch(
		"c","show-counts",
		"Precede each output line with the count of the number of times the line occurredin the input, followed by a single space.");
	TCLAP::SwitchArg ignoreNonAlphaNumericSwitch(
		"I","ignore-non-alpha-numeric",
		"When active, non-alphanumeric characters do not contribute to edit distance.");
	TCLAP::SwitchArg matchModeFastSwitch(
		"F","fast",
		"Operate in stream mode like uniq, only comparing against recent lines.");
	TCLAP::SwitchArg onlyOutputDuplicatesSwitch(
		"d","only-duplicates",
		"Only output lines that are repeated in the input.");
	TCLAP::SwitchArg onlyOutputUniquesSwitch(
		"u","only-uniques",
		"Only output lines that are not repeated in the input.");

	cmd.add(filenameArg);
	cmd.add(distanceArg);
	cmd.add(caseSwitch);
	cmd.add(showAllSwitch);
	cmd.add(showTotalsSwitch);
	cmd.add(ignoreNonAlphaNumericSwitch);
	cmd.add(matchModeFastSwitch);
	cmd.add(onlyOutputDuplicatesSwitch);
	cmd.add(onlyOutputUniquesSwitch);	
	cmd.parse(argc, argv);

	settings.maxEditDistance = distanceArg.getValue();
	settings.caseInsensitive = caseSwitch.getValue();
	settings.showAllMatches	= showAllSwitch.getValue();
	settings.showTotals = showTotalsSwitch.getValue();
	settings.onlyOutputDuplicates = onlyOutputDuplicatesSwitch.getValue();
	settings.onlyOutputUniques = onlyOutputUniquesSwitch.getValue();
	settings.ignoreNonAlphaNumeric = ignoreNonAlphaNumericSwitch.getValue();
	settings.matchModeFast = matchModeFastSwitch.getValue();
	filename = filenameArg.getValue();	
}

std::istream* getInput(const std::string& filename) {
	std::istream* inputStream;
	if(filename == "") {
		inputStream = &std::cin;
	} else {
		inputStream = new std::ifstream(filename.c_str());
	}
	return inputStream;
}

int main(int argc, char* argv[]) {

	try {
		std::string filename;
		Settings settings;
		parseCommandLine(argc, argv, filename, settings);
		
		Matcher matcher(settings);
		std::istream* inputStream = getInput(filename);		
		for (std::string line; getline(*inputStream, line); ) {
			settings.matchModeFast ? matcher.fast_add(line, &std::cout) : matcher.add(line);
		}

		settings.matchModeFast ? matcher.fast_end(&std::cout) : matcher.show(&std::cout);

	} catch (TCLAP::ArgException &e) {
		std::cerr << "An error occurred: ";
		std::cerr << e.error() << " for arg " << e.argId() << std::endl;
	}
}