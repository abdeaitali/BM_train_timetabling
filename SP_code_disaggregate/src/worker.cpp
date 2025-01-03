#include "worker.h"
#include "timer.h"

extern int world_rank;


vector<array<int, 2 >> Worker::handleTrain(
                                   const TrainRequest& train
                                   )
{   
    cout << "[slave="<< world_rank<<"] starting train: " << train.train_id << endl;
    
    Timer timer;
    timer.tic();
    
	//  read duration table
    bool succ = false;
    if(durationTables.find(train.train_type) == durationTables.end()) {
    	DurationTable durs;
        succ = readDurationTable("../data/type_" + train.train_type + ".csv", durs);
        durationTables[train.train_type] = durs;
    	assert(succ);
    }
    


    //  read StationTable
    if(stations.size() == 0) {
        succ = readStationTypes("../data/stations.csv", stations);
        assert(succ);
    }


    //  read track graph
    if(trackGraph.numNodes() == 0) {
        succ = readTrackGraph("../data/tracks.csv", trackGraph);
        assert(succ);
    }

    
	// unroll
    vector<const Node<TimePos, float>*> topoSort;
    Graph<TimePos, float> trainGraph;
    unroll(train, trackGraph, stations, durationTables[train.train_type], trainGraph, topoSort);

    //  random cost matrix
    costs = matf::rnd(100, 24 * 60 * 2, 0, 1); //  block x discretised time
    unordered_map<string, size_t> ids;
    linearBlockMatrixMapping(trackGraph, ids);
    assignEdgeCosts(costs, ids, train, trainGraph);
    
    // shortest path
    auto end = topoSort.back();
    unordered_map<const Node<TimePos, float>*, const Node<TimePos, float>*> predecessors;
    dagsp(trainGraph, topoSort, predecessors);
    
    
	vector<array<int, 2 >> result;
    const int N = 5;
    for (int i = 0; i < N; ++i) {
        array<int, 2 > e;
        for (int j = 0; j < 2; ++j) {
            e.at(j) = train.train_id * (i + 1)*(j + 1);
        }
        result.push_back(e);
    }
    
    cout << "[slave="<< world_rank<<"] completed, nodes: " << trainGraph.numNodes() << " time: " << timer.tac() <<endl;
    
    return result;
}