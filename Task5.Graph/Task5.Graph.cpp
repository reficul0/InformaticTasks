// Task5.Graph.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <stdio.h>
#include <iomanip> 
#include <algorithm>
#include <iterator>
#include <vector>
#include <list>
#include <tuple>
#include <random>
#include <time.h>

/*
10. Запишите программу нахождения диаметра, радиуса и центров графа по матрице минимальных расстояний.
*/

template<typename ValueType, typename IsValueValidFunction, typename MessageT>
ValueType GetValueFromUser(MessageT&& message, IsValueValidFunction isValueValid)
{
	using namespace std;

	ValueType value = 0;
	bool isValueInvalid = true;

	while (isValueInvalid)
	{
		cout << std::forward<MessageT>(message);
		cin >> value;

		bool isStreamInFailureState = cin.fail();
		isValueInvalid = isStreamInFailureState || !isValueValid(value);

		if (isValueInvalid)
		{
			if (isStreamInFailureState)
				cin.clear();// возвращаем объект потока в обычный режим
			cout << "Value is invalid. Try enter again. " << endl;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	system("cls");

	return value;
}


using EdgeValueT = std::int64_t;
using AdjacencyGraph = std::vector<std::vector<EdgeValueT>>;

template<typename GraphT>
struct GraphInfo
{
	EdgeValueT radius, diameter;
	std::list<size_t> centers;
};

template<typename GraphT>
void PrintGraph(GraphT const &graph, GraphInfo<GraphT> const &graphInfo)
{
	size_t const numbersInDiameterVal = std::log10(graphInfo.diameter) + 1,
				 numbersInMaxVertexId = std::log10(graph.size()) + 1;

	for (size_t vertexId(0); vertexId < graph.size(); ++vertexId)
	{
		std::cout << "[" << std::setfill(' ') << std::setw(numbersInDiameterVal) << vertexId << std::ends << "] ";

		for (auto const & vertexEdgeValue : graph[vertexId])
			std::cout << std::setfill(' ') << std::setw(numbersInDiameterVal) << vertexEdgeValue << std::ends << ' ';

		std::cout << '\n';
	}
	std::cout << '\n';
}

template<typename GraphT>
GraphInfo<std::decay_t<GraphT>> GetGraphInfo(GraphT const &graph)
{
	auto graphCopy = graph;
	return GetGraphInfo(std::move(graphCopy));
}

template<typename GraphT>
GraphInfo<std::decay_t<GraphT>> GetGraphInfo(GraphT &&graph)
{
	auto const N = graph.size();
	std::vector< EdgeValueT > eccentricities(N, INT64_MIN);

	// получаем матрицу минимальных расстояний через Алгоритм Флойда-Уоршелла
	for (size_t k = 0; k < N; ++k)
		for (size_t j = 0; j < N; ++j)
			for (size_t i = 0; i < N; ++i)
				graph[i][j] = std::min(graph[i][j], graph[i][k] + graph[k][j]);

	for (size_t i = 0; i < N; ++i)
		for (size_t j = 0; j < N; ++j)
			eccentricities[i] = std::max(eccentricities[i], graph[i][j]);


	GraphInfo<std::decay_t<GraphT>> graphInfo;

	auto const &eMinMax = std::minmax_element(eccentricities.begin(), eccentricities.end());
	graphInfo.radius = *eMinMax.first;
	graphInfo.diameter = *eMinMax.second;

	for (size_t vertexId(0); vertexId < N; ++vertexId)
		if (graphInfo.radius == eccentricities[vertexId])
			graphInfo.centers.push_back(vertexId);

	std::cout << "Graph of minimum distances: \n";
	PrintGraph(graph, graphInfo);

	std::cout << "Eccentricities: \n";
	for (size_t vertexId(0); vertexId < eccentricities.size(); ++vertexId)
		std::cout << "[" << vertexId << "] " << eccentricities[vertexId] << '\n';
	std::cout << "\n";

	return std::move(graphInfo);
}


int main()
{
	size_t verticesCount = GetValueFromUser<std::int64_t>("Eneter vertices count:\n", [](auto &value) { return value > 0; });

	AdjacencyGraph graph(verticesCount);

	std::srand(unsigned(time(0)));

	for (auto &vertexEdgesValues : graph)
	{
		vertexEdgesValues.resize(verticesCount);
		std::generate(vertexEdgesValues.begin(), vertexEdgesValues.end(), []() { return std::rand() % 1000; });
	}

	for (size_t i(0); i < verticesCount; ++i)
		graph[i][i] = 0;

	auto graphInfo = GetGraphInfo(graph);

	std::cout << "Source graph: \n";
	PrintGraph(graph, graphInfo);

	std::cout << "Graph info: " 
		<< "\nRadius = " << graphInfo.radius 
		<< "\nDiameter = " << graphInfo.diameter
	    << "\nCenters id = "; 
	std::copy(graphInfo.centers.cbegin(), graphInfo.centers.cend(), std::ostream_iterator<decltype(*graphInfo.centers.cbegin())>(std::cout, " "));
	std::cout << '\n';

    return 0;
}

