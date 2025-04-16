// CRM System Implementation
// main.cpp

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <ctime>
#include <map>
#include <algorithm>

// Forward declarations
class Customer;
class Interaction;
class SalesRepresentative;

// Abstract base class for Interaction (Abstraction)
class Interaction {
protected:
    std::string date;
    std::string content;
    std::string type;

public:
    // Constructor
    Interaction(const std::string& content) 
        : content(content) {
        // Get current time for the interaction
        time_t now = time(nullptr);
        tm* ltm = localtime(&now);
        
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
        date = std::string(buffer);
    }
    
    // Virtual destructor for proper inheritance
    virtual ~Interaction() = default;
    
    // Pure virtual method (Abstraction)
    virtual void display() const = 0;
    
    // Getters (Encapsulation)
    std::string getDate() const { return date; }
    std::string getContent() const { return content; }
    std::string getType() const { return type; }
};

// Derived class for Call interaction (Inheritance)
class Call : public Interaction {
private:
    int duration; // in minutes

public:
    Call(const std::string& content, int duration) 
        : Interaction(content), duration(duration) {
        type = "Call";
    }
    
    // Implementation of pure virtual method (Polymorphism)
    void display() const override {
        std::cout << "Call on " << date << " (Duration: " << duration << " minutes): " << content << std::endl;
    }
    
    int getDuration() const { return duration; }
};

// Derived class for Email interaction (Inheritance)
class Email : public Interaction {
private:
    std::string subject;

public:
    Email(const std::string& content, const std::string& subject) 
        : Interaction(content), subject(subject) {
        type = "Email";
    }
    
    // Implementation of pure virtual method (Polymorphism)
    void display() const override {
        std::cout << "Email on " << date << " (Subject: " << subject << "): " << content << std::endl;
    }
    
    std::string getSubject() const { return subject; }
};

// Derived class for Meeting interaction (Inheritance)
class Meeting : public Interaction {
private:
    std::string location;
    int duration; // in minutes

public:
    Meeting(const std::string& content, const std::string& location, int duration) 
        : Interaction(content), location(location), duration(duration) {
        type = "Meeting";
    }
    
    // Implementation of pure virtual method (Polymorphism)
    void display() const override {
        std::cout << "Meeting on " << date << " at " << location 
                  << " (Duration: " << duration << " minutes): " << content << std::endl;
    }
    
    std::string getLocation() const { return location; }
    int getDuration() const { return duration; }
};

// Abstract base class for Customer (Abstraction)
class Customer {
protected:
    int id;
    std::string name;
    std::string email;
    std::string phone;
    std::vector<std::shared_ptr<Interaction>> interactions;
    std::string type;

public:
    // Constructor
    Customer(int id, const std::string& name, const std::string& email, const std::string& phone)
        : id(id), name(name), email(email), phone(phone) {}
    
    // Virtual destructor
    virtual ~Customer() = default;
    
    // Add interaction
    void addInteraction(const std::shared_ptr<Interaction>& interaction) {
        interactions.push_back(interaction);
    }
    
    // Display interactions
    void displayInteractions() const {
        if (interactions.empty()) {
            std::cout << "No interactions recorded for " << name << std::endl;
            return;
        }
        
        std::cout << "Interactions for " << name << " (" << type << "):" << std::endl;
        for (const auto& interaction : interactions) {
            interaction->display();
        }
    }
    
    // Pure virtual method for customer-specific actions (Abstraction)
    virtual void performCustomerSpecificAction() const = 0;
    
    // Getters (Encapsulation)
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }
    std::string getType() const { return type; }
    
    // Calculate total interaction time (polymorphic behavior)
    virtual int calculateTotalInteractionTime() const {
        int total = 0;
        for (const auto& interaction : interactions) {
            if (auto call = std::dynamic_pointer_cast<Call>(interaction)) {
                total += call->getDuration();
            } else if (auto meeting = std::dynamic_pointer_cast<Meeting>(interaction)) {
                total += meeting->getDuration();
            }
        }
        return total;
    }
};

// Derived class for Regular Customer (Inheritance)
class RegularCustomer : public Customer {
private:
    std::string segment;

public:
    RegularCustomer(int id, const std::string& name, const std::string& email, 
                    const std::string& phone, const std::string& segment)
        : Customer(id, name, email, phone), segment(segment) {
        type = "Regular";
    }
    
    // Implementation of pure virtual method (Polymorphism)
    void performCustomerSpecificAction() const override {
        std::cout << "Sending regular promotional materials to " << name << " in segment " << segment << std::endl;
    }
    
    // Getter
    std::string getSegment() const { return segment; }
};

