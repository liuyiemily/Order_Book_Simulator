#include <algorithm>
#include <iostream>
#include <list>
#include <string>

using namespace std;

struct Order
{
	string name;
	string direction;
	int quantity;
    	int price;
	long long id; // use to maintain time priority
};

struct matched_order
{
	string buyer;
	string seller;
	int matched_quantity;
	int matched_price;
};

class order_simulator
{
public:
    order_simulator(const list<list<string> >& order_list):
        order_list_(order_list),
        buy_orders_(list<Order>{}),
        sell_orders_(list<Order>{})
         {}
    ~order_simulator(){}
    Order StringtoOrder(const list<string>& list1, long long i);
    list<matched_order> match();
    list<matched_order> process_order();

private:
    list<list<string> > order_list_;
    list<Order> buy_orders_;
    list<Order> sell_orders_;
};

// converts list<string> to list<Order>
Order order_simulator::StringtoOrder(const list<string>& list1, long long i)
{
	Order target;
	list<string> list_ = list1;
	target.name = list_.front();
	list_.pop_front();
	target.direction = list_.front();
	list_.pop_front();
	target.id = target.direction == "Buy" ? i : -i;
	target.quantity = atoi(list_.front().c_str());
	list_.pop_front();
	target.price = atoi(list_.front().c_str());

	return target;
}
// For comparison of Orders of same direction (both buy_orders or both sell_orders)
// rank the list according to price, time priority
bool hightolow(const Order& o1, const Order& o2)
{
	return ((o1.price > o2.price) || (o1.price == o2.price && o1.id < o2.id));
}

// return matched orders on the sorted buy_orders and sell_orders
list<matched_order> order_simulator::match()
{
	list<matched_order> results;
	while (buy_orders_.size() > 0 && sell_orders_.size() > 0 &&
		buy_orders_.front().price >= sell_orders_.front().price)
	{
		matched_order o;
		o.buyer = buy_orders_.front().name;
		o.seller = sell_orders_.front().name;
		// matched_price according to the time priority
		o.matched_price = buy_orders_.front().id < -sell_orders_.front().id ?
			buy_orders_.front().price : sell_orders_.front().price;
		if (buy_orders_.front().quantity <= sell_orders_.front().quantity)
		{
			o.matched_quantity = buy_orders_.front().quantity;
			buy_orders_.pop_front();
			sell_orders_.front().quantity -= o.matched_quantity;
		}
		else
		{
			o.matched_quantity = sell_orders_.front().quantity;
			sell_orders_.pop_front();
			buy_orders_.front().quantity -= o.matched_quantity;
		}
		results.push_back(o);
	}
	return results;
}

// the main function to process the list<string> and output matched orders
list<matched_order> order_simulator::process_order()
{
	list<matched_order> order_matched;

	long long i = 1;
	while (order_list_.size() > 0)
	{
		// initialize the lowest_sell_price and highest_buy_price
		int lowest_sell_price = sell_orders_.size() == 0 ? INT_MAX : sell_orders_.front().price;
		int highest_buy_price = buy_orders_.size() == 0 ? -1 : buy_orders_.front().price;

		while (true)
		{
			list<string> s = order_list_.front();
			Order o = StringtoOrder(s, i);
			if (o.direction == "Buy")
			{
				buy_orders_.push_back(o);
				// compare the new buy price with the current highest buy price and update it
				highest_buy_price = o.price < highest_buy_price ? highest_buy_price : o.price;
			}
			else if (o.direction == "Sell")
			{
				sell_orders_.push_back(o);
				// compare the new sell price with the current lowest sell price and update it
				lowest_sell_price = o.price > lowest_sell_price ? lowest_sell_price : o.price;
			}
			else
				throw ("Wrong direction!");

			order_list_.pop_front();
			++i;
			// break if the order book is empty or guarantee a match
			if (order_list_.size() == 0 || highest_buy_price >= lowest_sell_price)
				break;
		}

		if (highest_buy_price >= lowest_sell_price)
		{
			buy_orders_.sort(hightolow);
			sell_orders_.sort(hightolow);
			sell_orders_.reverse();

			list<matched_order> m_order = match();
			// transfer the matched orders to order_matched and empty the m_order
			order_matched.splice(order_matched.end(), m_order);
		}
	}
	return order_matched;
}

std::ostream& operator<<(std::ostream& ostr, const std::list<matched_order>& list1)
{
	ostr << " " << "Buyer" << "," << "\t" << "Seller" << "," << "\t" << "Quantity" << "," << "\t"
		<< "Price" << "\n";

	for (auto &i : list1)
    {
		ostr << " " << i.buyer << "," << "\t" << i.seller << "," << "\t" << i.matched_quantity << "," << "\t"
			<< i.matched_price << "\n";
	}
	return ostr;
}

int main()
{
	std::list<std::list<std::string> > order_list_1{
		{ "Customer1", "Buy", "10", "100" },
		{ "Customer2", "Buy", "10", "101" },
		{ "Seller1", "Sell", "5", "99" }
	};
	order_simulator order_1(order_list_1);
	list<matched_order> o1 = order_1.process_order();
	cout << o1 << "\n";

	std::list<std::list<std::string>> order_list_2{
		{ "Jane", "Buy", "50", "100" },
		{ "Bob", "Sell", "50", "110" },
		{ "Chris", "Sell", "60", "99" },
		{ "Mark", "Sell", "60", "99" },
		{ "Phillip", "Buy", "40", "120" }
	};

	order_simulator order_2(order_list_2);
	list<matched_order> o2 = order_2.process_order();
	cout << o2 << "\n";

	std::list<std::list<std::string>> order_list_3{
		{ "Customer1", "Buy", "10", "100" },
		{ "Customer1", "Buy", "100", "99" },
		{ "Customer2", "Buy", "200", "98" },
		{ "Customer3", "Sell", "1000", "50" }
	};

	order_simulator order_3(order_list_3);
	list<matched_order> o3 = order_3.process_order();
	cout << o3 << "\n";

	return 0;
}
