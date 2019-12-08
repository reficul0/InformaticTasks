// Task5.Graph.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <list>
#include <tuple>

using EdgeValueT = std::int64_t;
using AdjacencyGraph = std::vector<std::vector<EdgeValueT>>;

template<typename GraphT>
struct GraphInfo
{
	EdgeValueT radius, diameter;
	std::list<size_t> centers;
};
template<typename GraphT>
GraphInfo<GraphT> GetGraphInfo(GraphT &&graph)
{
	using VertexMaxAndMinEdgesT = std::pair<EdgeValueT, EdgeValueT>;
	std::vector< VertexMaxAndMinEdgesT > VertexesMaxAndMinEdges;

	auto getMinMaxElements = [](auto begin, auto end)
	{
		auto const &minMaxIters = std::minmax_element(begin, end);
		return std::make_pair(*minMaxIters.first, *minMaxIters.second);
	};

	std::transform(graph.begin(), graph.end(), std::back_inserter(VertexesMaxAndMinEdges), [getMinMaxElements](auto &vertexEdgesValues)
	{
		return getMinMaxElements(vertexEdgesValues.begin(), vertexEdgesValues.end());

		auto const &minMaxIters = std::minmax_element(begin, end);
		return std::make_pair(*minMaxIters.first, *minMaxIters.second);
	});

	GraphInfo<GraphT> graphInfo;
	std::tie(graphInfo.radius, graphInfo.diameter) = getMinMaxElements(VertexesMaxAndMinEdges.begin(), VertexesMaxAndMinEdges.end());

	std::copy_if(VertexesMaxAndMinEdges.begin(), VertexesMaxAndMinEdges.end(), std::back_inserter(graphInfo.centers), [graphInfo](const auto &vertexMinMaxEdgeVal){
		return vertexMinMaxEdgeVal.first == graphInfo.radius;
	});
	for (const auto &vertexMinMaxEdgeVal : VertexesMaxAndMinEdges)
		if (vertexMinMaxEdgeVal.first == graphInfo.radius)
			graphInfo.centers.push_back(vertexMinMaxEdgeVal.first);

	return std::move(graphInfo);
}

int main()
{
	AdjacencyGraph graph = { 
		{ 1,1 }, // it means 0,1,1
		{ 1,3 }, //			 1,0,3
		{ 1,2 }	 //			 1,2,0
	};

	auto graphInfo = GetGraphInfo(graph);

    return 0;
}

