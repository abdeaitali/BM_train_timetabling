#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <deque>

#include "unroll.h"
#include "pathsearch.h"

using namespace std;








////////////////////////////////////////////////////////////////////////////////
void recursive_unroll(
    const TrainRequest& request,
    const TimePos& sink,
    const Graph<string, int>& path,
    const DurationTable& durations,
    const unordered_map<string, int>& windows,
    Graph<TimePos, float>& network,
    vector<const Node<TimePos, float>*>& ordering,
    deque<const Node<TimePos, float>*>& actives) 
{
    //  nothing to do if there is no node left
    if (actives.empty())
        return;

    //  get first active node
    const Node<TimePos, float>* currNode = actives.front();
    actives.pop_front();

    //  dead node if time limit is reached
    const TimePos& currTimePos = currNode->label();
    const string& currStation = currTimePos.position_;
    const int currWindow = windows.at(currStation);

    if (currTimePos.time_ > currWindow)
        return;

    //  remember node order
    ordering.push_back(currNode);

    //  when reached destination, add free edge to sink and return
    if (currStation == request.to) {
        network.addEdge(currTimePos, sink);
        return;
    }

    //  should stop if next station is destination or requested
    //    assumes existence of currStation key
    const string& nextStation = path.node(currStation)->firstLabelTo();

    bool stopRequested = (   
        (request.to == nextStation) || 
        (find(   // O(n) time
            request.intermediate_stops.begin(),
            request.intermediate_stops.end(),
            nextStation
        ) != request.intermediate_stops.end())
    );

    //  wait (don't wait in first layer. it is constructed according to profit beforehand)
    if (currStation != request.from && 
        currTimePos.state_ == TimePos::State::Stopped) 
    {
        const int t = currTimePos.time_ + TIME_STEP;

        if (t <= currWindow)
        {
            TimePos nextStay(TimePos::State::Stopped, t, currStation, 0);
            bool existed = network.hasNode(nextStay);
            const auto nextNode = network.addNode(nextStay);
            network.addEdge(currTimePos, nextStay); // COST!!

            if (!existed) 
                actives.push_front(nextNode);
        }
    }

    //  get duration table
    //  NOTE:
    //    segfault if nextStation (or currStation for that matter) is empty 
    //    string, which is prevented by earlier 
    //      currStation == request.to 
    //    exit
    const auto& currDurations = durations.at(currStation).at(nextStation);

    //  *-stop
    if (currTimePos.state_ == TimePos::State::Fullspeed ||
        currTimePos.state_ == TimePos::State::Stopped) 
    {
        int t = currTimePos.time_;

        //  full-stop
        if (currTimePos.state_ == TimePos::State::Fullspeed)
            t += currDurations.at(BlockType::FullStop);
        //  stop-stop
        else 
            t += currDurations.at(BlockType::StopStop);
        
        t = nextStep(t, TIME_STEP);
        const float nextWindow = windows.at(nextStation);

        if (t <= nextWindow)
        {
            TimePos nextFullStop(TimePos::State::Stopped, t, nextStation, 0);
            bool existed = network.hasNode(nextFullStop);
            const auto nextNode = network.addNode(nextFullStop);
            network.addEdge(currTimePos, nextFullStop); // cost!!

            if (!existed) 
                actives.push_back(nextNode);
        }
    }

    //  *-full
    //  only applicable if no stop is requested
    if (!stopRequested)
    {
        if (currTimePos.state_ == TimePos::State::Stopped ||
            currTimePos.state_ == TimePos::State::Fullspeed) 
        {
            int t = currTimePos.time_;

            //  full-full
            if (currTimePos.state_ == TimePos::State::Fullspeed)
                t += currDurations.at(BlockType::FullFull);
            //  stop-full
            else
                t += currDurations.at(BlockType::StopFull);

            t = nextStep(t, TIME_STEP);
            const float nextWindow = windows.at(nextStation);

            if (t <= nextWindow)
            {    
                TimePos nextStopFull(TimePos::State::Fullspeed, t, nextStation, 0);
                bool existed = network.hasNode(nextStopFull);
                const auto nextNode = network.addNode(nextStopFull);
                network.addEdge(currTimePos, nextStopFull); // cost!!
        
                if (!existed)
                    actives.push_back(nextNode);
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
void unroll(
    const TrainRequest& request,
    const Graph<string, int>& track,
    const StationTable& stations,
    const DurationTable& durations,
    Graph<TimePos, float>& network,
    vector<const Node<TimePos, float>*>& ordering) 
{
    //  find path from start to goal
    Graph<string, int> path;
    exctractPath(track, request.from, request.to, path);

    //  get upper time bounds
    unordered_map<string, int> windows;
    stationWindows(request, path, durations, windows);

    //  add source and sink nodes
    TimePos source(TimePos::State::Stopped, 0, "START", 0);
    network.addNode(source);

    TimePos sink(TimePos::State::Stopped, 0, "END", 0);
    network.addNode(sink);

    //  spawn initial layer in from-station according to profit function
    deque<const Node<TimePos, float>*> actives;

    for (int t = request.triangle_t1; t <= request.triangle_t3; t += TIME_STEP) 
    {
        TimePos node(TimePos::State::Stopped, t, request.from, 0);
        const auto nodePtr = network.addNode(node);
        network.addEdge(source, node);

        actives.push_front(nodePtr);
    }

    //  iteratively (unrolled recursion using deque) explore possible routes
    ordering.push_back(network.node(source));

    while (!actives.empty()) 
    {
        recursive_unroll(
            request,
            sink,
            path,
            durations,
            windows,
            network,
            ordering,
            actives
        );
    }

    auto sinkNode = network.node(sink);
    ordering.push_back(sinkNode);

    if (sinkNode->inEdges().size() == 0) 
        cerr << "[warning] unroll: no valid path in request time window exists" << endl;
}


////////////////////////////////////////////////////////////////////////////////
// Maps the blocks with cost matrix linearly
void linearBlockMatrixMapping(
    const Graph<string, int>& track,
	const StationTable& stations,
	unordered_map<string, pair<size_t, size_t>>& ids)
{
    size_t id = 0;

	for (const auto& node : track.nodes()){
		ids[node.first] = make_pair(id, stations.at(node.first));
		id++;
	}
        
}


////////////////////////////////////////////////////////////////////////////////
//  assignEdgeCosts
//   this is being separated from unroll as costs change each iteration, while
//   the graph from unroll is fix
void assignEdgeCosts(
    const matf& costs,                          //  (block x steps) cost matrix
	const unordered_map<string, pair<size_t, size_t>>& ids,   //  block -> cost matrix row
    const TrainRequest& request,
    Graph<TimePos, float>& network)
{
    //  assign each edges' cost
    for (auto& fromkv : network.nodes()) 
    {
        auto from = &fromkv.second;

        for (auto& edge: from->outEdges()) 
        {
            auto to = edge.first;

            const int t1 = nextStep(from->label().time_, TIME_STEP);
            const int t2 = nextStep(to->label().time_, TIME_STEP);

            const int col1 = t1 / TIME_STEP;
            const int col2 = t2 / TIME_STEP;

            const string& pos = from->label().position_;

            //  normal nodes get costs from matrix
            if (pos != "START") 
            {
                const auto& id = ids.find(pos);
                
                assert (id != ids.end());

                const size_t row = id->second.first;

				// capacity usage cost minus capacity
				edge.second = costs.colSum(row, col1, col2) - id->second.second; 
            //  first layer nodes get assigned negative profit
            } 
            else { // if Source Node
                edge.second = -request.profit(t2);
            }

            //  edges to the virtual end node have no costs for now
            if (to->label().position_ == "END") 
                edge.second = 0.0;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
void stationWindows(
    const TrainRequest& request,
    const Graph<string, int>& path,
    const DurationTable& durations,
    unordered_map<string, int>& windows)
{
    const Node<string, int>* curr = path.node(request.to);
    assert(curr != nullptr);
    const Node<string, int>* prev = curr->firstFrom();

    int t = request.window_t2;
    windows[curr->label()] = t;

    while (curr != nullptr && prev != nullptr) 
    {
        const auto& durs = durations.at(curr->label()).at(prev->label());
        float dt = 0.0f;

        if (curr->label() == request.to)
            dt = durs.at(BlockType::FullStop);
        else
            dt = durs.at(BlockType::FullFull);

        t -= dt;

        windows[prev->label()] = t;

        curr = prev;
        prev = curr->firstFrom();
    }

    // cout << "START, " << request.triangle_t2 << endl;
    // const Node<string, int>* node = path.node(request.from);

    // while (node != nullptr) {
    //     cout << node->label() << ", " << windows[node->label()] << endl;
    //     node = node->firstTo();
    // }
}





























