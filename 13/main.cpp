#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include <algorithm>
#include <set>
#include <chrono>

using std::fstream; using std::cout; using std::endl;

struct Cart {
	Cart(int x,int y, char direction):m_xpos(x),m_ypos(y),m_direction(direction){}
	int m_xpos;
	int m_ypos;
	char m_direction;
	unsigned m_crossCounter = 1;
};

auto findCartsInLine(std::string &line,int y)
{
	std::vector<Cart> carts;

	for (int x = 0; x < line.size(); ++x){
		if (line[x] == '>' || line[x] == '<' || line[x] == 'v' || line[x] == '^') {
			carts.emplace_back(x, y, line[x]);
		}
	}
	return carts;
}

void repairMap(std::vector<std::string>& map, const std::vector<Cart>& carts)
{
	for (const auto& cart : carts) {

		int y = cart.m_ypos, x = cart.m_xpos;

		// '-'
		if (x > 0 && x < map[0].size() - 1) {
			if ((map[y][x - 1] == '-' || map[y][x - 1] == '\\' || map[y][x - 1] == '/' || map[y][x - 1] == '+')
				&& (map[y][x + 1] == '-' || map[y][x + 1] == '\\' || map[y][x + 1] == '/' || map[y][x + 1] == '+')
				) map[y][x] = '-';
		}
		// '|'
		if (y > 0 && y < map.size()) {
			if ((map[y - 1][x] == '|' || map[y - 1][x] == '\\' || map[y - 1][x] == '/' || map[y - 1][x] == '+')
				&& (map[y + 1][x] == '|' || map[y + 1][x] == '\\' || map[y + 1][x] == '/' || map[y + 1][x] == '+')
				) map[y][x] = '|';
		}

		// '/'
		if (x < map[0].size() - 1 && y < map.size() - 1) {
			if ((map[y + 1][x] == '|' || map[y + 1][x] == '+')
				&& (map[y][x + 1] == '-' || map[y][x + 1] == '+')
				) map[y][x] = '/';
		}
		if (y > 0 && x > 0) {
			if ((map[y - 1][x] == '|' || map[y - 1][x] == '+')
				&& (map[y][x - 1] == '-' || map[y][x - 1] == '+')
				) map[y][x] = '/';
		}
		
		// '\'
		if (x > 0 && y < map.size() - 1) {
			if((map[y+1][x] == '|' || map[y + 1][x] == '+')
				&& (map[y][x-1] == '-' || map[y][x - 1] == '+')
				) map[y][x] = '\\';
		}
		if (x < map[0].size() - 1 && y > 0) {
			if ((map[y - 1][x] == '|' || map[y - 1][x] == '+')
				&& (map[y][x + 1] == '-' || map[y][x + 1] == '+')
				) map[y][x] = '\\';
		}

		
	}


}

auto prepareMapAndCarts(const char* path)
{
	
	fstream input(path, std::ios::in); if (!input.good()) exit(-1);
	std::vector<std::string> map;
	std::vector<Cart> carts;

	while (!input.eof()) {
		std::string line;
		std::getline(input, line);
		auto newCarts = findCartsInLine(line, map.size());
		if (!newCarts.empty()) {
			if (carts.empty()) {
				carts = std::move(newCarts);
			}
			else {
				std::move(std::begin(newCarts),std::end(newCarts),std::back_inserter(carts));
			}
		}
		map.push_back(std::move(line));
	}
	input.close();
	repairMap(map, carts);

	return std::make_pair(map, carts);
}

