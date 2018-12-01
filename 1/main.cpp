#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <thread>

using std::fstream;
using std::cout; using std::endl;

void taskA(const std::vector<int>& vec,int& solution)
{
	 solution = std::accumulate(vec.cbegin(), vec.cend(), 0);
}
void taskB(const std::vector<int>& vec, int& solution)
{
	std::vector<int> frequences; frequences.push_back(0);
	while (true) {
		for (auto& f : vec) {
			int newfreq = frequences.back() + f;
			for (auto &existingf : frequences) {
				if (newfreq == existingf) {
					solution = newfreq;
					return;
				}
			}
			frequences.push_back(newfreq);
		}
	}
}


int main()
{
	fstream file("input.txt", std::ios::in);
	if (!file.good()){ 
		 std::cerr << "Cannot open the file\n";
		 return -1;
	}
	std::vector<int> data;
	while (!file.eof()) {
		int temp;
		file >> temp;
		data.push_back(temp);
	}
	file.close();

	int solutionA;
	int solutionB;

	std::thread t1(taskA, std::ref(data),std::ref(solutionA));
	std::thread t2(taskB, std::ref(data),std::ref(solutionB));
	t1.join(); t2.join();
	
	cout << "A: " << solutionA << " B: " << solutionB << endl;

	system("pause");

}