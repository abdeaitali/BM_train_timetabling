#include <iostream>

#include "trainrequest.h"

using namespace std;

// Computes the profit of departing at certain time
double TrainRequest::profit(int time) const 
{
    if (time < triangle_t1 || time > triangle_t3) {
        return 0;
    }
    
    if (time < triangle_t2) {
        return triangle_v * (time - triangle_t1) / (float)(triangle_t2 - triangle_t1);
    }
    
    return triangle_v - (triangle_v * (time - triangle_t2) / (float)(triangle_t3 - triangle_t2));    
}


// Printing the request info
ostream& operator << (
    ostream& out, 
    const TrainRequest& tr)
{
    out << "TrainRequest {";
    out << "train_id=" << tr.train_id;
    out << ", train_type=" << tr.train_type;
    out << ", from=" << tr.from;
    out << ", to=" << tr.to;
    out << ", t1=" << tr.triangle_t1;
    out << ", t2=" << tr.triangle_t2;
    out << ", t3=" << tr.triangle_t3;
    out << ", v=" << tr.triangle_v;
    out << ", t1=" << tr.window_t1;
    out << ", t2=" << tr.window_t2;
    out << ", stops=" << tr.intermediate_stops.size();
    out << "}";

    return out;
}