#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
using namespace std;

struct Order {
    int orderId;
    double price;
    int quantity;
    string type; // "BUY" or "SELL"
};

// Comparator for BUY side — highest price first
struct BuyComparator {
    bool operator()(Order a, Order b) {
        return a.price < b.price; // max heap by price
    }
};

// Comparator for SELL side — lowest price first
struct SellComparator {
    bool operator()(Order a, Order b) {
        return a.price > b.price; // min heap by price
    }
};

class OrderBook {
private:
    priority_queue<Order, vector<Order>, BuyComparator> buySide;
    priority_queue<Order, vector<Order>, SellComparator> sellSide;
    unordered_map<int, Order> orderMap; // orderId -> Order

public:
    void addOrder(Order o) {
        orderMap[o.orderId] = o;
        if (o.type == "BUY") buySide.push(o);
        else sellSide.push(o);
        cout << "Order added: " << o.orderId << endl;
    }
    void showTopFive() {
    cout << "\n--- TOP 5 BUY ORDERS ---" << endl;
    priority_queue<Order, vector<Order>, BuyComparator> tempBuy = buySide;
    int count = 0;
    while (!tempBuy.empty() && count < 5) {
        Order o = tempBuy.top();
        tempBuy.pop();
        // skip if cancelled (not in orderMap)
        if (orderMap.find(o.orderId) == orderMap.end()) continue;
        if (orderMap[o.orderId].price != o.price) continue; // stale entry
        cout << "ID: " << o.orderId << " Price: " << o.price << " Qty: " << o.quantity << endl;
        count++;
    }
    // repeat same for sell side
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
    
    if (updated.type == "BUY") buySide.push(updated);
    else sellSide.push(updated);
    
    cout << "Order " << orderId << " modified" << endl;
}
};



int main() {
    OrderBook ob;
    ob.addOrder({1, 250.50, 100, "BUY"});
    ob.addOrder({2, 240.00, 200, "SELL"});
    ob.addOrder({3, 255.00, 150, "BUY"});
    ob.showTopFive();

    ob.cancelOrder(1);
    ob.cancelOrder(99); // doesn't exist
    ob.showTopFive();

    ob.modifyOrder(3, 260.00, 175);
    ob.showTopFive();

}