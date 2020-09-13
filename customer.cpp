#include "customer.h"
#include "supermarket.h"
#include "checkout_counter.h"
#include "time_event.h"
#include "agenda.h"
#include "logger.h"
#include "stats.h"
#include <random>	
#include <queue>
#include <iostream>
using namespace std;

int Customer::global_id_ = 0;

Customer::Customer(supermarket *sm, Agenda *a) :Process(sm, a), kID_(global_id_++),
state_(New),
checkout_counter_idx_(-1), dis_T_arrival(supermarket_->lambd), dis_T_checkout(1,7),dis_T_shopping(10, 60), gen(rd())
{
	 
	logger_ = Logger::GetInstance();
}


Customer::~Customer()
{
}

int Customer::getID() const
{
	return kID_;
}

void Customer::Execute(double current_time)
{
	
	bool active = true;
	while (active)
	{
		switch (state_)
		{
		
		case New:
			logger_->Print("New customer arrived", Logger::L3);
			active = HandleNewState(current_time);			
			break;
		case Start_Shopping:
			logger_->Print("Customer started shopping", Logger::L3);
			active = StartShopping(current_time);
			break;
		case Shopping_Finished:
			logger_->Print("Customer finished shopping", Logger::L3);
			active = ShoppingFinished(current_time);
			break;
		case ServiceStarted:
			logger_->Print("Customer start service at checkout counter", Logger::L3);
			active = HandleServiceStartedState(current_time);
			state_ = ServiceCompleted;
			break;
		case ServiceCompleted:
			logger_->Print("Customer finishes service at checkout counter", Logger::L3);
			active = HandleServiceCompletedState(current_time);
			terminated_ = true; 
			break;
		}
	}
}

bool Customer::IsTerminated() const
{
	return terminated_;
}

void Customer::Activate(double event_time, std::string event_type)
{
	agenda_->AddEvent(new TimeEvent(event_time, this,event_type));
}

bool Customer::StartShopping(double current_time)
{
	logger_->Print("        - customer " + to_string(kID_)
	+ " gets a shopping card and starts shopping", Logger::L4);
	if (stats::initial_phase <= current_time)
	{
		stats::ave_card_queue_length += (current_time - stats::last_time_queue_modified)*supermarket_->shopping_cards_queue.size();
		stats::last_time_queue_modified = current_time;
	}
	supermarket_->shopping_cards_queue.pop();
	if (stats::initial_phase <= current_time)
	{
		customer_arrive_time = current_time;
	}
	supermarket_->shopping_card_size--;
	int end_of_shopping_time = current_time + dis_T_shopping(gen);
	logger_->Print("        - customer " + to_string(kID_)
		+ " will finish shopping at  " + to_string(end_of_shopping_time), Logger::L4);
	Activate(end_of_shopping_time,"End of Shopping");
	state_ = Shopping_Finished;
	return false;
}


bool Customer::ShoppingFinished(double current_time)
{
	if (stats::initial_phase <= current_time)
	{
		arrive_cc = current_time;
	}

	for (auto i = 0; i < supermarket_->checkout_counters.size(); i++)
		{
			if (supermarket_->checkout_counters[i]->IsEmpty())
				{
				checkout_counter_idx_ = i;
				logger_->Print("        - customer " + to_string(kID_)
					+ "  finds a free checkout counter and start service ", Logger::L4);
				state_ = ServiceStarted;	
				return true;			
				}		
		}
	int min = INFINITY;
	int min_cc_queue_idx=0;
	for (auto i = 0; i < supermarket_->checkout_counters.size(); i++)
		{	
			state_ = ServiceStarted;
			if(supermarket_->cc_queues[i].size()<min)
				{
					min = supermarket_->cc_queues[i].size();
					min_cc_queue_idx = i;
				}
		}
			if (min != INFINITY)
				{	
					checkout_counter_idx_ = min_cc_queue_idx;
					supermarket_->cc_queues[min_cc_queue_idx].push(this);
					logger_->Print("        - customer " + to_string(kID_)
					+ " joins the shortest checkout counter queue", Logger::L4);
					return false;
				}
		
	
}

