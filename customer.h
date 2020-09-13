#ifndef MODEL_CUSTOMER_H_
#define MODEL_CUSTOMER_H_
#include "process.h"
#include <random>
#include "stats.h"

class Logger;
class Agenda;
struct supermarket;

class Customer :public Process
{
public:

	Customer(supermarket *dh, Agenda *a);
	~Customer();

	int getID() const;

	void Execute(double current_time) override;

	bool IsTerminated() const override;

	void Activate(double event_time,std::string event_type) override;
	


private:
	std::exponential_distribution<> dis_T_arrival;
	std::uniform_int_distribution<> dis_T_shopping;
	std::uniform_int_distribution<> dis_T_checkout;
	std::random_device rd;
	std::mt19937 gen;
	enum State
	{
		Start_Shopping,
		Shopping_Finished,
		New,
		ServiceStarted,
		ServiceCompleted
	};
	
	static int global_id_;
	const int kID_;
	double customer_arrive_time;
	double arrive_cc;
	double t_joining_queue;
	bool StartShopping(double current_time);
	bool ShoppingFinished(double current_time);
	bool HandleNewState(double current_time);
	bool HandleServiceStartedState(double current_time);
	bool HandleServiceCompletedState(double current_time) const;
	


	Logger *logger_;
	State state_;
	int checkout_counter_idx_;
};

#endif /*MODEL_CUSTOMER_H_*/
