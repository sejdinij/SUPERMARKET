#include "agenda.h"
#include "time_event.h"
#include <cassert>
#include <iostream>

Agenda::Agenda()
{
}

Agenda::~Agenda()
{
	for (auto val : timeline_)
		delete val;
}

void Agenda::AddEvent(TimeEvent* tev)
{
	timeline_.insert(tev);
}

TimeEvent* Agenda::RemoveEvent()
{
	assert(timeline_.size() > 0 && "Agenda::RemoveEvent(): No more events to simulate!");

	TimeEvent* res = *timeline_.begin();
	timeline_.erase(timeline_.begin());
	return res;
}

double Agenda::FirstEventTime()
{
	assert(timeline_.size() > 0 && "Agenda::FirstEventTime(): No more events to simulate!");

	return (*timeline_.begin())->event_time;
}

void Agenda::Print()
{
	std::cout << "\n------------------------------------\n";
	for (auto v : timeline_)
	{
		std::cout << "[ " << v->event_time<<" - "<<v->event_type << " ]" << " ";
	}
	std::cout << "\n------------------------------------";
}
