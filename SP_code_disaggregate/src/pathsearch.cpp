#include <queue>
#include <utility>
#include "pathsearch.h"


////////////////////////////////////////////////////////////////////////////////
bool bfs(
    const Graph<string, int>& graph, 
    const nodesi* from, 
    const nodesi* to,
    unordered_map<nodesi*, nodesi*>& preds) 
{    
    queue <nodesi*> q;
    q.push(from);

    while (!q.empty()) 
    {
        auto n = q.front();
        q.pop();

        for (auto e : n->outEdges()) 
        {
            if (preds.find(e.first) == preds.end()) 
            {
                preds[e.first] = n;
                q.push(e.first);

                if (e.first->id() == to->id())
                    return true;
            } 
        }
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////
bool exctractPath(
    const Graph<string, int>& track, 
    const string& fromLabel, 
    const string& toLabel,
    Graph<string, int>& path) 
{
	const nodesi* from = track.node(fromLabel);
	const nodesi* to = track.node(toLabel);

    if (to == nullptr || from == nullptr)
        return false;

	// construct the precedency path from->to
    unordered_map<nodesi*, nodesi*> preds;
    preds[from] = nullptr;
	bool res = bfs(track, from, to, preds);

	// check if such a prec path was found
    if (!res)
        return false;

	// construct the direct path
    nodesi* node = to;
	nodesi* pred = preds.at(node);
    path.addNode(node->label());
    while(node != nullptr && pred != nullptr) 
    {
        const int dist = pred->edgeTo(node);
        path.addNode(pred->label());
        path.addEdge(pred->label(), node->label(), dist);

        node = pred;
        pred = preds.at(node);
    } 
    
    return true;
}