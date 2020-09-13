#include <cstdlib>
#include <ctime>
#include "simulator.h"
#include "supermarket.h"
#include "customer.h"
#include "agenda.h"
#include <iostream>
using namespace std;

Simulator::Simulator(double stop_time, int nrof_ser_desks, int nrof_cards,double lam) : kStopTime_(stop_time),
kNrofCheckoutCounter_(nrof_ser_desks),
kNrofShoppingCards_(nrof_cards),lambda(lam)
{
	s_market_ = nullptr;
	
}

Simulator::~Simulator()
{
	delete s_market_;
}

void Simulator::Initialize(bool step_by_step, Logger::LogVerbosity verb)
{
	agenda_ = new Agenda();
	s_market_ = new supermarket(kNrofCheckoutCounter_, kNrofShoppingCards_, agenda_,lambda);
	current_time_ = 0;
	step_by_step_ = step_by_step;
	Logger::GetInstance()->SetLogVerbosity(static_cast<Logger::LogVerbosity>(verb));
}

void Simulator::Start()
{
	Process *process_;

	(new Customer(s_market_, agenda_))->Activate(current_time_,"New Customer");

	while (current_time_ <= kStopTime_)
	{


		auto event = agenda_->RemoveEvent();
		current_time_ = event->event_time;

		Logger::GetInstance()->Print(("\n----------------------------------------------------------------------\n"), Logger::L3);
		Logger::GetInstance()->Print(("CLOCK = " + to_string(current_time_) + "\n"), Logger::L3);

		process_ = event->process;

		process_->Execute(current_time_);

		agenda_->Print();

		delete event;

		if (process_->IsTerminated()) delete process_;

		if (step_by_step_)
		{
			cout << "Press ENTER to continue...";
			string l; getline(cin, l);		}
	}
}
