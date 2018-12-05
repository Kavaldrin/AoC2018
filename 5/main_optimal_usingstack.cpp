#include <stack>
#include <fstream>
#include <locale>
#include <iostream>
#include <chrono>

using std::cout; using std::endl; using std::fstream;

auto loadData(const char *path) 
{
	std::string data;
	fstream input(path, std::ios::in); if (!input.good())  return data;
	std::getline(input, data);
	input.close();
	return data;
}

int taskA(const std::string& s) 
{
	std::locale loc;
	constexpr int difference = 'a' - 'A';
	std::stack<char> stack;
	for (const auto& c : s) {
		if (stack.empty()) stack.push(c);
		else if (stack.top() != c && (std::toupper(stack.top(), loc) == toupper(c, loc))) stack.pop();
		else stack.push(c);
	}

	return stack.size();
}
int taskBhelper(const std::string&s, char notC)
{
	std::locale loc;
	constexpr int difference = 'a' - 'A';
	std::stack<char> stack;
	for (const auto& c : s) {
		if (std::toupper(c, loc) != std::toupper(notC, loc)) {
			if (stack.empty()) stack.push(c);
			else if (stack.top() != c && (std::toupper(stack.top(), loc) == toupper(c, loc))) stack.pop();
			else stack.push(c);
		}
	}

	return stack.size();


}

int taskB(const std::string& s)
{
	int min = INT_MAX;
	for (int letter = 'a'; letter <= 'z'; ++letter) {
		auto result = taskBhelper(s, letter);
		if (min > result) min = result;
	}
	return min;
}


int main()
{
	auto t1 = std::chrono::steady_clock::now();
	auto data = loadData("input.txt");
	cout << "A: " << taskA(data) << " B: " << taskB(data)<< endl;
	auto t2 = std::chrono::steady_clock::now();
	cout << "Solved in: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << endl;

	system("pause");
}

//average time ~250ms -/+ 15 times faster