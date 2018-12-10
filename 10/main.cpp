#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <chrono>

using std::fstream; using std::cout; using std::endl;


struct Particle {

	Particle(int xPos, int yPos, int vX, int vY) :m_xPos(xPos), m_yPos(yPos), m_vX(vX), m_vY(vY) {}

	void makeMove(){
		m_xPos += m_vX;
		m_yPos += m_vY;
	}
	void makeStepBack() {
		m_xPos -= m_vX;
		m_yPos -= m_vY;
	}


	int m_xPos;
	int m_yPos;
	int m_vX;
	int m_vY;

};

void printParticles(std::vector<Particle>& particles)
{

	int x_max = std::max_element(particles.begin(), particles.end(), [](const Particle& p1, const Particle &p2) { return p1.m_xPos < p2.m_xPos; })->m_xPos;
	int x_min = std::min_element(particles.begin(), particles.end(), [](const Particle& p1, const Particle &p2) { return p1.m_xPos < p2.m_xPos; })->m_xPos;
	int y_max = std::max_element(particles.begin(), particles.end(), [](const Particle& p1, const Particle &p2) { return p1.m_yPos < p2.m_yPos; })->m_yPos;
	int y_min = std::min_element(particles.begin(), particles.end(), [](const Particle& p1, const Particle &p2) { return p1.m_yPos < p2.m_yPos; })->m_yPos;
	int index = 0;

	std::sort(particles.begin(), particles.end(),
		[](const Particle& p1, const Particle& p2) {
		if (p1.m_yPos == p2.m_yPos) return p1.m_xPos < p2.m_xPos;
		else return p1.m_yPos < p2.m_yPos;
		return true;
	});

	bool allParticles = false;
	for (int j = y_min; j <= y_max && !allParticles; ++j) {
		for (int i = x_min; i <= x_max && !allParticles; ++i) {
			if (particles[index].m_xPos == i && particles[index].m_yPos == j) {
				cout << "#";
				++index;
				if (index < particles.size()) {
					while (index < particles.size() && (particles[index].m_xPos == i && particles[index].m_yPos == j)) ++index;
					if (index == particles.size()) allParticles = true;
				}
				else 
					allParticles = true;
			}
			else
				cout << ".";
		}
		cout << endl;
	}

	cout << endl << endl;
}

auto prepareParticles(const char* path)
{
	std::vector<Particle> particles;
	fstream input(path, std::ios::in); if (!input.good()) return particles;

	while (!input.eof()) {
		std::string temp;
		std::getline(input, temp);
		int x, y, xv, yv;
		sscanf_s(temp.c_str(),"position=< %d, %d> velocity=<%d,  %d>", &x, &y, &xv, &yv);
		particles.emplace_back(x, y, xv, yv);
	}
	input.close();
	return particles;
}

void taskAB(std::vector<Particle>& particles)
{
	int differenceX = INT_MAX - 1;
	int differenceY = INT_MAX - 1;
	int prevDifferenceX = INT_MAX;
	int prevDifferenceY = INT_MAX;
	int iteration = 0;

	while (differenceX < prevDifferenceX && differenceY < prevDifferenceY) {

		for (auto& particle : particles) particle.makeMove();
		int x_max = std::max_element(particles.begin(), particles.end(), [](const Particle& p1, const Particle &p2) { return p1.m_xPos < p2.m_xPos; })->m_xPos;
		int x_min = std::min_element(particles.begin(), particles.end(), [](const Particle& p1, const Particle &p2) { return p1.m_xPos < p2.m_xPos; })->m_xPos;
		int y_max = std::max_element(particles.begin(), particles.end(), [](const Particle& p1, const Particle &p2) { return p1.m_yPos < p2.m_yPos; })->m_yPos;
		int y_min = std::min_element(particles.begin(), particles.end(), [](const Particle& p1, const Particle &p2) { return p1.m_yPos < p2.m_yPos; })->m_yPos;

		prevDifferenceX = differenceX;
		prevDifferenceY = differenceY;

		differenceX = std::abs(x_max - x_min);
		differenceY = std::abs(y_max - y_min);
		++iteration;

		//cout << iteration << " " << differenceX << " " << differenceY << endl;
	}


	for (auto& particle : particles) particle.makeStepBack();
	printParticles(particles);
	cout << "Seconds: " << iteration - 1 << endl;
	

}



int main()
{
	auto t1 = std::chrono::steady_clock::now();
	auto particles = prepareParticles("input_org.txt");
	taskAB(particles);
	auto t2 = std::chrono::steady_clock::now();
	cout << "Solved in: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << endl;


	system("pause");
}