#include <iostream>
#include <iomanip>
#include <string>
#include <queue>
#include <vector>
#include <limits>
#include <ctime>
#include <sstream>

using namespace std;

struct Product {
    int id;
    string name;
    double price;
    int stock;
};

struct Customer {
    int ticket;
    string name;
};

struct SaleItem {
    string name;
    int quantity;
    double unitPrice;

    double total() const {
        return unitPrice * quantity;
    }
};

class SupermarketCheckoutSystem {
private:
    vector<Product> products;
    queue<Customer> waitingCustomers;
    int nextTicket = 1001;
    int completedSales = 0;
    double totalRevenue = 0.0;
    const double taxRate = 0.075; // 7.5% example tax rate; adjust for local requirements.

    void pause() {
        cout << "\nPress Enter to continue...";
        cin.get();
    }

    void clearInputLine() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int readInt(const string& prompt, int minValue, int maxValue) {
        int value;
        while (true) {
            cout << prompt;
            if (cin >> value && value >= minValue && value <= maxValue) {
                clearInputLine();
                return value;
            }
            cout << "Invalid input. Enter a number from "
                 << minValue << " to " << maxValue << ".\n";
            cin.clear();
            clearInputLine();
        }
    }

    double readMoney(const string& prompt, double minValue = 0.0) {
        double value;
        while (true) {
            cout << prompt;
            if (cin >> value && value >= minValue) {
                clearInputLine();
                return value;
            }
            cout << "Invalid amount. Please try again.\n";
            cin.clear();
            clearInputLine();
        }
    }

    string readNonEmptyLine(const string& prompt) {
        string value;
        do {
            cout << prompt;
            getline(cin, value);
            if (value.empty()) {
                cout << "This field cannot be empty.\n";
            }
        } while (value.empty());
        return value;
    }

    Product* findProduct(int id) {
        for (auto& product : products) {
            if (product.id == id) return &product;
        }
        return nullptr;
    }

    void printHeader(const string& title) const {
        cout << "\n============================================================\n";
        cout << "              FRESHMART SUPERMARKET\n";
        cout << "                 " << title << "\n";
        cout << "============================================================\n";
    }

    void showProducts() const {
        printHeader("PRODUCT CATALOG");
        cout << left << setw(6) << "ID"
             << setw(25) << "PRODUCT"
             << right << setw(12) << "PRICE"
             << setw(12) << "STOCK" << "\n";
        cout << string(55, '-') << "\n";

        for (const auto& product : products) {
            cout << left << setw(6) << product.id
                 << setw(25) << product.name
                 << right << setw(12) << fixed << setprecision(2) << product.price
                 << setw(12) << product.stock << "\n";
        }
    }

    void addCustomer() {
        printHeader("ADD CUSTOMER TO QUEUE");
        string name = readNonEmptyLine("Customer name: ");
        Customer customer{nextTicket++, name};
        waitingCustomers.push(customer);

        cout << "\nCustomer added successfully.\n";
        cout << "Ticket number: " << customer.ticket << "\n";
        cout << "Queue position: " << waitingCustomers.size() << "\n";
    }

    void viewQueue() const {
        printHeader("CUSTOMER WAITING QUEUE");
        if (waitingCustomers.empty()) {
            cout << "There are no customers waiting.\n";
            return;
        }

        queue<Customer> copy = waitingCustomers;
        cout << left << setw(12) << "TICKET" << "CUSTOMER\n";
        cout << string(40, '-') << "\n";
        int position = 1;
        while (!copy.empty()) {
            const Customer& customer = copy.front();
            cout << left << setw(12) << customer.ticket
                 << customer.name << "  (position " << position << ")\n";
            copy.pop();
            ++position;
        }
        cout << "\nCustomers waiting: " << waitingCustomers.size() << "\n";
    }

    void printReceipt(const Customer& customer,
                      const vector<SaleItem>& cart,
                      double subtotal, double tax, double grandTotal,
                      double amountPaid, double change) {
        time_t now = time(nullptr);
        tm* localTime = localtime(&now);
        char timeBuffer[32] = "Unknown";
        if (localTime) {
            strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", localTime);
        }

        printHeader("CUSTOMER RECEIPT");
        cout << "Ticket: " << customer.ticket << "\n";
        cout << "Customer: " << customer.name << "\n";
        cout << "Date: " << timeBuffer << "\n\n";

        cout << left << setw(23) << "ITEM"
             << right << setw(6) << "QTY"
             << setw(12) << "UNIT PRICE"
             << setw(12) << "TOTAL" << "\n";
        cout << string(53, '-') << "\n";

        for (const auto& item : cart) {
            cout << left << setw(23) << item.name.substr(0, 22)
                 << right << setw(6) << item.quantity
                 << setw(12) << fixed << setprecision(2) << item.unitPrice
                 << setw(12) << item.total() << "\n";
        }

        cout << string(53, '-') << "\n";
        cout << right << setw(41) << "Subtotal: " << setw(12) << subtotal << "\n";
        cout << right << setw(41) << "Tax (" << taxRate * 100 << "%): "
             << setw(12) << tax << "\n";
        cout << right << setw(41) << "TOTAL: " << setw(12) << grandTotal << "\n";
        cout << right << setw(41) << "Amount paid: " << setw(12) << amountPaid << "\n";
        cout << right << setw(41) << "Change: " << setw(12) << change << "\n";
        cout << "\nThank you for shopping at FreshMart!\n";
    }

