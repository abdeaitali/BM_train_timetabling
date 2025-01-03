#ifndef DAGSP_H
#define DAGSP_H

#include <limits>
#include <algorithm>
#include <cassert>
#include <unordered_map>

#include "graph.h"
#include "node.h"


static const double infd = std::numeric_limits<double>::infinity();

template <class N, typename C>
using ndmap = std::unordered_map<const Node<N, C>*, double>;

template <class N, typename C>
using nnmap = std::unordered_map<const Node<N, C>*, const Node<N, C>*>;


////////////////////////////////////////////////////////////////////////////////
template <class N, typename C>
void relax_node(
    const Node<N, C>* node, 
    ndmap<N, C>& costs, 
    nnmap<N, C>& predecessors) 
{    
    for (auto it : node->outEdges()) 
    {
        if (costs[node] + it.second < costs[it.first]) {    // it.first is dest node, 
                                                            // it.second is cost to dest node 
            costs[it.first] = costs[node] + it.second;
            predecessors[it.first] = node;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
template <class N, typename C>
C dagsp(
    const Graph<N, C>& g, 
    const std::vector<const Node<N, C>*>& sorting,
    nnmap<N, C>& predecessors)
{    
    assert (g.numNodes() == sorting.size());
    
    //  initialise costs
    ndmap<N, C> costs;
	
    for(auto it : sorting)
        costs[it] = infd;

    costs[sorting[0]] = 0.0;
    
    //  update distances in topological order
	for (auto it : sorting)
		relax_node(it, costs, predecessors);
	
	return costs.at(sorting[sorting.size() - 1]);

}


////////////////////////////////////////////////////////////////////////////////
template <class N, typename C>
void evaluatePath(
	const Node<N, C>* sink,
	const unordered_map<const Node<TimePos, float>*, const Node<TimePos, float>*>& predecessors,
	const unordered_map<string, pair<size_t, size_t>> &ids,
	int T,
	int8_T* subGrad
	)
{
	auto pred = sink;
	auto curr = predecessors.find(sink);
	while (curr != predecessors.end())
	{
		// get the times
		const int t1 = nextStep(curr->second->label().time_, TIME_STEP);
		const int t2 = nextStep(pred->label().time_, TIME_STEP);

		const int col1 = t1 / TIME_STEP;
		const int col2 = t2 / TIME_STEP;

		//mexPrintf("%d (%d)--> %d (%d)\n", col1, t1, col2, t2);

		// the source is reached
		if (t1 == 0)
			return;

		// get the block
		const string& pos = curr->second->label().position_;
		const auto& id = ids.find(pos);
		const auto b = id->second.first;

		// Subgradient
		for (size_t i = col1; i <= col2 ; i++)
		{
			subGrad[b*T + i] += 1;
		}

		// next precedent
		pred = curr->second;
		curr = predecessors.find(pred);
	}

}


#endif