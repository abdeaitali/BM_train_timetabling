#include <iostream>
#include <cassert>
#include <functional>
#include <unordered_set>

#include "common.h"
#include "graph.h"
#include "csvreader.h"
#include "matrix.h"
#include "pathsearch.h"
#include "unroll.h"
#include "timer.h"
#include "dagsp.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) 
{
	Timer timer;

    cout << "read data from files ... ";
    timer.tic();

    //  read duration table
    bool succ = false;
    DurationTable durations;
    succ = readDurationTable("../data/type_train2.csv", durations);
    assert(succ);

    //  read StationTable
    StationTable stations;
    succ = readStationTypes("../data/stations.csv", stations);
    assert(succ);

	//  read track graph
    Graph<string, int> track;
    succ = readTrackGraph("../data/tracks.csv", track);
    assert(succ);

	//  read sample request
    vector<TrainRequest> requests;
    succ = readTrainRequests("../data/sample_input.csv", requests);
    assert(succ);

    //  generate cost matrix
    matf costs = matf::rnd(24, 24*60*2, 0.0001, 1);

	// mapping cost to tracks
    unordered_map<string, size_t> ids;
    linearBlockMatrixMapping(track, ids);
    cout << " OK [" << timer.tac() << "s]" << endl;

    // unroll
    cout << "unroll graph ...";
    timer.tic();

	int nbRequests = requests.size();
    vector<Graph<TimePos, float>> network(nbRequests);
    vector<vector<const Node<TimePos, float>*>> ordering(nbRequests);
	for (int i = 0; i < nbRequests; ++i)
	    unroll(requests[i], track, stations, durations, network[i], ordering[i]);
	cout << " OK [" << timer.tac() << "s]" << endl;

    //  assign costs
    cout << "assign edge costs ...";
    timer.tic();
	for (int i = 0; i < nbRequests; ++i)
		assignEdgeCosts(costs, ids, requests[i], network[i]);
    
    cout << " OK [" << timer.tac() << "s]" << endl;

    //  shortest path
    cout << "compute shortest paths ...";
    timer.tic();

    vector<unordered_map<const Node<TimePos, float>*, const Node<TimePos, float>*>> paths(nbRequests);
	vector<float> Phi(nbRequests);
	for (int i = 0; i < nbRequests; ++i)
	{
		dagsp(network[i], ordering[i], paths[i], Phi[i]);
		cout << Phi[i] << endl;
	}
    cout << "OK [" << timer.tac() << "s]" << endl;


    //  info
    cout << endl;
	for (size_t i = 0; i < nbRequests; i++)
	{
		cout << "network size: " << network[i].numNodes() << endl;
	}

	// ok
    return EXIT_SUCCESS;
}