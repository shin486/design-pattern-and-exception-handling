#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <cctype>
#include <iomanip>

// Product class
class Product {
private:
    std::string id;
    std::string name;
    float price;

public:
    Product() : id(""), name(""), price(0) {} // Default constructor
    Product(const std::string& id, const std::string& name, float price) 
        : id(id), name(name), price(price) {}

    std::string getId() const { return id; }
    std::string getName() const { return name; }
    float getPrice() const { return price; }
};

// Shopping Cart Item class
class CartItem {
private:
    Product product;
    int quantity;

public:
    CartItem() : product(Product("", "", 0)), quantity(0) {} // Default constructor
    CartItem(const Product& product, int quantity) 
        : product(product), quantity(quantity) {}

    Product getProduct() const { return product; }
    int getQuantity() const { return quantity; }
    void setQuantity(int qty) { quantity = qty; }
    float getTotalPrice() const { return product.getPrice() * quantity; }
};

// Order Item class
class OrderItem {
private:
    std::string productId;
    std::string productName;
    float price;
    int quantity;

public:
    OrderItem() : productId(""), productName(""), price(0), quantity(0) {} // Default constructor
    OrderItem(const std::string& id, const std::string& name, float price, int quantity)
        : productId(id), productName(name), price(price), quantity(quantity) {}

    std::string getProductId() const { return productId; }
    std::string getProductName() const { return productName; }
    float getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    float getTotalPrice() const { return price * quantity; }
};

// Order class
class Order {
private:
    static int nextOrderId;
    int orderId;
    float totalAmount;
    std::string paymentMethod;
    OrderItem* items;
    int itemCount;

public:
    Order(float total, const std::string& method, OrderItem* items, int count)
        : totalAmount(total), paymentMethod(method), items(items), itemCount(count) {
        orderId = nextOrderId++;
    }

    ~Order() {
        delete[] items;
    }

    int getOrderId() const { return orderId; }
    float getTotalAmount() const { return totalAmount; }
    std::string getPaymentMethod() const { return paymentMethod; }
    int getItemCount() const { return itemCount; }
    const OrderItem* getItems() const { return items; }

    void display() const {
        std::cout << "Order ID: " << orderId << "\n";
        std::cout << "Total Amount: " << totalAmount << "\n";
        std::cout << "Payment Method: " << paymentMethod << "\n";
        std::cout << "Order Details:\n";
        std::cout << std::left
                  << std::setw(12) << "Product ID"
                  << std::setw(20) << "Name"
                  << std::setw(10) << "Price"
                  << std::setw(10) << "Quantity" << "\n";
        for (int i = 0; i < itemCount; i++) {
            std::cout << std::left
                      << std::setw(12) << items[i].getProductId()
                      << std::setw(20) << items[i].getProductName()
                      << std::setw(10) << items[i].getPrice()
                      << std::setw(10) << items[i].getQuantity() << "\n";
        }
        std::cout << "\n";
    }
};

int Order::nextOrderId = 1;

// Payment Strategy Interface
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() {}
    virtual void pay(float amount) = 0;
    virtual std::string getMethodName() const = 0;
};

// Concrete Payment Strategies
class CashPayment : public PaymentStrategy {
public:
    void pay(float amount) override {
        std::cout << "Paid $" << amount << " in cash.\n";
    }
    std::string getMethodName() const override { return "Cash"; }
};

class CardPayment : public PaymentStrategy {
public:
    void pay(float amount) override {
        std::cout << "Paid $" << amount << " by credit/debit card.\n";
    }
    std::string getMethodName() const override { return "Credit / Debit Card"; }
};

class GCashPayment : public PaymentStrategy {
public:
    void pay(float amount) override {
        std::cout << "Paid $" << amount << " via GCash.\n";
    }
    std::string getMethodName() const override { return "GCash"; }
};

// Payment Context (Singleton)
class PaymentContext {
private:
    static PaymentContext* instance;
    PaymentStrategy* strategy;

    PaymentContext() : strategy(nullptr) {}
    PaymentContext(const PaymentContext&) = delete;
    PaymentContext& operator=(const PaymentContext&) = delete;

public:
    static PaymentContext* getInstance() {
        if (!instance) {
            instance = new PaymentContext();
        }
        return instance;
    }

