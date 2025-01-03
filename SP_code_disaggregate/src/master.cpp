#include <mpi.h>
#include <unordered_map>
#include <iostream>
#include <array>

#include "master.h"

using namespace std;

extern int world_rank;
extern int world_size;
extern int TERMINATE;
extern int JOB_ASSIGN_TAG;
extern int JOB_DONE_TAG;
extern int JOB_RESULT_TAG;

void master(
            const vector<TrainRequest>& requests
            )
{
    // master
    unordered_map<int, vector<array<int, 2 >> > result;
    const int MAX_JOB_ID = requests.size();
    
    int jobId = 0;
    unordered_map<int, int> jobMap;
    for (int slave = 1; slave < world_size; ++slave) {
        if (jobId < MAX_JOB_ID) {
            MPI_Send(&jobId, 1, MPI_INT, slave, JOB_ASSIGN_TAG, MPI_COMM_WORLD);
            jobMap[slave] = jobId;
            ++jobId;
        } else {
            MPI_Send(&TERMINATE, 1, MPI_INT, slave, JOB_ASSIGN_TAG, MPI_COMM_WORLD);
        }
    }
    
    while (jobMap.size() > 0) {
        MPI_Status status;
        int job_size = -1;
        MPI_Recv(&job_size, 1, MPI_INT, MPI_ANY_SOURCE, JOB_DONE_TAG, MPI_COMM_WORLD, &status);
        int slave = status.MPI_SOURCE;
        int result_job_id = jobMap[slave];
        vector<array<int, 2 >> data(job_size);
        MPI_Recv(&data[0], 2 * job_size, MPI_INT, slave, JOB_RESULT_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        result[result_job_id] = data;
        jobMap.erase(slave);
        
        if (jobId < MAX_JOB_ID) {
            MPI_Send(&jobId, 1, MPI_INT, slave, JOB_ASSIGN_TAG, MPI_COMM_WORLD);
            jobMap[slave] = jobId;
            ++jobId;
        } else {
            MPI_Send(&TERMINATE, 1, MPI_INT, slave, JOB_ASSIGN_TAG, MPI_COMM_WORLD);
        }
    }
}