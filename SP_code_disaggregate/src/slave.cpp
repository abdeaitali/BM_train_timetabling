#include <mpi.h>
#include <unordered_map>
#include <iostream>
#include <array>

#include "slave.h"
#include "worker.h"

using namespace std;

extern int world_rank;
extern int world_size;
extern int TERMINATE;
extern int JOB_ASSIGN_TAG;
extern int JOB_DONE_TAG;
extern int JOB_RESULT_TAG;

void slave(
           const vector<TrainRequest>& requests
           )
{
    Worker w;
    
    // go into slave loop
    int currentJobID = -1;
    MPI_Recv(&currentJobID, 1, MPI_INT, 0, JOB_ASSIGN_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    
    while (currentJobID != TERMINATE) {
        //cout << "[slave=" << world_rank << "] " << currentJobID << endl;
        
        vector<array<int, 2 >> data = w.handleTrain(requests[currentJobID]);
        int result_size = data.size();
        
        MPI_Send(&result_size, 1, MPI_INT, 0, JOB_DONE_TAG, MPI_COMM_WORLD);
        MPI_Send(&data[0], 2 * data.size(), MPI_INT, 0, JOB_RESULT_TAG, MPI_COMM_WORLD);
        
        MPI_Recv(&currentJobID, 1, MPI_INT, 0, JOB_ASSIGN_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    cout << "[slave=" << world_rank << "] " << "TERMINATE" << endl;
}