#ifndef MODEL_SUPERMARKET_H_
#define MODEL_SUPERMARKET_H_

#include <queue>

class Agenda;
class Customer;
class CheckoutCounter;
struct ShoppingCard;

struct supermarket
{
	const int kNumberOfCheckoutCounters;
	const int kNumberOfShoppingCards;
	double lambd;
	supermarket(int nr_of_sds, int nr_of_cards, Agenda *agenda,double la);
	~supermarket();

	std::queue<Customer*> shopping_cards_queue; //queue to shopping cards
	std::vector<CheckoutCounter*> checkout_counters; // nr of checkout counter
	std::vector < std::queue<Customer*>>cc_queues; // cc_queue of customers
	int shopping_card_size;
	
};
#endif /*MODEL_SUPERMARKET_H_*/
