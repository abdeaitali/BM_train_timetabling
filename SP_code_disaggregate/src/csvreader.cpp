#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <unordered_map>

#include "csvreader.h"


static const unordered_map<string, LocationType> str2loc = {
    {"dp", DP},
    {"mblsi", MBLSI}
};


////////////////////////////////////////////////////////////////////////////////
// Reads the data table with duration of (SS, SF, FS, FF)
bool readDurationTable(
    const string& filename,
    DurationTable& table)
{
    ifstream fin(filename);

    if (!fin.is_open())
        return false;

    string from, to, line, dur;

    getline(fin, line);  //  read header

    while (fin)
    {
        if (!getline(fin, from, ';')) 
            break;

        if (!getline(fin, to, ';')) 
            return false;

        if (!getline(fin, line, ';')) 
            return false;

        if (!getline(fin, dur, ';')) 
            return false;

        table[from][to][StopStop] = stoi(dur);

        if (!getline(fin, dur, ';')) 
            return false;

        table[from][to][StopFull] = stoi(dur);    

        if (!getline(fin, dur, ';')) 
            return false;

        table[from][to][FullStop] = stoi(dur);    

        if (!getline(fin, dur)) 
            return false;

        table[from][to][FullFull] = stoi(dur);
    }

    fin.close();
    return true;
}


////////////////////////////////////////////////////////////////////////////////
// Reads the requests data (trainId, type, from, to, triangle, window)
bool readTrainRequests(
    const string& filename,
    vector<TrainRequest> &result)
{
    ifstream fin(filename);
    
    if (!fin.is_open())
        return false;
    
    string header;
    getline(fin, header);  //  read header
    
    string train_id;
    string triangle_t1, triangle_t2, triangle_t3, triangle_v;
    string window_t1, window_t2;
    string train_type, from, to, intermediate_stops;
    
    while (fin)
    {
        if (!getline(fin, train_id, ';'))
            break;
        
        if (!getline(fin, train_type, ';'))
            return false;
        
        if (!getline(fin, from, ';'))
            return false;
        
        if (!getline(fin, to, ';'))
            return false;
        
        if (!getline(fin, triangle_t1, ';'))
            return false;
        
        if (!getline(fin, triangle_t2, ';'))
            return false;
        
        if (!getline(fin, triangle_t3, ';'))
            return false;
        
        if (!getline(fin, triangle_v, ';'))
            return false;
        
        if (!getline(fin, window_t1, ';'))
            return false;
        
        if (!getline(fin, window_t2, ';'))
            return false;
        
        if(!getline(fin, intermediate_stops))
            intermediate_stops = "";
        
        TrainRequest tr;
        tr.train_id = stoi(train_id);
        tr.train_type = train_type;
        tr.from = from;
        tr.to = to;
        tr.triangle_t1 = stoi(triangle_t1);
        tr.triangle_t2 = stoi(triangle_t2);
        tr.triangle_t3 = stoi(triangle_t3);
        tr.triangle_v = stoi(triangle_v);
        tr.window_t1 = stoi(window_t1);
        tr.window_t2 = stoi(window_t2);

        if(!(intermediate_stops == "")) 
        {
            string delimiter = ",";
            size_t pos = 0;
            string token;

            while ((pos = intermediate_stops.find(delimiter)) != std::string::npos) {
                token = intermediate_stops.substr(0, pos);
                tr.intermediate_stops.push_back(token);
                intermediate_stops.erase(0, pos + delimiter.length());
            }

            token = intermediate_stops.substr(0, pos);
            tr.intermediate_stops.push_back(token);
        }

        result.push_back(tr);
    }
    
    
    fin.close();
    return true;
}


////////////////////////////////////////////////////////////////////////////////
// Reads the stations data (name, capacity, location type)
bool readStationTypes(
    const string& filename,
    StationTable& table)
{
    ifstream fin(filename);

    if (!fin.is_open())
        return false;

    string name, cap, type, offset;

    getline(fin, offset);  //  read header

    while (fin)
    {
        if (!getline(fin, name, ';')) 
            break;

        if (!getline(fin, cap, ';')) 
            return false;

        if (!getline(fin, type, ';')) 
            return false; 

        if (!getline(fin, offset)) 
            return false; 

		table[name] = stoi(cap); // capacity in the station
    }

    return true;
}


////////////////////////////////////////////////////////////////////////////////
// Reads the tracks data (track id, start, end, line, distance)
bool readTrackGraph(
    const string& filename,
    Graph<string, int>& graph)
{
    ifstream fin(filename);

    if (!fin.is_open())
        return false;

    string id, start, end, line, dist;

    getline(fin, line);  // read header

    while (fin)
    {
        if (!getline(fin, id, ';')) 
            break;

        if (!getline(fin, start, ';')) 
            return false;

        if (!getline(fin, end, ';')) 
            return false; 

        if (!getline(fin, line, ';')) 
            return false; 

        if (!getline(fin, dist)) 
            return false; 

        graph.addNode(start);//start
        graph.addNode(end);//end

        if (!graph.addEdge(start, end, stoi(dist)))//distance
            return false;
    }

    return true;
}


////////////////////////////////////////////////////////////////////////////////
// Printing the train durations info
ostream& operator << (ostream& out, const DurationTable& table) 
{
    for (const auto& pssti : table) 
    {
        for (const auto& psti : pssti.second)
        {
            out << pssti.first << ", " << psti.first;

            for (const auto& pti : psti.second) 
                out << ", " << pti.second;

            out << endl;
        }
    }

    return out;
}


////////////////////////////////////////////////////////////////////////////////
// Printing the stations info
ostream& operator << (ostream& out, const StationTable& table) 
{
    for (const auto& slp : table) 
        out << slp.first << ": " << slp.second << endl;

    return out;
}