#include "time_event.h"
#include <string>;

TimeEvent::TimeEvent(double in_event_time, Process* in_process,std::string e_t) : event_time(in_event_time),
process(in_process), event_type(e_t)
{
}

TimeEvent::~TimeEvent()
{
}

bool TimeEventComparator::operator()(const TimeEvent* a, const TimeEvent* b) const
{
	return a->event_time < b->event_time;
}