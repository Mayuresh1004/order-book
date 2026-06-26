#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
using namespace std;

enum class OrderSide
{
    BUY,
    SELL
};

struct Trade
{
    int buyOrderId;

    int sellOrderId;

    int quantity;

    double executionPrice;
};

vector<Trade> trades;

struct Order
{
    int orderId;
    double price;
    int quantity;

    OrderSide side;

    long long timestamp;
};

long long currentTimestamp = 0;

// Comparator for BUY side — highest price first
struct BuyComparator
{
    bool operator()(const Order& a,const Order& b) const
    {
        if(a.price==b.price)
            return a.timestamp>b.timestamp;

        return a.price<b.price;
    }
};

// Comparator for SELL side — lowest price first
struct SellComparator {
    bool operator()(Order a, Order b) const {
        if(a.price != b.price)
            return a.price > b.price; // min heap by price
        return a.timestamp > b.timestamp;
    }
};

class OrderBook {
private:
    priority_queue<Order, vector<Order>, BuyComparator> buySide;
    priority_queue<Order, vector<Order>, SellComparator> sellSide;
    unordered_map<int, Order> orderMap; // orderId -> Order

public:
    void addOrder(Order o) {
        o.timestamp = ++currentTimestamp;
        orderMap[o.orderId] = o;
        if (o.side == OrderSide::BUY) buySide.push(o);
        else sellSide.push(o);
        cout << "Order added: " << o.orderId << endl;
        matchOrders();
    }
    void showTopFive() {
        cout << "\n--- TOP 5 BUY ORDERS ---" << endl;
        priority_queue<Order, vector<Order>, BuyComparator> tempBuy = buySide;
        int count = 0;

        while (!tempBuy.empty() && count < 5) {
            Order o = tempBuy.top();
            tempBuy.pop();

            // Skip cancelled orders
            if (orderMap.find(o.orderId) == orderMap.end())
                continue;

            // Skip stale entries after modification
            if (orderMap[o.orderId].price != o.price || 
                orderMap[o.orderId].quantity != o.quantity)
                continue;

            cout << "ID: " << o.orderId
                 << " Price: " << o.price
                 << " Qty: " << o.quantity << endl;

            count++;
        }

        cout << "\n--- TOP 5 SELL ORDERS ---" << endl;
        priority_queue<Order, vector<Order>, SellComparator> tempSell = sellSide;
        count = 0;

        while (!tempSell.empty() && count < 5) {
            Order o = tempSell.top();
            tempSell.pop();

            // Skip cancelled orders
            if (orderMap.find(o.orderId) == orderMap.end())
                continue;

            // Skip stale entries after modification
            if (orderMap[o.orderId].price != o.price || 
                orderMap[o.orderId].quantity != o.quantity)
                continue;

            cout << "ID: " << o.orderId
                 << " Price: " << o.price
                 << " Qty: " << o.quantity << endl;

            count++;
        }

        cout << endl;
    }
void cancelOrder(int orderId) {
    if (orderMap.find(orderId) == orderMap.end()) {
        cout << "Order " << orderId << " not found" << endl;
        return;
    }
    orderMap.erase(orderId);
    cout << "Order " << orderId << " cancelled" << endl;
}

void modifyOrder(int orderId, double newPrice, int newQty) {
    if (orderMap.find(orderId) == orderMap.end()) {
        cout << "Order " << orderId << " not found" << endl;
        return;
    }
    Order updated = orderMap[orderId]; // get existing
    updated.price = newPrice;
    updated.quantity = newQty;
    
    orderMap.erase(orderId);  // remove old
    orderMap[orderId] = updated;  // add updated
    
    if (updated.side == OrderSide::BUY) buySide.push(updated);
    else sellSide.push(updated);
    
    cout << "Order " << orderId << " modified" << endl;
}

