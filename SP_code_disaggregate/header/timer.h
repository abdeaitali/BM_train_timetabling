#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <chrono>

using namespace std;


class Timer
{
    public:

        Timer();        //  start timer

        void tic();     //  start or reset the timer
        double tac();   //  return seconds since start
        double toc();   //  print and return seconds since start

    private:

        chrono::high_resolution_clock::time_point then_;
};


#endif
