#ifndef UNROLL_H
#define UNROLL_H

#include <unordered_map>

#include "trainrequest.h"
#include "graph.h"
#include "timepos.h"
#include "csvreader.h"
#include "matrix.h"

using namespace std;


#define TIME_STEP 30

////////////////////////////////////////////////////////////////////////////////
inline int nextStep(const int t, const int step = TIME_STEP) {
	return ceilf((float)t / step) * step;
}


void unroll(
    const TrainRequest& request,
    const Graph<string, int>& track,
    const StationTable& stations,
    const DurationTable& durations,
    Graph<TimePos, float>& network,
    vector<const Node<TimePos, float>*>& ordering
);

// Maps the blocks with cost matrix linearly
void linearBlockMatrixMapping(
    const Graph<string, int>& track,
	const StationTable& stations,
	unordered_map<string, pair<size_t, size_t>>& ids
);


void assignEdgeCosts(
    const matf& costs,
	const unordered_map<string, pair<size_t, size_t>>& ids,
    const TrainRequest& request,
    Graph<TimePos, float>& network
);


void stationWindows(
    const TrainRequest& request,
    const Graph<string, int>& track,
    const DurationTable& durations,
    unordered_map<string, int>& windows
);


#endif