#ifndef SIMULATOR_AGENDA_H_
#define SIMULATOR_AGENDA_H_

#include <set>
#include "time_event.h"

class Agenda
{
public:
	Agenda();
	~Agenda();

	void AddEvent(TimeEvent *tev);
	TimeEvent* RemoveEvent();
	double FirstEventTime();

	void Print();
private:
	std::multiset <TimeEvent*, TimeEventComparator> timeline_;
};

#endif /* SIMULATOR_AGENDA_H_ */
