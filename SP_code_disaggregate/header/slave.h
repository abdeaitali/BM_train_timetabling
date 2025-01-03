#ifndef SLAVE_H
#define SLAVE_H

#include <vector>
#include "trainrequest.h"

using namespace std;

void slave(
           const vector<TrainRequest>& requests
           );

#endif