    void setStrategy(PaymentStrategy* newStrategy) {
        if (strategy) delete strategy;
        strategy = newStrategy;
    }

    void executePayment(float amount) {
        if (strategy) {
            strategy->pay(amount);
        } else {
            throw std::runtime_error("Payment strategy not set.");
        }
    }

    std::string getPaymentMethod() const {
        if (strategy) {
            return strategy->getMethodName();
        }
        return "Unknown";
    }

    ~PaymentContext() {
        delete strategy;
    }
};

PaymentContext* PaymentContext::instance = nullptr;

// E-Commerce System
class ECommerceSystem {
private:
    Product* products;
    int productCount;
    CartItem* cart;
    int cartSize;
    int cartCapacity;
    Order** orders;
    int orderCount;
    int orderCapacity;

    void resizeCart() {
        int newCapacity = cartCapacity == 0 ? 5 : cartCapacity * 2;
        CartItem* newCart = new CartItem[newCapacity];
        for (int i = 0; i < cartSize; i++) {
            newCart[i] = cart[i];
        }
        delete[] cart;
        cart = newCart;
        cartCapacity = newCapacity;
    }

    void resizeOrders() {
        int newCapacity = orderCapacity == 0 ? 5 : orderCapacity * 2;
        Order** newOrders = new Order*[newCapacity];
        for (int i = 0; i < orderCount; i++) {
            newOrders[i] = orders[i];
        }
        delete[] orders;
        orders = newOrders;
        orderCapacity = newCapacity;
    }

    void initializeProducts() {
        productCount = 6;
        products = new Product[productCount];
        products[0] = Product("1", "Paper", 20);
        products[1] = Product("2", "Pencil", 10);
        products[2] = Product("3", "Notebook", 50);
        products[3] = Product("4", "Eraser", 5);
        products[4] = Product("5", "Stapler", 30);
        products[5] = Product("6", "Ruler", 15);
    }

    void addToCart(const Product& product, int quantity) {
        for (int i = 0; i < cartSize; i++) {
            if (cart[i].getProduct().getId() == product.getId()) {
                cart[i].setQuantity(cart[i].getQuantity() + quantity);
                return;
            }
        }

        if (cartSize >= cartCapacity) {
            resizeCart();
        }
        cart[cartSize++] = CartItem(product, quantity);
    }

