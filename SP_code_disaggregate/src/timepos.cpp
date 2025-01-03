#include "timepos.h"
#include <string>

using namespace std;

TimePos::TimePos(TimePos::State state, int time, std::string position, int waitTimeRemaining) :
    state_(state),
    time_(time),
    position_(position),
    waitTimeRemaining_(waitTimeRemaining)
{}

bool TimePos::operator == (const TimePos& o) const {
    return state_ == o.state_ &&
        time_ == o.time_ &&
        position_ == o.position_ &&
        waitTimeRemaining_ == o.waitTimeRemaining_;
}

ostream& operator << (ostream& out, const TimePos& tp) 
{
    out << "{";
    out << (tp.state_ == TimePos::Fullspeed ? "F" : "S" );
    out << ", t = " << tp.time_;
    out << ", p = " << tp.position_;
    out << ", dt = " << tp.waitTimeRemaining_;
    out << "}";
    
    return out;
}