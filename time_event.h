#ifndef SIMULATOR_TIME_EVENT_H_
#define SIMULATOR_TIME_EVENT_H_
#include<string>

class Process;

struct TimeEvent
{
	TimeEvent(double in_event_time, Process* in_customer,std::string event_type);;
	virtual ~TimeEvent();
	double event_time;
	Process *process;
	std::string event_type;
};

struct TimeEventComparator
{
public:
	bool operator()(const TimeEvent* a, const TimeEvent* b) const;
};

#endif /*SIMULATOR_TIME_EVENT_H_ */
