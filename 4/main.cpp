#include <iostream>
#include <fstream>
#include <bitset>
#include <unordered_map>
#include <set>
#include <regex>
#include <array>
#include <chrono>

//sleep bit = 1
//awaken bit = 0


using std::fstream; using std::cout; using std::endl;


struct Guard {
	Guard(int id):m_id(id){}
	int m_id;
	std::unordered_map<std::string, std::bitset<60>> m_days;
	bool operator<(Guard& g) const { return m_id < g.m_id; }
	bool operator==(const Guard& g) const { return g.m_id == m_id; }
};

bool operator<(const Guard& g1, const Guard& g2) { return g1.m_id < g2.m_id; }

auto prepareData(const char *path)
{
	std::set<std::string> data;
	fstream input(path, std::ios::in); if (!input.good()) return data;
	while (!input.eof()) {
		std::string temp;
		std::getline(input, temp);
		data.insert(std::move(temp));
	}
	input.close();

	return data;
}

auto prepareGuards(const std::set<std::string>& data)
{
	std::set<Guard> guards;

	std::regex datePattern("[0-9]+-[0-1][0-9]-[0-3][0-9]");
	std::regex timePattern("[0-9]{2}:[0-9]{2}");
	std::regex guardIdPattern("Guard #[0-9]+");
	std::regex wakePattern("wakes up");
	std::regex sleepPattern("falls asleep");



	Guard keyGuard(0);
	for (const auto& line : data) {
		
		std::smatch dateR;
		std::smatch timeR;
		std::regex_search(line, dateR, datePattern);
		std::regex_search(line, timeR, timePattern);
		const std::string date = dateR.str();
		const std::string time = timeR.str();

		std::smatch toDo;
		if (std::regex_search(line, toDo, guardIdPattern)) {

			keyGuard.m_id = std::stoi(toDo.str().substr(7,std::string::npos));
		}
		else {
			bool toSet = false;
			auto foundGuard = guards.find(keyGuard);
			if(std::regex_search(line, toDo, sleepPattern)) toSet = true;
			if (foundGuard == guards.end()) {
				Guard toInsert(keyGuard.m_id);
				auto& bitset = toInsert.m_days[date];
				int start = std::stoi(time.substr(3));
				for (start; start < 60; ++start) bitset[start] = toSet;
				guards.insert(std::move(toInsert));
			}
			else {
				Guard& g = const_cast<Guard&>(*foundGuard); // ale kwas
				auto& bitset = g.m_days[date];
				int start = std::stoi(time.substr(3));
				for (start; start < 60; ++start) bitset[start] = toSet;
			}
		}
	}

	return guards;
}


int taskA(const std::set<Guard>& guards)
{
	int id;
	int maxSlept = 0;
	for (const auto& guard : guards) {
		int Slept = 0;
		for (auto& day : guard.m_days) Slept += day.second.count();

		if (Slept > maxSlept) {
			maxSlept = Slept;
			id = guard.m_id;
		}
		
	}

	Guard& laziestOne = const_cast<Guard&>(*(guards.find(Guard(id))));
	std::array<int, 60> minutes; minutes.fill(0);
	for (auto& sets : laziestOne.m_days) {
		for (int i = 0; i < 60; ++i) {
			if (sets.second[i]) ++minutes[i];
		}
	}

	auto laziestMinute = std::distance(minutes.begin(), std::max_element(minutes.begin(), minutes.end()));
	return id * laziestMinute;

}

int taskB(std::set<Guard>& guards)
{
	int ID;
	int maxMinutes = 0;
	int minute;

	for (const auto& guard : guards) {
		std::array<int, 60> slept; slept.fill(0);
		for (const auto& day : guard.m_days) {
			for (int i = 0; i < 60; ++i) {
				if (day.second[i]) ++slept[i];
			}
		}
		int max = *std::max_element(slept.begin(), slept.end());
		if (max > maxMinutes) {
			minute = std::distance(slept.begin(), std::max_element(slept.begin(), slept.end()));
			ID = guard.m_id;
			maxMinutes = max;
		}
	}

	return minute * ID;

}



int main()
{
	auto t1 = std::chrono::steady_clock::now();
	auto data = prepareData("input.txt");
	auto guards = prepareGuards(data);
	auto solutionA = taskA(guards);
	auto solutionB = taskB(guards);
	auto t2 = std::chrono::steady_clock::now();
	cout << "A: "<< solutionA << endl;
	cout << "B: "<< solutionB << endl;
	cout << "Solved in: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";

	system("pause");
}