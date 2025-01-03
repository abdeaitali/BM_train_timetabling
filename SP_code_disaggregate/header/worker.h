#ifndef WORKER_H
#define WORKER_H

#include <vector>
#include <array>

#include "csvreader.h"
#include "graph.h"
#include "timepos.h"
#include "unroll.h"
#include "matrix.h"
#include "dagsp.h"


using namespace std;

class Worker {
public:
    vector<array<int, 2 >> handleTrain(
                                       const TrainRequest& train
                                       );
private:
    unordered_map<string, DurationTable> durationTables;
    StationTable stations;
    Graph<string, int> trackGraph;
    matf costs;
    
};



#endif