// Derived class for VIP Customer (Inheritance)
class VIPCustomer : public Customer {
private:
    std::string accountManager;
    double loyaltyPoints;

public:
    VIPCustomer(int id, const std::string& name, const std::string& email, 
                const std::string& phone, const std::string& accountManager)
        : Customer(id, name, email, phone), accountManager(accountManager), loyaltyPoints(0) {
        type = "VIP";
    }
    
    // Implementation of pure virtual method (Polymorphism)
    void performCustomerSpecificAction() const override {
        std::cout << "Scheduling quarterly review with " << name 
                  << " and account manager " << accountManager << std::endl;
    }
    
    void addLoyaltyPoints(double points) {
        loyaltyPoints += points;
        std::cout << "Added " << points << " loyalty points to " << name 
                  << ". Total: " << loyaltyPoints << std::endl;
    }
    
    // Override to provide VIP-specific calculation (Polymorphism)
    int calculateTotalInteractionTime() const override {
        // VIP gets a 20% boost to interaction time calculation for reporting purposes
        return static_cast<int>(Customer::calculateTotalInteractionTime() * 1.2);
    }
    
    // Getters
    std::string getAccountManager() const { return accountManager; }
    double getLoyaltyPoints() const { return loyaltyPoints; }
};

// Derived class for Corporate Customer (Inheritance)
class CorporateCustomer : public Customer {
private:
    std::string companyName;
    int numberOfEmployees;
    double annualContract;

public:
    CorporateCustomer(int id, const std::string& name, const std::string& email, 
                     const std::string& phone, const std::string& companyName, 
                     int numberOfEmployees, double annualContract)
        : Customer(id, name, email, phone), companyName(companyName), 
          numberOfEmployees(numberOfEmployees), annualContract(annualContract) {
        type = "Corporate";
    }
    
    // Implementation of pure virtual method (Polymorphism)
    void performCustomerSpecificAction() const override {
        std::cout << "Arranging corporate training session for " << companyName 
                  << " with " << numberOfEmployees << " potential users" << std::endl;
    }
    
    void renewContract(double newAmount) {
        std::cout << "Renewing contract for " << companyName << ". Old amount: $" 
                  << annualContract << ", New amount: $" << newAmount << std::endl;
        annualContract = newAmount;
    }
    
    // Override to provide Corporate-specific calculation (Polymorphism)
    int calculateTotalInteractionTime() const override {
        // Corporate gets a multiplier based on company size
        double multiplier = 1.0;
        if (numberOfEmployees > 1000)
            multiplier = 1.5;
        else if (numberOfEmployees > 100)
            multiplier = 1.3;
        
        return static_cast<int>(Customer::calculateTotalInteractionTime() * multiplier);
    }
    
    // Getters
    std::string getCompanyName() const { return companyName; }
    int getNumberOfEmployees() const { return numberOfEmployees; }
    double getAnnualContract() const { return annualContract; }
};

// SalesRepresentative class
class SalesRepresentative {
private:
    int id;
    std::string name;
    std::vector<std::shared_ptr<Customer>> customers;
    
    // Private method for finding a customer (Encapsulation)
    std::shared_ptr<Customer> findCustomer(int customerId) {
        auto it = std::find_if(customers.begin(), customers.end(),
            [customerId](const std::shared_ptr<Customer>& c) { return c->getId() == customerId; });
        
        if (it != customers.end())
            return *it;
            
        return nullptr;
    }

public:
    SalesRepresentative(int id, const std::string& name)
        : id(id), name(name) {}
        
    // Add a customer to the rep's portfolio
    void addCustomer(const std::shared_ptr<Customer>& customer) {
        customers.push_back(customer);
    }
    
    // Record a call with a customer
    void recordCall(int customerId, const std::string& content, int duration) {
        auto customer = findCustomer(customerId);
        if (customer) {
            auto call = std::make_shared<Call>(content, duration);
            customer->addInteraction(call);
            std::cout << "Call recorded with " << customer->getName() << std::endl;
            
            // Add loyalty points for VIP customers
            if (auto vipCustomer = std::dynamic_pointer_cast<VIPCustomer>(customer)) {
                vipCustomer->addLoyaltyPoints(duration * 0.5);
            }
        } else {
            std::cout << "Customer not found." << std::endl;
        }
    }
    
    // Record an email to a customer
    void recordEmail(int customerId, const std::string& content, const std::string& subject) {
        auto customer = findCustomer(customerId);
        if (customer) {
            auto email = std::make_shared<Email>(content, subject);
            customer->addInteraction(email);
            std::cout << "Email recorded with " << customer->getName() << std::endl;
            
            // Add loyalty points for VIP customers
            if (auto vipCustomer = std::dynamic_pointer_cast<VIPCustomer>(customer)) {
                vipCustomer->addLoyaltyPoints(10);
            }
        } else {
            std::cout << "Customer not found." << std::endl;
        }
    }
    
