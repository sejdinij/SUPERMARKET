#include <iostream>
#include "supermarket.h"
#include "shopping_card.h"
#include "customer.h"
#include "checkout_counter.h"


supermarket::supermarket(int nr_of_sds, int nr_of_cards, Agenda *agenda,double l) : kNumberOfCheckoutCounters(nr_of_sds), kNumberOfShoppingCards(nr_of_cards),lambd(l)
{
	for (int i = 0; i < kNumberOfCheckoutCounters; i++)
	{
		checkout_counters.push_back(new CheckoutCounter(i, this, agenda));
		cc_queues.push_back(std::queue<Customer*>());
	}
	shopping_card_size = kNumberOfShoppingCards;
	
}

supermarket::~supermarket()
{
	while (!shopping_cards_queue.empty())
	{
		Customer *c = shopping_cards_queue.front();
		shopping_cards_queue.pop();
		delete c;
	}

	for (auto *sd : checkout_counters)
	{
		delete sd;
	}
}