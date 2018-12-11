#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <cstdio>

using std::cout; using std::endl;

constexpr int GRID_SIZE = 300;
constexpr int input = 6392;

auto prepareGrid()
{
	std::array<std::array<long long, GRID_SIZE>, GRID_SIZE> grid;
	
	for (int y = 0; y < GRID_SIZE; ++y) {
		for (int x = 0; x < GRID_SIZE; ++x) {
			int rackID = x + 10 + 1;
			grid[y][x] = rackID * (y + 1);
			grid[y][x] += input;
			grid[y][x] *= rackID;
			std::string tempVal = std::to_string(grid[y][x]);
			grid[y][x] = tempVal[tempVal.size() - 3] - '0';
			grid[y][x] -= 5;
		}
	}
	
	return grid;
}

std::string taskA(std::array<std::array<long long,GRID_SIZE>,GRID_SIZE>& grid)
{
	int max = 0;
	int maxX, maxY;
	for (int y = 0; y < GRID_SIZE - 2; ++y) {
		for (int x = 0; x < GRID_SIZE - 2; ++x) {
			int newVal = 0;

			for (int i = 0; i < 3; ++i) {
				for (int j = 0; j < 3; ++j) {
					newVal += grid[y + i][x + j];
				}
			}

			if (newVal > max) {
				max = newVal;
				maxX = x + 1;
				maxY = y + 1;
			}
		}
	}


	return std::to_string(maxX) + "," + std::to_string(maxY);
}

std::string taskB(std::array<std::array<long long, GRID_SIZE>, GRID_SIZE>& grid)
{
	int max = 0;
	int maxX, maxY;
	int maxN;

	//wrong idea
	//https://www.geeksforgeeks.org/submatrix-sum-queries/ i will try to do it in my python solution

	for (int n = 2; n < GRID_SIZE; ++n) {
		
		for (int y = 0; y < GRID_SIZE - n + 1; ++y) {
			for (int x = 0; x < GRID_SIZE - n + 1; ++x) {
				int newVal = 0;
				for (int i = 0; i < n; ++i) {
					for (int j = 0; j < n; ++j) {
						newVal += grid[y + i][x + j];
					}
				}

				if (newVal > max) {
					max = newVal;
					maxX = x + 1;
					maxY = y + 1;
					maxN = n;
				}
			}
		}
	}


	return std::to_string(maxX) + "," + std::to_string(maxY) + "," + std::to_string(maxN);
}


int main()
{
	auto grid = prepareGrid();
	cout << taskA(grid) << endl;
	cout << "Now wait.. 1-2min\n";
	cout << taskB(grid) << endl;

	system("pause");
	return 0;
}