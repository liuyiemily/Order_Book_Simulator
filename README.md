# Order_Book_Simulator
Implemented an order book simulator in C++, given a list of strings to output the matched orders with price, time priority. 

Objectives:
The task is to implement an order book simulator. You are given orders as a list of strings and are required write code to print output of what the matched orders will be.
Orders should be matched in “price,time” priority.

– for example, given the following input:

Name, Direction, Quantity, Price;
Customer1 ,Buy,10,100;
Customer2,Buy,10,101;
Seller1,Sell,5,99

Then a single trade between ‘Customer2’ and ‘Seller1’ should be generated, because even though Customer2’s order came in later, it is at a more competitive price so should get filled first, yielding the following output:
Customer2,Seller1,5,101

Note also from the above example: The execution price for the trade should be the limit price of the order that was already in the order book. So, for example, if there is an order to buy @ 100 and then an order to sell @ 2, then the trade should be generated with a price of 100, not 2. Whereas, if instead the orders came in the opposite order – an order to sell @ 2 followed by an order to buy @ 100, then the trade would be done at a price of 2 instead of 100.  Another way to think of this: There is no need to incorporate special semantics for ‘market’ orders: A “market” order to buy is equivalent to a limit Buy order with a price of infinity, and a “market” order to sell can be expressed as a limit Sell order with a price of 0.

When multiple orders between the same buyer and seller have been matched with the same resultant price, it is ok to output, for example:
Customer1,Seller1,IBM,Buy,10,100;
Customer1,Seller1,IBM,Buy,20,100;

instead of
Customer1,Seller1,IBM,Buy,30,100
