#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <string>
#include <array>
#include <cstdio>


using std::fstream; using std::cout; using std::endl;
using elemType = std::pair<std::pair<int, int>, std::pair<int, int>>;
using dataType = std::vector <elemType>;
using counterType = std::map<std::pair<int, int>, int>;

void parseLine(std::string& str, dataType& data)
{
	elemType elem;
	/*
	std::string token = "@";
	str = str.substr (str.find(token) + 2,std::string::npos);

	token = ":";
	std::string pos = str.substr(0, str.find(token));
	std::string size = str.substr(str.find(token) + 2, std::string::npos);
	
	token = ",";
	elem.first.first = std::stoi(pos.substr(0, pos.find(token)));
	elem.first.second = std::stoi(pos.substr(pos.find(token)+1, std::string::npos));

	token = "x";
	elem.second.first = std::stoi(size.substr(0, size.find(token)));
	elem.second.second = std::stoi(size.substr(size.find(token) + 1, std::string::npos));
	XDDDDDDDDDDDDDDDD!!!!!!!!!!!!!!! ty no nie wiem jak tam twoje parsowanie*/
	int ignore;
	sscanf_s(str.c_str(), "#%d @ %d,%d: %dx%d",&ignore, &elem.first.first, &elem.first.second, &elem.second.first, &elem.second.second);
	data.push_back(std::move(elem));
}

auto prepareData(const char* path)
{
	dataType data;
	fstream input(path, std::ios::in); if (!input.good()) return data;


	while (!input.eof()) {
		std::string temp;
		std::getline(input, temp);
		parseLine(temp, data);
	}
	input.close();


	return data;
}


int taskA(const dataType& data,counterType& counter)
{	
	
	for (auto& elem : data) {
		int xSize = elem.second.first;
		int ySize = elem.second.second;
		int maxX = elem.first.first + xSize;
		int maxY = elem.first.second + ySize;

		for (int x = elem.first.first; x < maxX; ++x) {
			for (int y = elem.first.second; y < maxY; ++y) {
				++counter[std::pair<int, int>(x, y)];
			}
		}

	}

	int answer = 0;

	for (auto& elem : counter)
		if (elem.second > 1)
			++answer;

	return answer;

}

int taskB(const dataType& data,std::map<std::pair<int,int>,int>& counter)
{
	

	bool unique = true;
	int index = 0;

	for (auto& elem : data) {
		++index;
		int xSize = elem.second.first;
		int ySize = elem.second.second;
		int maxX = elem.first.first + xSize;
		int maxY = elem.first.second + ySize;

		for (int x = elem.first.first; x < maxX; ++x) {
			for (int y = elem.first.second; y < maxY; ++y) {
				if (counter[std::pair<int, int>(x, y)] > 1)
					unique = false;
				if (!unique) break;
			}
			if (!unique) break;
		}

		if (unique) return index;
		unique = true;
	}
	return -1;
	
}

auto solve(const dataType& data)
{
	counterType counter;
	int A = taskA(data, counter);
	int B = taskB(data, counter);
	return std::make_pair(A, B);

}

int main()
{
	auto data = prepareData("input.txt");
	auto solution = solve(data);
	cout << "A: " << solution.first << " B: " << solution.second<< endl;



	system("pause");

}