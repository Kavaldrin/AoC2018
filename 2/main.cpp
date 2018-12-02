#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>


using std::fstream;
using std::cout; using std::endl;


std::vector<std::string> prepareData(std::string fileName)
{
	fstream file(fileName.c_str(), std::ios::in);
	if (!file.good()) {
		throw std::runtime_error("Cannot open the file");
	}
	std::vector<std::string> data;
	while (!file.eof()) {
		std::string line;
		std::getline(file, line);
		data.push_back(std::move(line));
	}
	file.close();

	return data;
}

int taskA(const std::vector<std::string>& data)
{
	int twos = 0; int threes = 0;
	auto toAddThree = false; auto toAddTwo = false;
	for (const auto& str : data) {
		std::map<char, int> counter;
		for (const auto& letter : str) {
			auto found = counter.find(letter);
			if (found != counter.end()) ++(found->second);
			else counter[letter] = 1;
		}
		for (auto& pr : counter) {
			if (pr.second == 2) toAddTwo = true;
			if (pr.second == 3) toAddThree = true;
		}
		
		if (toAddTwo) ++twos;
		if (toAddThree) ++threes;
		toAddTwo = false; toAddThree = false;
	}

	return twos * threes;
}


std::string taskB(const std::vector<std::string>& data)
{
	const std::size_t strSize = data[0].size();

	for (auto it = data.begin(); it != data.end() - 1; ++it) {
		for (auto insideIt = it + 1; insideIt != data.end(); ++insideIt) {
			
			int compatilibty = 0;
			for (int i = 0; i < strSize; ++i) {
				if ((*it)[i] == (*insideIt)[i]) ++compatilibty;
			}
			if (compatilibty == strSize - 1) {
				std::string answer = "";
				for (int i = 0; i < strSize; ++i) {
					if ((*it)[i] == (*insideIt)[i]) answer += (*it)[i];
				}
				return answer;
			}
		}
	}
	return "error";
}



int main()
{
	auto data = prepareData("input.txt");
	cout << "A: " << taskA(data) << endl;
	fstream file("answer.txt", std::ios::out); if (!file.good()) return -1;
	file << taskB(data);
	cout << "B: answer in file\n";
	file.close();


	system("pause");
}