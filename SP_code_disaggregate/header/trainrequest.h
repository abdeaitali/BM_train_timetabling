#ifndef TRAINREQUEST_H
#define TRAINREQUEST_H

#include <string>
#include <vector>


class TrainRequest  
{
	// Members
    public:

        int train_id; // train id
        std::string train_type; // type
        std::string from; // starting station
        std::string to; // terminal station
        int triangle_t1; // earliest departure time
        int triangle_t2; // latest departure time
        int triangle_t3; // ideal departure time
        int triangle_v; // value coefficient
        int window_t1; // earliest arrival time
        int window_t2; // latest arrival time
        std::vector<std::string> intermediate_stops; // intermediate stops
        
	// Methods

        // Printing method (as friend, to have access to the class members)
        friend std::ostream& operator << (
            std::ostream& out, 
            const TrainRequest& tr
        );
    
		// Computes the profit of departing at certain time
		double profit(int time) const;
};


#endif