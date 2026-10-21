# FreshMart Supermarket Checkout System (C++)

A menu-driven C++ console application demonstrating how a queue can manage customers at a supermarket checkout.

## Features
- Add customers to a waiting queue and assign ticket numbers.
- Display the queue in first-in, first-out (FIFO) order.
- Check out the next customer.
- Browse a sample product catalog with stock levels and prices.
- Add items to a customer's cart and calculate subtotal, example tax, total, payment, and change.
- Print a receipt in the console.
- Restock products.
- View completed checkouts and total sales revenue.

## Data structure
The project uses `std::queue<Customer>`. Customers are added at the rear with `push()` and the customer at the front is served and removed with `pop()`. This illustrates the FIFO (First In, First Out) principle.

## Requirements
- A C++ compiler that supports C++17 (for example, GCC, Clang, or a recent Visual Studio compiler).

## Compile and run

### Linux / macOS / MinGW
```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o supermarket_checkout
./supermarket_checkout
```

### Windows PowerShell (with MinGW g++)
```powershell
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o supermarket_checkout.exe
.\supermarket_checkout.exe
```

You can also open `main.cpp` in Code::Blocks or Visual Studio Code and compile it with a C++17-capable compiler.

## How to use
1. Select **Add customer to queue** and enter a name.
2. Add more customers if desired.
3. Select **View waiting queue** to check the order.
4. Select **Checkout next customer**. Add products by ID and quantity; enter `0` when finished shopping.
5. Enter the payment amount. The program prints a receipt and updates sales totals.
6. Use **Restock product** to increase stock when needed.
7. Use **Sales summary** to see completed checkouts and revenue.

## Notes and limitations
- The listed products, prices, stock quantities, and 7.5% tax rate are sample values for demonstration. Adjust them to suit your assignment or local requirements.
- This is an educational console prototype. Data is kept in memory and resets when the program closes.
- If checkout is cancelled after adding items, stock is not restored automatically in this starter version; for a production system, use a cart reservation/commit step and restore stock on cancellation.
- The system uses one FIFO queue and one checkout lane. Real supermarkets may use multiple lanes, loyalty systems, discounts, refunds, receipt printers, databases, and priority rules.
