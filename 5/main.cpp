#include <fstream>
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <locale>
#include <chrono>

using std::fstream;
using std::cout; using std::endl;

//just to practise using threads, well was bad idea here
//should operate on stack/que string is too expensive

std::string loadData(const char* path)
{
	std::string data;
	fstream file(path, std::ios::in); if (!file.good()) return data;
	std::getline(file, data);
	file.close();
	return data;
}
auto partData(std::string& data)
{
	std::vector<std::string> newData;
	int partition = data.size() / 4;
	for (int i = 0; i < 4; ++i) {
		newData.push_back(data.substr(i*partition, partition));
	}
	return newData;
}


void react(std::string& s)
{
	bool reaction = true;
	constexpr int difference = 'a' - 'A';

	while (reaction) {
		reaction = false;
		for (int i = 0; i < s.size()-1; ++i) {
			if (s[i] - difference == s[i + 1] || s[i] + difference == s[i + 1]) {
				s.erase(i, 2);
				reaction = true;
			}
		}
	}

}

void taskA(std::vector<std::string>& data,int &answer)
{
	std::thread threads[4];
	for (int i = 0; i < 4; ++i) {
		threads[i] = std::thread(react, std::ref(data[i]));
	}
	for (auto& thread : threads) thread.join();

	std::string afterThreads = "";
	for (auto& s : data) afterThreads += s;
	react(afterThreads);
	
	answer = afterThreads.size();
}


std::string copyStrWithoutLetter(const std::string &s, char c)
{
	std::locale loc;
	std::string newStr;
	for (auto letter : s) {
		if (letter != c && letter && letter != toupper(c,loc)) 
			newStr += letter;
	}
	return newStr;
}

int taskB(std::string& data)
{
	int min = INT_MAX;
	int newMin;
	for (int letter = 'a'; letter <= 'z'; ++letter) {
		std::string newStr = copyStrWithoutLetter(data,letter);
		auto newData = partData(newStr);
		taskA(newData,newMin);
		if (min > newMin) min = newMin;
	}
	return min;
}


int main()
{
	auto t1 = std::chrono::steady_clock::now();
	auto data = loadData("input.txt");
	auto newData = partData(data);
	int answer; taskA(newData, answer);
	auto t2 = std::chrono::steady_clock::now();

	cout << answer << endl;
	cout << taskB(data) << endl;
	cout << "Solved in: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " ms\n";


	system("pause");
}

//needs large amount of time, at least 4s cuz of operating on strings