    // Record a meeting with a customer
    void recordMeeting(int customerId, const std::string& content, 
                      const std::string& location, int duration) {
        auto customer = findCustomer(customerId);
        if (customer) {
            auto meeting = std::make_shared<Meeting>(content, location, duration);
            customer->addInteraction(meeting);
            std::cout << "Meeting recorded with " << customer->getName() << std::endl;
            
            // Add loyalty points for VIP customers
            if (auto vipCustomer = std::dynamic_pointer_cast<VIPCustomer>(customer)) {
                vipCustomer->addLoyaltyPoints(duration * 2);
            }
        } else {
            std::cout << "Customer not found." << std::endl;
        }
    }
    
    // Perform customer-specific actions for all customers
    void performCustomerActions() {
        for (const auto& customer : customers) {
            customer->performCustomerSpecificAction();
        }
    }
    
    // Display customers
    void displayCustomers() const {
        if (customers.empty()) {
            std::cout << "No customers assigned to " << name << std::endl;
            return;
        }
        
        std::cout << "Customers assigned to " << name << ":" << std::endl;
        for (const auto& customer : customers) {
            std::cout << "ID: " << customer->getId() 
                      << ", Name: " << customer->getName()
                      << ", Type: " << customer->getType() << std::endl;
        }
    }
    
    // View customer interactions
    void viewCustomerInteractions(int customerId) {
        auto customer = findCustomer(customerId);
        if (customer) {
            customer->displayInteractions();
        } else {
            std::cout << "Customer not found." << std::endl;
        }
    }
    
    // Generate a report of total interaction times
    void generateInteractionTimeReport() {
        std::cout << "\nInteraction Time Report for Sales Rep: " << name << "\n";
        std::cout << "----------------------------------------\n";
        
        for (const auto& customer : customers) {
            int totalTime = customer->calculateTotalInteractionTime();
            std::cout << "Customer: " << customer->getName() 
                      << " (" << customer->getType() << ")"
                      << " - Total Interaction Time: " << totalTime << " minutes\n";
        }
        std::cout << "----------------------------------------\n";
    }
    
    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    const std::vector<std::shared_ptr<Customer>>& getCustomers() const { return customers; }
};

// CRM class to manage the overall system
class CRM {
private:
    std::vector<std::shared_ptr<Customer>> customers;
    std::vector<std::shared_ptr<SalesRepresentative>> salesReps;
    int nextCustomerId;
    int nextSalesRepId;

public:
    CRM() : nextCustomerId(1), nextSalesRepId(1) {}
    
    // Create a regular customer
    std::shared_ptr<RegularCustomer> createRegularCustomer(
        const std::string& name, const std::string& email, 
        const std::string& phone, const std::string& segment) {
        
        auto customer = std::make_shared<RegularCustomer>(
            nextCustomerId++, name, email, phone, segment);
        customers.push_back(customer);
        return customer;
    }
    
    // Create a VIP customer
    std::shared_ptr<VIPCustomer> createVIPCustomer(
        const std::string& name, const std::string& email, 
        const std::string& phone, const std::string& accountManager) {
        
        auto customer = std::make_shared<VIPCustomer>(
            nextCustomerId++, name, email, phone, accountManager);
        customers.push_back(customer);
        return customer;
    }
    
    // Create a corporate customer
    std::shared_ptr<CorporateCustomer> createCorporateCustomer(
        const std::string& name, const std::string& email, 
        const std::string& phone, const std::string& companyName,
        int numberOfEmployees, double annualContract) {
        
        auto customer = std::make_shared<CorporateCustomer>(
            nextCustomerId++, name, email, phone, companyName, 
            numberOfEmployees, annualContract);
        customers.push_back(customer);
        return customer;
    }
    
    // Create a sales representative
    std::shared_ptr<SalesRepresentative> createSalesRepresentative(const std::string& name) {
        auto rep = std::make_shared<SalesRepresentative>(nextSalesRepId++, name);
        salesReps.push_back(rep);
        return rep;
    }
    
    // Assign a customer to a sales representative
    void assignCustomerToRep(int customerId, int repId) {
        std::shared_ptr<Customer> customer = nullptr;
        std::shared_ptr<SalesRepresentative> rep = nullptr;
        
        // Find the customer
        for (const auto& c : customers) {
            if (c->getId() == customerId) {
                customer = c;
                break;
            }
        }
        
        // Find the sales rep
        for (const auto& r : salesReps) {
            if (r->getId() == repId) {
                rep = r;
                break;
            }
        }
        
        // Assign if both exist
        if (customer && rep) {
            rep->addCustomer(customer);
            std::cout << "Customer " << customer->getName() 
                      << " assigned to " << rep->getName() << std::endl;
        } else {
            std::cout << "Customer or Sales Rep not found." << std::endl;
        }
    }
    
