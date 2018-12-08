#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <chrono>

using std::fstream; using std::cout; using std::endl;

struct Node {

	int m_numChilds;
	int m_numValues;

	Node* m_parent = nullptr;
	std::vector<Node> m_childs;
	std::vector<int> m_values;
};

Node addNode(std::fstream& input, Node* previous)
{
	Node newNode;
	int childNodes; input >> childNodes;
	int numValues; input >> numValues;
	newNode.m_numChilds = childNodes;
	newNode.m_numValues = numValues;
	newNode.m_parent = previous;

	for (int i = 0; i < childNodes; ++i) {
		newNode.m_childs.push_back(addNode(input, &newNode));
	}

	for (int i = 0; i < numValues; ++i) {
		int val; input >> val;
		newNode.m_values.push_back(val);
	}

	return newNode;
}

auto prepareTree(const char* path)
{
	Node head;
	int recursiveLevel = 0;
	fstream input(path, std::ios::in); if (!input.good()) return head;
	while (!input.eof()) {
		head = addNode(input,nullptr);
	}
	input.close();

	return head;
}


int sumNode(Node &head)
{
	int sum = 0;
	for (auto& child : head.m_childs) {
		sum += sumNode(child);
	}
	return std::accumulate(head.m_values.begin(), head.m_values.end(), sum);
}


int sumNodeB(Node &head)
{
	int sum = 0;

	if (head.m_numChilds) {
		for (auto& val : head.m_values) {
			if (val <= head.m_numChilds) {
				sum += sumNodeB(head.m_childs[val - 1]);
			}
		}
	}
	else {
		sum += std::accumulate(head.m_values.begin(), head.m_values.end(), 0);
	}

	return sum;
}



int taskA(Node &head)
{
	int sum = sumNode(head);
	return sum;
}
int taskB(Node &head)
{
	int sum = sumNodeB(head);
	return sum;
}



int main()
{

	auto t1 = std::chrono::steady_clock::now();
	auto tree = prepareTree("input_org.txt");
	cout << taskA(tree) << endl;
	cout << taskB(tree) << endl;
	auto t2 = std::chrono::steady_clock::now();
	cout << "Solved in: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << endl;

	system("pause");
	return 0;
}