    void matchOrders() {
        while (!buySide.empty() && !sellSide.empty()) {
            // Clean stale buy orders from top
            while (!buySide.empty()) {
                Order top = buySide.top();
                if (orderMap.find(top.orderId) == orderMap.end() ||
                    orderMap[top.orderId].quantity == 0) {
                    buySide.pop();
                } else {
                    break;
                }
            }

            // Clean stale sell orders from top
            while (!sellSide.empty()) {
                Order top = sellSide.top();
                if (orderMap.find(top.orderId) == orderMap.end() ||
                    orderMap[top.orderId].quantity == 0) {
                    sellSide.pop();
                } else {
                    break;
                }
            }

            if (buySide.empty() || sellSide.empty())
                break;

            Order buy = buySide.top();
            Order sell = sellSide.top();

            // Get fresh copies from map (to ensure current state)
            buy = orderMap[buy.orderId];
            sell = orderMap[sell.orderId];

            // No match possible
            if (buy.price < sell.price)
                break;

            // Execute trade
            int tradeQty = min(buy.quantity, sell.quantity);
            double tradePrice = sell.price; // Seller's price

            trades.push_back({
                buy.orderId,
                sell.orderId,
                tradeQty,
                tradePrice
            });

            cout << "\n=== TRADE EXECUTED ===\n";
            cout << "BUY Order  : " << buy.orderId << endl;
            cout << "SELL Order : " << sell.orderId << endl;
            cout << "Qty        : " << tradeQty << endl;
            cout << "Price      : " << tradePrice << endl;
            cout << "========================\n";

            // Update quantities
            buy.quantity -= tradeQty;
            sell.quantity -= tradeQty;

            // Update or remove from map
            if (buy.quantity > 0) {
                orderMap[buy.orderId] = buy;
            } else {
                orderMap.erase(buy.orderId);
            }

            if (sell.quantity > 0) {
                orderMap[sell.orderId] = sell;
            } else {
                orderMap.erase(sell.orderId);
            }

            // Remove matched orders from queues
            buySide.pop();
            sellSide.pop();
            
            // Re-push if partially filled
            if (buy.quantity > 0) {
                buySide.push(buy);
            }
            if (sell.quantity > 0) {
                sellSide.push(sell);
            }
        }
    }

void showTradeHistory() {

    cout << "\n----- TRADE HISTORY -----\n";

    if (trades.empty()) {
        cout << "No trades executed.\n";
        return;
    }

    for (const auto &t : trades) {
        cout << "BUY: " << t.buyOrderId
             << " SELL: " << t.sellOrderId
             << " Qty: " << t.quantity
             << " Price: " << t.executionPrice
             << endl;
    }
}
};



int main() {
    OrderBook ob;
    int choice, id, qty;
    double price;
    string type;

    while (true) {
        cout << "\n1. Add Order\n2. Cancel Order\n3. Modify Order\n4. Show Top 5\n5. Show Trade History\n6. Exit\n";
        cout << "Choice: ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }
        cin.ignore(10000, '\n');

        if (choice == 1) {
            cout << "Enter id, price, qty, type (BUY/SELL): ";
            if (!(cin >> id >> price >> qty >> type)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input format. Use: id price qty type (space-separated)" << endl;
                continue;
            }
            cin.ignore(10000, '\n');
            ob.addOrder({id, price, qty, (type == "BUY" ? OrderSide::BUY : OrderSide::SELL)});
        } else if (choice == 2) {
            cout << "Enter orderId to cancel: ";
            if (!(cin >> id)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input." << endl;
                continue;
            }
            cin.ignore(10000, '\n');
            ob.cancelOrder(id);
        } else if (choice == 3) {
            cout << "Enter orderId, newPrice, newQty: ";
            if (!(cin >> id >> price >> qty)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input format. Use: id price qty (space-separated)" << endl;
                continue;
            }
            cin.ignore(10000, '\n');
            ob.modifyOrder(id, price, qty);
        } else if (choice == 4) {
            ob.showTopFive();
        } else if (choice == 5) {
            ob.showTradeHistory();
        } else if (choice == 6) {
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}