    // Display all customers
    void displayAllCustomers() const {
        if (customers.empty()) {
            std::cout << "No customers in the system." << std::endl;
            return;
        }
        
        std::cout << "All Customers:" << std::endl;
        for (const auto& customer : customers) {
            std::cout << "ID: " << customer->getId() 
                      << ", Name: " << customer->getName()
                      << ", Type: " << customer->getType() << std::endl;
        }
    }
    
    // Display all sales representatives
    void displayAllSalesReps() const {
        if (salesReps.empty()) {
            std::cout << "No sales representatives in the system." << std::endl;
            return;
        }
        
        std::cout << "All Sales Representatives:" << std::endl;
        for (const auto& rep : salesReps) {
            std::cout << "ID: " << rep->getId() 
                      << ", Name: " << rep->getName() << std::endl;
        }
    }
    
    // Generate system-wide report
    void generateSystemReport() const {
        std::cout << "\n========== CRM SYSTEM REPORT ==========\n";
        
        // Count customers by type
        std::map<std::string, int> customerCounts;
        int totalInteractionTime = 0;
        
        for (const auto& customer : customers) {
            customerCounts[customer->getType()]++;
            totalInteractionTime += customer->calculateTotalInteractionTime();
        }
        
        std::cout << "Total Customers: " << customers.size() << std::endl;
        for (const auto& pair : customerCounts) {
            std::cout << "  " << pair.first << " Customers: " << pair.second << std::endl;
        }
        
        std::cout << "Total Sales Representatives: " << salesReps.size() << std::endl;
        std::cout << "Total Interaction Time: " << totalInteractionTime << " minutes" << std::endl;
        std::cout << "======================================\n";
    }
};

// Main function to demonstrate the CRM system
int main() {
    // Create a CRM system
    CRM crm;
    
    // Create some customers
    auto regularCustomer = crm.createRegularCustomer(
        "John Doe", "john@example.com", "555-1234", "Small Business");
    
    auto vipCustomer = crm.createVIPCustomer(
        "Jane Smith", "jane@example.com", "555-5678", "Michael Johnson");
    
    auto corporateCustomer = crm.createCorporateCustomer(
        "Bob Anderson", "bob@megacorp.com", "555-9876", "MegaCorp", 
        1500, 50000.00);
    
    // Create sales representatives
    auto salesRep1 = crm.createSalesRepresentative("Alice Thompson");
    auto salesRep2 = crm.createSalesRepresentative("David Wilson");
    
    // Assign customers to sales representatives
    crm.assignCustomerToRep(regularCustomer->getId(), salesRep1->getId());
    crm.assignCustomerToRep(vipCustomer->getId(), salesRep1->getId());
    crm.assignCustomerToRep(corporateCustomer->getId(), salesRep2->getId());
    
    // Display all customers and sales representatives
    crm.displayAllCustomers();
    crm.displayAllSalesReps();
    
    // Record some interactions
    salesRep1->recordCall(regularCustomer->getId(), "Discussed new product features", 15);
    salesRep1->recordEmail(vipCustomer->getId(), "Sending exclusive offer details", "VIP Exclusive Offer");
    salesRep1->recordMeeting(vipCustomer->getId(), "Quarterly review meeting", "Headquarters", 60);
    salesRep2->recordCall(corporateCustomer->getId(), "Technical support for recent installation", 30);
    salesRep2->recordMeeting(corporateCustomer->getId(), "Contract renewal discussion", "Client's Office", 90);
    
    // Display customer interactions
    std::cout << "\n--- Customer Interactions ---\n";
    salesRep1->viewCustomerInteractions(regularCustomer->getId());
    salesRep1->viewCustomerInteractions(vipCustomer->getId());
    salesRep2->viewCustomerInteractions(corporateCustomer->getId());
    
    // Perform customer-specific actions
    std::cout << "\n--- Customer-Specific Actions ---\n";
    salesRep1->performCustomerActions();
    salesRep2->performCustomerActions();
    
    // Corporate-specific action
    corporateCustomer->renewContract(75000.00);
    
    // Generate interaction time reports
    std::cout << "\n--- Interaction Time Reports ---\n";
    salesRep1->generateInteractionTimeReport();
    salesRep2->generateInteractionTimeReport();
    
    // Generate system-wide report
    crm.generateSystemReport();

    return 0;
}