void makeCartMove(Cart& cart, std::vector<std::string>& map)
{
	switch (cart.m_direction) {
	case '^': --cart.m_ypos; break;
	case 'v': ++cart.m_ypos; break;
	case '>': ++cart.m_xpos; break;
	case '<': --cart.m_xpos; break;
	}


	int newX = cart.m_xpos, newY = cart.m_ypos;

	switch (map[newY][newX]) {
	case '\\':
		if (cart.m_direction == '^') cart.m_direction = '<';
		else if (cart.m_direction == '>') cart.m_direction = 'v';
		else if (cart.m_direction == 'v') cart.m_direction = '>';
		else if (cart.m_direction == '<') cart.m_direction = '^';
		break;
	case '/':
		if (cart.m_direction == '^') cart.m_direction = '>';
		else if (cart.m_direction == '>') cart.m_direction = '^';
		else if (cart.m_direction == 'v') cart.m_direction = '<';
		else if (cart.m_direction == '<') cart.m_direction = 'v';
		break;
	case '+':

		if (cart.m_crossCounter == 1) {
			if (cart.m_direction == '^') cart.m_direction = '<';
			else if (cart.m_direction == '>') cart.m_direction = '^';
			else if (cart.m_direction == 'v') cart.m_direction = '>';
			else if (cart.m_direction == '<') cart.m_direction = 'v';
		}
		else if (cart.m_crossCounter == 3) {
			if (cart.m_direction == '^') cart.m_direction = '>';
			else if (cart.m_direction == '>') cart.m_direction = 'v';
			else if (cart.m_direction == 'v') cart.m_direction = '<';
			else if (cart.m_direction == '<') cart.m_direction = '^';
		}
		++cart.m_crossCounter;
		if (cart.m_crossCounter == 4) cart.m_crossCounter = 1;
		break;

	}
}


bool isCrash(const std::vector<Cart>& carts)
{
	for (auto it = carts.begin(); it != carts.end() - 1; ++it) {
		for (auto it2 = it + 1; it2 != carts.end(); ++it2) {
			if (it->m_xpos == it2->m_xpos && it->m_ypos == it2->m_ypos) return true;
		}
	}

	return false;

}


void prepareCrashedtoRemove(std::vector<Cart>& carts,std::set<int>& toRemove)
{
	for (auto it = carts.begin(); it != carts.end() - 1; ++it) {
		for (auto it2 = it + 1; it2 != carts.end(); ++it2) {
			if (it->m_xpos == it2->m_xpos && it->m_ypos == it2->m_ypos) {
				toRemove.insert(std::distance(carts.begin(), it));
				toRemove.insert(std::distance(carts.begin(), it2));
			}
		}
	}
	
}

bool nextMapStateA(std::vector<std::string>& map, std::vector<Cart>& carts)
{

	std::sort(carts.begin(), carts.end(), [](const Cart& lhs, const Cart& rhs) {
		if (lhs.m_ypos == rhs.m_ypos) return lhs.m_xpos < rhs.m_xpos;
		else return lhs.m_ypos < rhs.m_ypos;
		return true;
	});

	for (int i = 0; i < carts.size(); ++i) {
		makeCartMove(carts[i], map);
		if (isCrash(carts)) {
			return true;
		}
	}

	return false;
}

void nextMapStateB(std::vector<std::string>& map, std::vector<Cart>& carts)
{

	std::sort(carts.begin(), carts.end(), [](const Cart& lhs, const Cart& rhs) {
		if (lhs.m_ypos == rhs.m_ypos) return lhs.m_xpos < rhs.m_xpos;
		else return lhs.m_ypos < rhs.m_ypos;
		return true;
	});

	std::set<int> toRemove;


	for (int i = 0; i < carts.size(); ++i) {
		makeCartMove(carts[i], map);
		if (isCrash(carts)) {
			prepareCrashedtoRemove(carts, toRemove);
		}
	}

	for (auto it = toRemove.rbegin(); it != toRemove.rend(); ++it) {
		carts.erase(carts.begin() + *it);
	}
	
}

std::string getCrashInfo(const std::vector<Cart>& carts)
{
	for (auto it = carts.begin(); it != carts.end() - 1; ++it) {
		for (auto it2 = it + 1; it2 != carts.end(); ++it2) {
			if (it->m_xpos == it2->m_xpos && it->m_ypos == it2->m_ypos) return std::to_string(it->m_xpos) + "," + std::to_string(it->m_ypos);
		}
	}
	return "";
}

std::string taskA(std::vector<std::string>& map, std::vector<Cart> carts)
{
	while (!nextMapStateA(map, carts));
	return getCrashInfo(carts);
}

std::string taskB(std::vector<std::string>& map, std::vector<Cart> carts)
{
	while (carts.size() != 1) nextMapStateB(map, carts);
	return std::to_string(carts[0].m_xpos) + "," + std::to_string(carts[0].m_ypos);
}

int main()
{
	auto t1 = std::chrono::steady_clock::now();
	auto[map, carts] = prepareMapAndCarts("input_org.txt");
	cout << taskA(map, carts) << endl;
	cout << taskB(map, carts) << endl;
	auto t2 = std::chrono::steady_clock::now();
	cout << "Solved in: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << endl;
	

	system("pause");
}