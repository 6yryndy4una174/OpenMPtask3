#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include <omp.h>

std::vector<std::vector<int>> generateGraph(int n, float chance) {
	std::vector<std::vector<int>> out(n, std::vector<int>(n, -1));
	for (int i = 0; i < n; ++i) {
		for (int j = i; j < n; ++j) {
			if (i == j)
			{
				out[i][j] = 0;
				continue;
			}
			int rand_num = rand() % 100;
			float val = (float)rand_num / 100;
			if (val <= chance) {
				out[i][j] = out[j][i] = 1 + rand() % 100;
			}
		}
	}
	return out;
}

std::vector<std::vector<int>> Floid(const std::vector<std::vector<int>>& graph) {
	std::vector<std::vector<int>> out(graph.size(), std::vector<int>(graph.size(), 10000));
	for (int i = 0; i < graph.size(); ++i) {
		for (int j = i; j < graph.size(); ++j) {
			if (graph[i][j] != -1)
				out[i][j] = out[j][i] = graph[i][j];
		}
	}

	for (int i = 0; i < graph.size(); ++i) {
		for (int j = 0; j < graph.size(); ++j) {
			for (int k = 0; k < graph.size(); ++k) {
				out[j][k] = std::min(out[j][k], out[j][i] + out[i][k]);
			}
		}
	}
	return out;
}

std::vector<std::vector<int>> ParallelFloid(const std::vector<std::vector<int>>& graph, int n) {
	std::vector<std::vector<int>> out(graph.size(), std::vector<int>(graph.size(), 10000));
	for (int i = 0; i < graph.size(); ++i) {
		for (int j = i; j < graph.size(); ++j) {
			if (graph[i][j] != -1)
				out[i][j] = out[j][i] = graph[i][j];
		}
	}

	for (int i = 0; i < graph.size(); ++i) {
		omp_set_num_threads(n);
#pragma omp parallel for
		for (int j = 0; j < graph.size(); ++j) {
			for (int k = 0; k < graph.size(); ++k) {
				out[j][k] = std::min(out[j][k], out[j][i] + out[i][k]);
			}
		}
	}
	return out;
}

int main() {
	std::vector<std::vector<int>> graph = generateGraph(1000, 0.5);
	clock_t start;
	clock_t end;

	start = clock();
	Floid(graph);
	end = clock();
	std::cout << end - start << std::endl;

	start = clock();
	ParallelFloid(graph, 4);
	end = clock();
	std::cout << end - start << std::endl;

	start = clock();
	ParallelFloid(graph, 8);
	end = clock();
	std::cout << end - start << std::endl;

	start = clock();
	ParallelFloid(graph, 16);
	end = clock();
	std::cout << end - start << std::endl;

	start = clock();
	ParallelFloid(graph, 100);
	end = clock();
	std::cout << end - start << std::endl;

	return 0;
}