    void checkout() {
        if (cartSize == 0) {
            std::cout << "Your shopping cart is empty!\n";
            return;
        }

        std::cout << "\nProducts for Checkout:\n";
        std::cout << std::left
                  << std::setw(12) << "Product ID"
                  << std::setw(20) << "Name"
                  << std::setw(10) << "Price"
                  << std::setw(10) << "Quantity"
                  << std::setw(10) << "Total" << "\n";
        float totalAmount = 0;
        for (int i = 0; i < cartSize; i++) {
            const Product& p = cart[i].getProduct();
            int qty = cart[i].getQuantity();
            float itemTotal = p.getPrice() * qty;
            totalAmount += itemTotal;
            std::cout << std::left
                      << std::setw(12) << p.getId()
                      << std::setw(20) << p.getName()
                      << std::setw(10) << p.getPrice()
                      << std::setw(10) << qty
                      << std::setw(10) << itemTotal << "\n";
        }
        std::cout << "Total Amount: " << totalAmount << "\n";

        PaymentContext* paymentContext = PaymentContext::getInstance();
        
        bool paymentSelected = false;
PaymentStrategy* strategy = nullptr;
while (!paymentSelected) {
    std::cout << "\nSelect Payment Method:\n";
    std::cout << "1. Cash\n";
    std::cout << "2. Card\n";
    std::cout << "3. GCash\n";
    std::cout << "Enter choice (1-3): ";

    std::string input;
    std::getline(std::cin, input);

    // Trim leading/trailing whitespace (optional but recommended)
    input.erase(0, input.find_first_not_of(" \t"));
    input.erase(input.find_last_not_of(" \t") + 1);

    try {
        size_t pos;
        int choice = std::stoi(input, &pos);

        // Check if entire string was converted (rejects "1 1", "2abc")
        if (pos != input.length()) {
            throw std::invalid_argument("Invalid input");
        }

        switch (choice) {
            case 1: strategy = new CashPayment(); break;
            case 2: strategy = new CardPayment(); break;
            case 3: strategy = new GCashPayment(); break;
            default:
                std::cout << "Invalid choice. Please enter 1, 2, or 3.\n";
                continue;
        }
        paymentContext->setStrategy(strategy);
        paymentSelected = true;
    } catch (...) {
        std::cout << "Invalid input. Please enter a number (1-3).\n";
    }
}

        try {
            paymentContext->executePayment(totalAmount);
            
            OrderItem* orderItems = new OrderItem[cartSize];
            for (int i = 0; i < cartSize; i++) {
                const Product& p = cart[i].getProduct();
                orderItems[i] = OrderItem(p.getId(), p.getName(), p.getPrice(), cart[i].getQuantity());
            }

            if (orderCount >= orderCapacity) {
                resizeOrders();
            }
            orders[orderCount++] = new Order(totalAmount, paymentContext->getPaymentMethod(), orderItems, cartSize);

            std::ofstream logFile("orders_log.txt", std::ios::app);
            if (logFile.is_open()) {
                logFile << "[LOG] -> Order ID: " << orders[orderCount-1]->getOrderId() 
                         << " has been successfully checked out and paid using " 
                         << paymentContext->getPaymentMethod() << ".\n";
                logFile.close();
            }

            delete[] cart;
            cart = nullptr;
            cartSize = 0;
            cartCapacity = 0;

            std::cout << "\nYou have successfully checked out the products!\n";
        } catch (const std::exception& e) {
            std::cout << "Payment failed: " << e.what() << "\n";
        }
    }

public:
    ECommerceSystem() 
        : products(nullptr), productCount(0),
          cart(nullptr), cartSize(0), cartCapacity(0),
          orders(nullptr), orderCount(0), orderCapacity(0) {
        initializeProducts();
    }

    ~ECommerceSystem() {
        delete[] products;
        delete[] cart;
        for (int i = 0; i < orderCount; i++) {
            delete orders[i];
        }
        delete[] orders;
    }

    void viewProducts() {
        std::cout << "\nAvailable Products:\n";
        std::cout << std::left
                  << std::setw(12) << "Product ID"
                  << std::setw(20) << "Name"
                  << std::setw(10) << "Price" << "\n";
        for (int i = 0; i < productCount; i++) {
            std::cout << std::left
                      << std::setw(12) << products[i].getId()
                      << std::setw(20) << products[i].getName()
                      << std::setw(10) << products[i].getPrice() << "\n";
        }

        bool addingProducts = true;
        while (addingProducts) {
            std::cout << "\nEnter the ID of the product you want to add to the shopping cart: ";
            std::string productId;
            std::getline(std::cin, productId);

            Product* selectedProduct = nullptr;
            for (int i = 0; i < productCount; i++) {
                if (products[i].getId() == productId) {
                    selectedProduct = &products[i];
                    break;
                }
            }

            if (!selectedProduct) {
                std::cout << "Invalid product ID. Please try again.\n";
                continue;
            }

            bool validQuantity = false;
int quantity = 0;
while (!validQuantity) {
    std::cout << "Enter quantity: ";
    std::string input;
    std::getline(std::cin, input);

    // Check if the entire string is a valid positive integer
    bool is_valid = true;
    for (char c : input) {
        if (!isdigit(c)) {  // Reject if any non-digit character exists
            is_valid = false;
            break;
        }
    }

    if (is_valid && !input.empty()) {
        quantity = std::stoi(input);
        if (quantity > 0) {
            validQuantity = true;
        } else {
            std::cout << "Quantity must be positive. Please try again.\n";
        }
    } else {
        std::cout << "Invalid quantity. Please enter a positive number without spaces or symbols.\n";
    }
}

            addToCart(*selectedProduct, quantity);
            std::cout << "Product added successfully!\n";

            char choice = ' ';
            bool validChoice = false;
            while (!validChoice) {
                std::cout << "Do you want to add another product? (Y/N): ";
                std::string input;
                std::getline(std::cin, input);
                
                // Trim whitespace from input
                input.erase(0, input.find_first_not_of(" \t"));
                input.erase(input.find_last_not_of(" \t") + 1);
                
                if (input.size() == 1) {
                    choice = toupper(input[0]);
                    if (choice == 'Y' || choice == 'N') {
                        validChoice = true;
                    }
                }
                
                if (!validChoice) {
                    std::cout << "Invalid input. Please enter exactly Y or N.\n";
                }
            }
            
            addingProducts = (choice == 'Y');
        }
    }
        