bool Customer::HandleNewState(double current_time)
{
	logger_->Print("        - customer " + to_string(kID_)
		+ " has arrived", Logger::L4);
	auto* new_customer = new Customer(supermarket_, agenda_);
	if (supermarket_->shopping_card_size > 0)
	{
		if (supermarket_->shopping_cards_queue.empty())
		{
			int end_of_shopping_time = current_time + dis_T_shopping(gen);
			Activate(end_of_shopping_time, "End of shopping");
			logger_->Print("        - customer " + to_string(kID_)
				+ " stars shoping ", Logger::L4);
			state_ = Shopping_Finished;
			stats::average_waiting_for_Card += current_time - t_joining_queue;

			logger_->Print("    End of shopping at " + to_string(end_of_shopping_time),
				Logger::L4, Logger::LOG_NEW_EVENT);
			supermarket_->shopping_card_size--;
		}
		else
		{
			if (stats::initial_phase <= current_time)
			{
			stats::ave_card_queue_length += (current_time - stats::last_time_queue_modified)*supermarket_->shopping_cards_queue.size();
			stats::last_time_queue_modified = current_time;
			t_joining_queue = current_time;
			stats::customers_in_queue++;
			}
			supermarket_->shopping_cards_queue.push(this);
			logger_->Print("        - customer " + to_string(kID_)
				+ " added to shopping card queue", Logger::L4);
			state_ = Start_Shopping;
		}
	}
	else
	{
		if (stats::initial_phase <= current_time)
		{
		stats::ave_card_queue_length += (current_time - stats::last_time_queue_modified)*supermarket_->shopping_cards_queue.size();
		stats::last_time_queue_modified = current_time;
		t_joining_queue = current_time;
		stats::customers_in_queue++;
		}
		supermarket_->shopping_cards_queue.push(this);
		logger_->Print("        - customer " + to_string(kID_)
			+ " added to shopping card queue", Logger::L4);
		state_ = Start_Shopping;
	}
	int next_new_customer_arrived_time = current_time+ dis_T_arrival(gen);
	if (next_new_customer_arrived_time == current_time) next_new_customer_arrived_time++;
	new_customer->Activate(next_new_customer_arrived_time,"New customer arrival");
	logger_->Print("    New Customer Event at " + to_string(next_new_customer_arrived_time),
		Logger::L4, Logger::LOG_NEW_EVENT);
	return false;
}


bool Customer::HandleServiceStartedState(double current_time)
{
	if (stats::initial_phase <= current_time)
	{
		stats::ave_waiting_cc += (current_time - arrive_cc);
		stats::total_customers_cc++;
	}
	
	supermarket_->checkout_counters[checkout_counter_idx_]->AssignCustomer(this);

	logger_->Print("        - customer " + to_string(kID_)
		+ " is assigned to checkout counter " + to_string(checkout_counter_idx_), Logger::L4);

	int service_compl_time =current_time + dis_T_checkout(gen);
	if (stats::initial_phase <= current_time)
	{
		stats::total_customers_st++;
		stats::ave_supermarket_time += (current_time - customer_arrive_time);
	}

	
	Activate(service_compl_time,"Service completed");
	logger_->Print("    Service Completed Event at " + to_string(service_compl_time),
		Logger::L4, Logger::LOG_NEW_EVENT);

	return false;
}

bool Customer::HandleServiceCompletedState(double current_time) const
{
	supermarket_->checkout_counters[checkout_counter_idx_]->RemoveCustomer();
	supermarket_->shopping_card_size++;
	Logger::GetInstance()->Print("        - customer " + to_string(kID_)
		+ " removed from check out counter " + to_string(checkout_counter_idx_), Logger::L4);

	if (supermarket_->checkout_counters[checkout_counter_idx_]->cc_wants_a_break())
		{
		supermarket_->checkout_counters[checkout_counter_idx_]->Activate(current_time,"CheckoutCounter goes on break");
		}
	else if (supermarket_->cc_queues[checkout_counter_idx_].size()>0)
	{
		auto customer = supermarket_->cc_queues[checkout_counter_idx_].front();
		supermarket_->cc_queues[checkout_counter_idx_].pop();
		customer->Activate(current_time, "Start Service ");
	}

	if (!supermarket_->shopping_cards_queue.empty())
		{
		auto customer = supermarket_->shopping_cards_queue.front();
		customer->Activate(current_time,"Start Shopping");
		}	
		return false;
}



 