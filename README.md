# Order Matching Engine

A lightweight **electronic limit order book and order matching engine** implemented in **C++17**. This project simulates the core functionality of modern stock exchanges by maintaining separate buy and sell order books, automatically matching compatible orders using **price-time priority**, and recording executed trades.

---

## Features

* Buy and Sell Limit Orders
* Automatic Order Matching
* Price-Time Priority
* Partial Order Execution
* Order Modification
* Order Cancellation
* Trade History
* Top 5 Buy/Sell Order Display
* Lazy Deletion for Efficient Cancellation

---

## Architecture

```
                 User
                   │
                   ▼
            +--------------+
            |  OrderBook   |
            +--------------+
             │     │     │
             │     │     │
             ▼     ▼     ▼
        Buy Heap Sell Heap Order Map
             │           │
             └─────┬─────┘
                   ▼
           Matching Engine
                   │
                   ▼
            Executed Trades
```

---

## Data Structures Used

### `priority_queue`

Maintains the best available buy and sell orders.

* Buy Side → Max Heap
* Sell Side → Min Heap

Provides efficient access to the highest bid and lowest ask.

---

### `unordered_map`

Maps

```
Order ID
      ↓
 Order Details
```

Used for:

* O(1) order lookup
* Order modification
* Order cancellation

---

### `vector`

Stores executed trades sequentially for trade history.

---

## Matching Algorithm

Whenever a new order is added or an existing order is modified, the engine attempts to match orders automatically.

The matching process follows:

```
while

Highest BUY Price >= Lowest SELL Price

{

Execute Trade

Reduce Quantities

Remove Filled Orders

Reinsert Partially Filled Orders

}
```

---

## Price-Time Priority

The engine follows the same matching principle used by most modern exchanges.

Orders are prioritized by:

1. Better Price
2. Earlier Timestamp

Example:

```
BUY 250 @ Time 1
BUY 250 @ Time 2
```

The first order executes before the second.

---

## Trade Execution

Example

```
BUY

Price : 250
Qty   : 100
```

```
SELL

Price : 245
Qty   : 40
```

Result

```
Trade Executed

Quantity : 40

Price : 245
```

Remaining Order

```
BUY

Price : 250

Qty : 60
```

---

## Time Complexity

| Operation    | Complexity         |
| ------------ | ------------------ |
| Add Order    | O(log n)           |
| Cancel Order | O(1) Average       |
| Modify Order | O(log n)           |
| Best Buy     | O(1)               |
| Best Sell    | O(1)               |
| Match Orders | O(log n) per trade |

---

## Technologies Used

* C++17
* STL
* Priority Queue
* Unordered Map
* Vector
* Object-Oriented Programming

---

## Current Limitations

* Single-threaded implementation
* Supports limit orders only
* In-memory order book (no persistence)
* Single financial instrument
* Console-based interface

---

## Future Improvements

* Market Orders
* Multiple Stock Symbols
* Thread-safe Matching Engine
* TCP Socket Server
* REST API
* Persistent Storage
* Performance Benchmarking
* Unit Tests
* Market Data Feed Simulation

---

## Sample Execution

```
1. Add Order
2. Cancel Order
3. Modify Order
4. Show Top 5
5. Show Trade History
6. Exit
```

Example

```
Add BUY Order

ID : 1

Price : 250

Qty : 100
```

```
Add SELL Order

ID : 2

Price : 245

Qty : 60
```

Output

```
Trade Executed

BUY Order  : 1

SELL Order : 2

Qty        : 60

Price      : 245
```

---

## Learning Outcomes

This project strengthened my understanding of:

* Data Structures
* Heap-based Priority Queues
* Price-Time Priority Matching
* Object-Oriented Design
* Time Complexity Analysis
* STL Containers
* Electronic Trading Systems
* Order Book Management
* Trade Execution Logic

---