    void viewShoppingCart() {
        if (cartSize == 0) {
            std::cout << "\nYour shopping cart is empty.\n";
            return;
        }

        std::cout << "\nShopping Cart:\n";
        std::cout << std::left
                  << std::setw(12) << "Product ID"
                  << std::setw(20) << "Name"
                  << std::setw(10) << "Price"
                  << std::setw(10) << "Quantity"
                  << std::setw(10) << "Total" << "\n";
        float total = 0;
        for (int i = 0; i < cartSize; i++) {
            const Product& p = cart[i].getProduct();
            int qty = cart[i].getQuantity();
            float itemTotal = p.getPrice() * qty;
            total += itemTotal;
            std::cout << std::left
                      << std::setw(12) << p.getId()
                      << std::setw(20) << p.getName()
                      << std::setw(10) << p.getPrice()
                      << std::setw(10) << qty
                      << std::setw(10) << itemTotal << "\n";
        }
        std::cout << "Total: " << total << "\n";

        char choice = 'N';
bool validChoice = false;

while (!validChoice) {
    std::cout << "Do you want to check out all the products? (Y/N): ";
    std::string input;
    std::getline(std::cin, input);

    // Trim leading/trailing whitespace
    input.erase(0, input.find_first_not_of(" \t"));
    input.erase(input.find_last_not_of(" \t") + 1);

    // Check if input is exactly 1 character (Y/N, case-insensitive)
    if (input.length() == 1) {
        choice = toupper(input[0]);
        if (choice == 'Y' || choice == 'N') {
            validChoice = true;
        }
    }

    if (!validChoice) {
        std::cout << "Invalid choice. Please enter exactly Y or N.\n";
    }
}

if (choice == 'Y') {
    checkout();
} }

    void viewOrders() {
        if (orderCount == 0) {
            std::cout << "\nNo orders have been placed yet.\n";
            return;
        }

        std::cout << "\nOrder History:\n";
        for (int i = 0; i < orderCount; i++) {
            orders[i]->display();
        }
    }

    void run() {
        bool running = true;
        while (running) {
            std::cout << "\n=== E-Commerce System Menu ===\n";
            std::cout << "1. View Products\n";
            std::cout << "2. View Shopping Cart\n";
            std::cout << "3. View Orders\n";
            std::cout << "4. Exit\n";
            std::cout << "Enter your choice (1-4): ";
    
            std::string input;
            std::getline(std::cin, input);
    
            // --- Start of Input Validation ---
            bool isValid = true;
            if (input.empty()) {
                isValid = false;
            } else {
                // Check every character is a digit (no spaces, letters, etc.)
                for (char c : input) {
                    if (!isdigit(c)) {
                        isValid = false;
                        break;
                    }
                }
                // If valid, convert to integer and check range
                if (isValid) {
                    int choice = 0;
                    for (char c : input) {  // Manual string-to-int conversion
                        choice = choice * 10 + (c - '0');
                    }
                    if (choice >= 1 && choice <= 4) {
                        switch (choice) {
                            case 1: viewProducts(); break;
                            case 2: viewShoppingCart(); break;
                            case 3: viewOrders(); break;
                            case 4: 
                                std::cout << "Thank you for using our e-commerce system!\n";
                                running = false;
                                break;
                            default:  // Handled by range check above
                                break;
                        }
                    } else {
                        std::cout << "Invalid choice. Please enter a number (1-4).\n";
                    }
                } else {
                    std::cout << "Invalid input. Please enter a number (1-4).\n";
                }
            }
            // --- End of Input Validation ---
        }
    }
};
int main() {
    ECommerceSystem system;
    system.run();
    return 0;
}