    void checkoutNextCustomer() {
        printHeader("CHECKOUT");
        if (waitingCustomers.empty()) {
            cout << "No customers are waiting. Add a customer first.\n";
            return;
        }

        Customer customer = waitingCustomers.front();
        cout << "Now serving ticket " << customer.ticket
             << " - " << customer.name << "\n";

        vector<SaleItem> cart;
        while (true) {
            showProducts();
            cout << "\nEnter product ID to add an item, or 0 to finish shopping.\n";
            int id = readInt("Product ID: ", 0, 999999);
            if (id == 0) break;

            Product* product = findProduct(id);
            if (!product) {
                cout << "Product ID not found.\n";
                continue;
            }
            if (product->stock <= 0) {
                cout << "Sorry, " << product->name << " is out of stock.\n";
                continue;
            }

            int quantity = readInt("Quantity: ", 1, 100000);
            if (quantity > product->stock) {
                cout << "Only " << product->stock << " unit(s) available.\n";
                continue;
            }

            auto existing = cart.end();
            for (auto it = cart.begin(); it != cart.end(); ++it) {
                if (it->name == product->name) {
                    existing = it;
                    break;
                }
            }

            if (existing != cart.end()) {
                existing->quantity += quantity;
            } else {
                cart.push_back({product->name, quantity, product->price});
            }
            product->stock -= quantity;
            cout << quantity << " x " << product->name << " added to the cart.\n";
        }

        if (cart.empty()) {
            cout << "No items were purchased. Customer remains at the front of the queue.\n";
            return;
        }

        double subtotal = 0.0;
        for (const auto& item : cart) subtotal += item.total();
        double tax = subtotal * taxRate;
        double grandTotal = subtotal + tax;

        cout << "\nSubtotal: " << fixed << setprecision(2) << subtotal << "\n";
        cout << "Tax (" << taxRate * 100 << "%): " << tax << "\n";
        cout << "Total due: " << grandTotal << "\n";

        double amountPaid = readMoney("Amount paid: ", 0.0);
        while (amountPaid < grandTotal) {
            cout << "Insufficient payment. Amount still due: "
                 << fixed << setprecision(2) << grandTotal - amountPaid << "\n";
            amountPaid += readMoney("Additional payment: ", 0.0);
        }

        double change = amountPaid - grandTotal;
        printReceipt(customer, cart, subtotal, tax, grandTotal, amountPaid, change);

        waitingCustomers.pop();
        ++completedSales;
        totalRevenue += grandTotal;
    }

    void restockProduct() {
        showProducts();
        int id = readInt("\nEnter product ID to restock: ", 1, 999999);
        Product* product = findProduct(id);
        if (!product) {
            cout << "Product ID not found.\n";
            return;
        }
        int quantity = readInt("Quantity to add: ", 1, 100000);
        product->stock += quantity;
        cout << product->name << " restocked. New stock: " << product->stock << "\n";
    }

    void salesSummary() const {
        printHeader("SALES SUMMARY");
        cout << "Completed checkouts: " << completedSales << "\n";
        cout << "Total sales revenue: " << fixed << setprecision(2)
             << totalRevenue << "\n";
        cout << "Customers still waiting: " << waitingCustomers.size() << "\n";
    }

public:
    SupermarketCheckoutSystem() {
        products = {
            {1, "Rice (1 kg)", 18.50, 40},
            {2, "Bread", 12.00, 30},
            {3, "Milk (1 litre)", 16.75, 25},
            {4, "Eggs (6 pack)", 22.00, 20},
            {5, "Cooking Oil (1 litre)", 28.50, 18},
            {6, "Sugar (1 kg)", 15.00, 35},
            {7, "Tomatoes (1 kg)", 10.00, 25},
            {8, "Bottled Water", 5.00, 60},
            {9, "Soap", 8.50, 45},
            {10, "Biscuits", 7.00, 50}
        };
    }

    void run() {
        int choice;
        do {
            printHeader("CHECKOUT MANAGEMENT SYSTEM");
            cout << "1. Add customer to queue\n";
            cout << "2. View waiting queue\n";
            cout << "3. Checkout next customer\n";
            cout << "4. View product catalog\n";
            cout << "5. Restock product\n";
            cout << "6. View sales summary\n";
            cout << "7. Exit\n";
            cout << "------------------------------------------------------------\n";
            choice = readInt("Choose an option: ", 1, 7);

            switch (choice) {
                case 1: addCustomer(); break;
                case 2: viewQueue(); break;
                case 3: checkoutNextCustomer(); break;
                case 4: showProducts(); break;
                case 5: restockProduct(); break;
                case 6: salesSummary(); break;
                case 7:
                    cout << "\nThank you for using FreshMart Checkout System.\n";
                    break;
            }
        } while (choice != 7);
    }
};

int main() {
    SupermarketCheckoutSystem system;
    system.run();
    return 0;
}
