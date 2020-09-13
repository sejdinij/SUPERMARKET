#ifndef SIMULATOR_SIMULATOR_H_
#define SIMULATOR_SIMULATOR_H_
#include "logger.h"

class Agenda;
struct supermarket;

class Simulator
{
public:
	Simulator(double stop_time, int nrof_service_desks, int nrof_cards,double lamb);
	~Simulator();

	void Initialize(bool step_by_step, Logger::LogVerbosity verb);

	void Start();

private:
	double lambda;
	
	const double kStopTime_;
	const int kNrofCheckoutCounter_;
	const int kNrofShoppingCards_;
	

	double current_time_;
	bool step_by_step_;

	supermarket *s_market_;
	Agenda *agenda_;
};

#endif /* SIMULATOR_SIMULATOR_H_ */
