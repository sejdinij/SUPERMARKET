#ifndef MODEL_CHECKOUT_COUNTER_H_
#define MODEL_CHECKOUT_COUNTER_H_
#include "process.h"
#include "logger.h"
#include <random>

struct supermarket;
class Agenda;
class Customer;
class CheckoutCounter : public Process
{
public:
	CheckoutCounter(int id, supermarket *sm, Agenda *a);
	~CheckoutCounter();

	void Execute(double current_time) override;

	bool IsTerminated() const override;

	void Activate(double event_time, std::string event_type) override;

	bool IsEmpty();
	void AssignCustomer(Customer *c);
	Customer* RemoveCustomer();
	int getID();
	bool cc_wants_a_break();

private:
	bool is_On_Break;
	bool wants_to_go_on_break;
	enum State
	{
		Available,
		Not_Available
	};
	const int kID_;
	Customer *served_customer_;
	State state_;
	Logger *logger_;
	std::uniform_int_distribution<> dis_T_p;
	std::uniform_int_distribution<> dis_T_susp;
	std::random_device rd;
	std::mt19937 gen;

};


#endif /* MODEL_CHECKOUT_COUNTER_H_ */
