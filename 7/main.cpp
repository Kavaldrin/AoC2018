#include <set>
#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using std::fstream; using std::cout; using std::endl;

/*
Step C must be finished before step A can begin.
*/


struct Worker {
	char m_workingOn = 0;
	int m_timeLeft = 0;
};


auto preparePoints(const char* path)
{

	std::map<char, std::set<char>> points;
	fstream input(path, std::ios::in); if (!input.good()) return points;
	while (!input.eof()) {
		std::string temp;
		std::getline(input, temp);
		char current = temp[36], toDoBefore = temp[5]; //na pale i co
		points[current].insert(toDoBefore);
	}
	input.close();
	return points;
}


std::string taskA(const std::map<char, std::set<char>>& points)
{
	std::string answer = "";
	std::set<char> ready;
	std::set<char> completed;


	//preparing starting points
	std::set<char> toDoBefore;
	std::set<char> current;
	for (auto& elem : points) {
		for (auto& p : elem.second) toDoBefore.insert(p);
		current.insert(elem.first);
	}
	for (auto& elem : toDoBefore) {
		auto found = current.find(elem);
		if (found == current.end()) ready.insert(elem);
	}
	toDoBefore.clear(); current.clear();
	////////////////



	while (!ready.empty()) {
		completed.insert(*ready.begin());
		answer += *ready.begin();
		ready.erase(*ready.begin());

		for (auto& elem : points) {

			auto checkPoint = completed.find(elem.first);
			if (checkPoint != completed.end()) continue;

			bool toAdd = true;
			for (auto& toDo : elem.second) {
				auto found = completed.find(toDo);
				if (found == completed.end()) {
					toAdd = false;
					break;
				}
			}
			if (toAdd) ready.insert(elem.first);
		}
	}

	return answer;
}



int taskB(const std::map<char, std::set<char>>& points, const int workersLimit)
{
	std::string answer = "";
	std::set<char> ready;
	std::set<char> completed;


	//preparing starting points
	std::set<char> toDoBefore;
	std::set<char> current;
	for (auto& elem : points) {
		for (auto& p : elem.second) toDoBefore.insert(p);
		current.insert(elem.first);
	}
	for (auto& elem : toDoBefore) {
		auto found = current.find(elem);
		if (found == current.end()) ready.insert(elem);
	}
	toDoBefore.clear(); current.clear();
	//


	//3 sets and vector for clarity of code
	std::vector <Worker> workers; workers.reserve(workersLimit); workers.resize(workersLimit);
	std::set<int> idleWorkers; for (int i = 0; i < workersLimit; ++i) idleWorkers.insert(i);
	std::set<int> workingWorkers;
	std::set<char> workingOn;
	
	int seconds = -1;

	while (true) {

		++seconds;

		//////////////////////////// checking that work is done
		std::vector<int> toEraseAdd;
		for (auto& workingNum : workingWorkers) {
			if (!workers[workingNum].m_timeLeft) {
				answer += workers[workingNum].m_workingOn;
				workingOn.erase(workers[workingNum].m_workingOn);
				completed.insert(workers[workingNum].m_workingOn);
				workers[workingNum].m_workingOn = 0;
				toEraseAdd.push_back(workingNum);
				
			}
		}
		for (auto& num : toEraseAdd) {
			workingWorkers.erase(num);
			idleWorkers.insert(num);
		}
		////////////////////////////////////

		//////looking for new avalible task
		for (auto& elem : points) {

			auto checkPoint = completed.find(elem.first);
			auto checkPoint2 = workingOn.find(elem.first);
			if (checkPoint != completed.end()) continue;
			if (checkPoint2 != workingOn.end()) continue;


			bool toAdd = true;
			for (auto& toDo : elem.second) {
				auto found = completed.find(toDo);
				if (found == completed.end()) {
					toAdd = false;
					break;
				}
			}
			if (toAdd) ready.insert(elem.first);
		}
		/////////////////////////



		//////////////////////////////// adding new task

		std::vector<char> toEraseTask;
		for (auto& newTask : ready) {
			if (idleWorkers.empty()) break;

			for(auto& idleNum : idleWorkers) {
				if (!workers[idleNum].m_workingOn) {
					workers[idleNum].m_workingOn = newTask;
					workingOn.insert(newTask);
					toEraseTask.push_back(newTask);
					workers[idleNum].m_timeLeft = newTask - 'A'+61;
					workingWorkers.insert(idleNum);
					idleWorkers.erase(idleNum);
					break;
				}
			}
		}
		for (auto& task : toEraseTask) {
			ready.erase(task);
		}
		//////////////////////////////////


		for (auto& workerNum : workingWorkers) {
			--workers[workerNum].m_timeLeft;
		}

		if (ready.empty() && idleWorkers.size() == workersLimit) break;


	}

	return seconds;
}


int main()
{

	auto t1 = std::chrono::steady_clock::now();
	auto points = preparePoints("input_org.txt");
	cout << taskA(points) << endl;
	cout << taskB(points, 5) << endl;
	auto t2 = std::chrono::steady_clock::now();
	cout << "Solved in: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << endl;

	system("pause");
}
