#ifndef PATHSEARCH_H
#define PATHSEARCH_H

#include <iostream>
#include <unordered_map>

#include "csvreader.h"
#include "graph.h"

using namespace std;


bool exctractPath(
    const Graph<string, int>& graph, 
    const string& fromLabel, 
    const string& toLabel,
    Graph<string, int>& path
);


#endif