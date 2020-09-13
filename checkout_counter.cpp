#include "checkout_counter.h"
#include "stats.h"
#include "agenda.h"
#include "customer.h"
#include "time_event.h"
#include <random>


CheckoutCounter::CheckoutCounter(int id, supermarket *sm, Agenda *a) :Process(sm, a), served_customer_(nullptr), kID_(id), dis_T_p(60,120),
dis_T_susp(5,20),gen(rd())
{
	state_ = Available;
	is_On_Break = false;
	wants_to_go_on_break = false;
}
void CheckoutCounter::Execute(double current_time)
{
	bool active = true;
	double temp_time;


	while (active)
	{
		switch (state_)
		{
		case Available:
			logger_->Print("Checkout Counter is available", Logger::L2);
			temp_time = current_time + dis_T_p(gen);
			state_ = Not_Available;
			active = false;
			Activate(temp_time,"CheckoutCounter not available");
			is_On_Break = false;
			wants_to_go_on_break = false;
			break;
		case Not_Available:
			logger_->Print("CheckoutCounter is not available", Logger::L2);

			double service_time = current_time + dis_T_susp(gen);
			if (IsEmpty()&&!is_On_Break)
				{		
					is_On_Break = true;
					state_ = Available;
					Activate(service_time,"CheckoutCounter Available");
					active = false;
				}
			else
			{
					wants_to_go_on_break = true;				
					state_ = Not_Available;
					Activate(service_time
						
						, "CheckoutCounter not available");
					active = false;
					
				}
			break;

		}
	}
}

bool CheckoutCounter::IsTerminated() const
{
	return terminated_;
}

void CheckoutCounter::Activate(double event_time,std::string event_type)
{
	agenda_->AddEvent(new TimeEvent(event_time, this,event_type));
}

CheckoutCounter::~CheckoutCounter()
{
	if (served_customer_ != nullptr)
		delete served_customer_;
}

bool CheckoutCounter::IsEmpty()
{
	return (served_customer_ == nullptr);
}

void CheckoutCounter::AssignCustomer(Customer* c)
{
	served_customer_ = c;
}

Customer* CheckoutCounter::RemoveCustomer()
{
	Customer *c = served_customer_;
	served_customer_ = nullptr;
	return c;
}

int CheckoutCounter::getID()
{
	return kID_;
}

bool CheckoutCounter::cc_wants_a_break()
{
	return wants_to_go_on_break;
}

