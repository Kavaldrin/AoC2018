#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include <algorithm>
#include <set>
#include <chrono>

using std::fstream; using std::cout; using std::endl;
int maxX, maxY, minX, minY;

struct Field {
	int m_minTo;
	std::vector<std::pair<int, int>> m_pointsDistance;
};

auto readPoints(const char* path)
{
	std::vector<std::pair<int, int>> points;
	fstream input(path, std::ios::in); if (!input.good()) return points;
	while (!input.eof()) {
		std::pair<int, int> temp;
		std::string line;
		std::getline(input, line);
		sscanf_s(line.c_str(), "%d, %d", &temp.first, &temp.second);
		points.push_back(std::move(temp));
	}
	return points;
}

void findMinMaxXY(const std::vector<std::pair<int, int>>& points)
{
	maxX = points[0].first, maxY = points[0].second, minX = points[0].first, minY = points[0].second;
	for (auto &p : points) {
		if (p.first > maxX) maxX = p.first;
		if (minX > p.first) minX = p.first;
		if (p.second > maxY) maxY = p.second;
		if (minY > p.second) minY = p.second;
	}
}

auto makeGrid()
{
	std::vector<std::vector<Field>> fields;
	fields.reserve(maxX - minX + 1); fields.resize(maxX - minX + 1);
	for (auto& field : fields) {
		field.reserve(maxY - minY + 1);
		field.resize(maxY - minY + 1);
	}
	return fields;
}


void prepareGrid(std::vector<std::vector<Field>>& fields, const std::vector<std::pair<int, int>>& points)
{
	int index = 1;

	for (auto& p : points) {
		int x_p = p.first - minX;
		int y_p = p.second - minY;

		for (int x = 0; x < maxX - minX + 1; ++x) {
			for (int y = 0; y < maxY - minY + 1; ++y) {
				fields[x][y].m_pointsDistance.push_back(std::make_pair(index, std::abs(x_p - x) + std::abs(y_p - y)));
			}
		}
		++index;
	}

	for (auto &vec : fields) {
		for (auto &v : vec) {


			int min = INT_MAX;
			int minPoint = 0;
			for (auto& p : v.m_pointsDistance) {
				if (min > p.second) {
					min = p.second;
					minPoint = p.first;
				}
				else if (min == p.second) {
					minPoint = 0;
				}
			}
			v.m_minTo = minPoint;

		}
	}


}

auto disqualifyPoints(const std::vector<std::vector<Field>>& grid)
{
	std::set<int> set;
	for (int i = 0; i < maxX - minX + 1; ++i) set.insert(grid[i][0].m_minTo);
	for (int i = 0; i < maxX - minX + 1; ++i) set.insert(grid[i][maxY - minY].m_minTo);
	for (int j = 0; j < maxY - minY + 1; ++j) set.insert(grid[0][j].m_minTo);
	for (int j = 0; j < maxY - minY + 1; ++j) set.insert(grid[maxX - minX][j].m_minTo);
	return set;
}


/*DEBUG

void printGrid(const std::vector<std::vector<Field>>& grid)
{

	for (auto& vec : grid) {
		for (auto& p : vec) {
			cout << p.m_minTo << " ";
		}
		cout << endl;
	}
}

*/

int taskA(std::vector<std::vector<Field>>& grid, std::set<int>& disq, int numPoints)
{
	std::vector<int> answer; answer.reserve(numPoints + 1); answer.resize(numPoints + 1);
	answer.assign(numPoints, 0);
	for (auto& row : grid) {
		for (auto& f : row) {
			auto foundIn = disq.find(f.m_minTo);
			if (foundIn == disq.end()) {
				++answer[f.m_minTo];
			}
		}
	}
	return *std::max_element(answer.begin(), answer.end());
}


int taskB(std::vector<std::vector<Field>>& grid, const int maxDistance)
{
	int familyFriendlyRegion = 0;

	for (auto& row : grid) {
		for (auto& field : row) {
			int sum = 0;
			for (auto &dist : field.m_pointsDistance) {
				sum += dist.second;
			}
			if (sum < maxDistance) ++familyFriendlyRegion;
		}
	}

	return familyFriendlyRegion;
}



int main()
{
	auto t1 = std::chrono::steady_clock::now();
	auto points = readPoints("input.txt");
	findMinMaxXY(points);
	auto grid = makeGrid();
	prepareGrid(grid, points);
	auto disqualifed = disqualifyPoints(grid);
	cout << "A: " << taskA(grid, disqualifed, points.size()) << endl;
	cout << "B: " << taskB(grid, 10000) << endl;
	auto t2 = std::chrono::steady_clock::now();
	cout << "Solved in: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << endl;


	system("pause");

}