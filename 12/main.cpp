#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <chrono>

//compile with c++17 standard

using std::fstream; using std::cout; using std::endl;

auto prepareData(const char* path)
{
	std::string initState;
	std::unordered_map<std::string, char> rules;
	fstream input(path, std::ios::in); if (!input.good()) return std::make_pair(initState, rules);

	std::getline(input, initState);
	initState = initState.substr(15, std::string::npos);
	std::string temp;
	std::getline(input, temp);

	while (!input.eof()) {
		std::getline(input, temp);
		std::string pattern = temp.substr(0,temp.find(" => "));
		char state = temp.substr(temp.find(" => "), std::string::npos)[4];
		rules[pattern] = state;
	}
	input.close();

	return std::make_pair(initState, rules);
}

void nextGeneration(std::string &state, const std::unordered_map<std::string, char>& rules,int& start)
{
	const std::size_t patternSize = 5;
	
	if (state[0] == '#' || state[1] == '#' || state[2] == '#') {
		state = "..." + state;
		start -= 3;
	}
	
	if (state[state.length() - 1] == '#' || state[state.length() - 2] == '#' || state[state.length() - 3] == '#')
		state += "...";

	std::string newState = state;
	for (auto& ch : newState) ch = '.';

	for (int i = 0 ; i < state.size() - 4; ++i) {
		for (auto& rule : rules) {
			int difference = 5;
			for (int j = 0; j < 5; ++j) {
				if (rule.first[j] == state[i+j]) --difference;
			}
			if (!difference) {
				newState[i + 2] = rule.second;
				break;
			}
		}
	}

	state = newState;
}

int plantsSum(std::string &generation, int& start)
{
	int sum = 0;
	for (int i = 0; i < generation.size(); ++i) {
		if(generation[i] == '#') sum += i + start;
	}
	return sum;
}

int taskA(std::string state, const std::unordered_map<std::string, char>& rules)
{
	int sumOfPlants;
	int start = 0;
	for (int i = 0; i < 20; ++i) {
		 nextGeneration(state, rules,start);
	}
	return plantsSum(state,start);
}

bool isStabilized(const std::string& generation, const std::string& nextGeneration)
{
	for (int i = 0; i < generation.size(); ++i) {
		if (i + 1 < nextGeneration.size()) {
			if (generation[i] != nextGeneration[i + 1]) {
				return false;
			}
		}
	}
	return true;
}


long long taskB(std::string state, const std::unordered_map<std::string, char>& rules)
{
	int sumOfPlants;
	int start = 0;
	int prevStart;
	int generations = 0;
	std::string prevGeneration;
	do {
		++generations;
		prevGeneration = state;
		prevStart = start;
		nextGeneration(state, rules, start);
	} while (!isStabilized(prevGeneration, state));

	long long beforeMovement = plantsSum(prevGeneration, prevStart);
	long long plantsNum = 0; for (auto& ch : prevGeneration) if (ch == '#') ++plantsNum;
	long long answer = beforeMovement + (50000000000 - (generations-1)) * plantsNum;

	return answer;
}


int main()
{
	auto t1 = std::chrono::steady_clock::now();
	auto[initState, rules] = prepareData("input_org.txt");
	cout << taskA(initState,rules) << endl;
	cout << taskB(initState, rules) << endl;
	auto t2 = std::chrono::steady_clock::now();
	cout << "Solved in: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << endl;



	system("pause");
}