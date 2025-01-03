#include <vector>
#include <mpi.h>
#include <array>
#include <unordered_map>

#include "master.h"
#include "slave.h"
#include "csvreader.h"
#include "mpi_constants.h"


int world_rank, world_size;

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    //  read request
    bool succ;
    vector<TrainRequest> requests;
    succ = readTrainRequests("../data/sample_input.csv", requests);
    assert(succ);
    
    if (world_rank == 0) {
        master(requests);
    } else {
        slave(requests);
    }
    MPI_Finalize();
	cout << "Main program executed successfully!" << endl;
    return (EXIT_SUCCESS);
}