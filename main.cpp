#include <iostream>
#include "simulator.h"
#include "logger.h"
#include "stats.h"
#include "stats.h"


int main()
{
	printf("------------------------------------------------------------------------\n");
	printf("                           INITIALIZATION\n");
	printf("------------------------------------------------------------------------\n");
	double  lam;
	std::cout << "Please provide lambda: ";
	std::cin >> lam;
	std::cout << "Please provide initial phase: ";
	std::cin >> stats::initial_phase; 
	Simulator *simulator = new Simulator(1000, 1, 20,lam);

	bool step_by_step = true; int logger_verb;
	printf("Choose simulation mode (1 - step by step, 0 - continous): ");
	std::cin >> step_by_step;
	do
	{
		printf("Choose logger verbosity level (1-4): ");
		std::cin >> logger_verb;
	} while (logger_verb < 1 || logger_verb > 4);

	simulator->Initialize(step_by_step, static_cast<Logger::LogVerbosity>(logger_verb));

	printf("------------------------------------------------------------------------\n");
	printf("                           SIMULATION\n");
	printf("------------------------------------------------------------------------\n");

	simulator->Start();


	printf("------------------------------------------------------------------------\n");
	printf("                           FINISHED\n");
	printf("------------------------------------------------------------------------\n");


	printf("------------------------------------------------------------------------\n");
	printf("                           STATISTICS\n");
	printf("------------------------------------------------------------------------\n");
	std::cout << "\nAverage waiting time for the checkout counter  [min]: ";
	std::cout << stats::ave_waiting_cc / stats::total_customers_cc;
	std::cout << "\nAverage time customers spend at supermarket  [min]: ";
	std::cout << stats::ave_supermarket_time / stats::total_customers_st;
	std::cout << "\nAverage card queue length: ";
	std::cout << stats::ave_card_queue_length/1000;
	std::cout << "\nAverage waiting time for card: ";
	std::cout << stats::average_waiting_for_Card / stats::customers_in_queue;
	std::cout << "\n";


	delete simulator;

	system("Pause");
}