#ifndef MODEL_PROCESS_H_
#define MODEL_PROCESS_H_
#include<string>
struct	supermarket;
class Agenda;

class Process
{
public:
	Process(supermarket *sm, Agenda *a) :supermarket_(sm), agenda_(a), terminated_(false) {}
	virtual  void Execute(double current_time) = 0;

	virtual  bool IsTerminated() const = 0;

	virtual  void Activate(double event_time, std::string event_type) = 0;
protected:
	supermarket *supermarket_;
	Agenda *agenda_;
	bool terminated_;
};

#endif /* MODEL_PROCESS_H_ */
