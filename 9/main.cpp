#include <list>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
#include <chrono>

using std::cout; using std::endl;



long long taskA(int numPlayers, long maxValue)
{
	std::map<int, long long> leaderboard;
	std::list<long> circle; circle.push_back(0);
	//used list here, because of cheap erase/insert operations

	int currentPlayer = 1;
	int currentMarble = 1;
	auto currentIndex = circle.begin();
	
	while (currentMarble <= maxValue) {

		if (!(currentMarble % 23)) {
			leaderboard[currentPlayer] += currentMarble;

			for(int i=0;i<7;++i){
				if (currentIndex == circle.begin()) {
					currentIndex = circle.end();
				}
				--currentIndex;
			}
			leaderboard[currentPlayer] += *currentIndex;
			currentIndex = circle.erase(currentIndex);
			if (currentIndex == circle.end()) currentIndex = circle.begin();
		}

		else {
			++currentIndex;
			if (currentIndex == circle.end()) currentIndex = circle.begin();
			++currentIndex;
			currentIndex = circle.insert(currentIndex, currentMarble);
		}


		++currentMarble;
		if (currentPlayer == numPlayers) currentPlayer = 1;
		else ++currentPlayer;

		
	}
	

	
	return std::max_element(leaderboard.begin(), leaderboard.end(), [](const std::pair<int, long long>& p1, const std::pair<int, long long>& p2) {return p1.second < p2.second; })->second;
}




int main()
{

	auto t1 = std::chrono::steady_clock::now();
	cout << taskA(428, 70825) << endl;
	cout << taskA(428, 70825 * 100) << endl;
	auto t2 = std::chrono::steady_clock::now();
	cout << "Solved in: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << endl;


	
	system("pause");
}