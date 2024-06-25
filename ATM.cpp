#include <iostream> 
#include <string> 
#include <vector> 
#include <algorithm> 
#include <limits> 
using namespace std; 
// Function prototypes 
void ATM(); 
void new_account(); 
int account_num_creation(); 
void print(string text); 
void format_on(); 
void format_off(); 
int check_int_input(int min, int max); 
int check_passcode(); 
// Account structure 
struct Account { 
string name; 
string type; 
int acc_number; 
int password; 
int balance; 
}; 
// Additional function prototypes 
void performTransaction(Account &account); 
void performWithdrawal(Account &account); 
void performDeposit(Account &account); 
void performFundTransfer(Account &sourceAccount); 
void printAccountInfo(const Account &account); 
// Global variables 
vector<Account> accounts_vector; 
int choice; 
int num; 
int pass; 
int x = 1; 
string type; 
string cancel; 
// Main function for the program. Prints a welcome message and prompts the user for a choice 
int main() { 
while (true) { 
        print( 
            "Welcome to our bank\n" 
            "You have two choices: \n" 
            "(1) NEW BANK ACCOUNT \n" 
            "(2) ATM \n" 
            "Type 1 or 2: "); 
        choice = check_int_input(1, 2); 
 
        // User choices 
        if (choice == 1) { 
            new_account(); 
        } else if (choice == 2) { 
            ATM(); 
        } 
    } 
    return 0; 
} 
 
// Account number creation function 
int account_num_creation() { 
    return accounts_vector.size() + 1; 
} 
 
// New account creation function 
void new_account() { 
    Account account; 
    print( 
        "Welcome\n" 
        "Enter your name: "); 
    cin >> account.name; 
    cout << endl; 
    print("Create your password: "); 
    account.password = check_passcode(); 
    cout << endl; 
    print("Enter your balance: "); 
    account.balance = check_int_input(0, 100000000); 
    cout << endl; 
 
    // Set account type to GOLD or SILVER 
    account.type = (account.balance >= 1000000) ? "GOLD" : "SILVER"; 
 
    account.acc_number = account_num_creation(); 
    accounts_vector.push_back(account); 
 
    format_on(); 
    cout << "Your account is created successfully\n"; 
    cout << "Your account number is " << account.acc_number << endl; 
    cout << "Your account type is " << account.type << endl; 
    cout << "Your balance is $" << account.balance << endl; 
    cout << endl; 
    format_off(); 
} 
 
// ATM function 
void ATM() { 
    print( 
        "Welcome to the ATM\n" 
        "Enter your account number: "); 
 
    int accountNumber = check_int_input(1, 100); 
    cout << endl; 
 
    auto accountIterator = find_if( 
        accounts_vector.begin(), 
        accounts_vector.end(), 
        [accountNumber](const Account &account) { 
            return accountNumber == account.acc_number; 
        }); 
 
    if (accountIterator == accounts_vector.end()) { 
        print( 
            "Invalid account number\n" 
            "Please try again later\n\n\n"); 
    } else { 
        int attempts = 3; 
        while (attempts > 0) { 
            print("Please enter your PIN number: "); 
            int enteredPin = check_int_input(0, 10000); 
            cout << endl; 
 
            if (enteredPin == accountIterator->password) { 
                performTransaction(*accountIterator); 
                break; 
            } else { 
                cout << "Incorrect PIN. You have " << attempts - 1 << " attempts left.\n"; 
                attempts--; 
            } 
        } 
 
        if (attempts == 0) { 
            print("Too many incorrect attempts. Exiting...\n"); 
        } 
    } 
} 
 
// Additional functions for transaction handling 
void performTransaction(Account &account) { 
    print("Login successful!\n"); 
    printAccountInfo(account); 
 
    while (true) { 
        print( 
            "Choose the action:\n" 
            "(1) Cash Withdrawal\n" 
            "(2) Deposit\n" 
            "(3) Fund Transfer\n" 
            "(4) Exit\n" 
            "Enter your choice: "); 
 
        int choice = check_int_input(1, 4); 
        cout << endl; 
 
        switch (choice) { 
            case 1: 
                performWithdrawal(account); 
                break; 
            case 2: 
                performDeposit(account); 
                break; 
            case 3: 
                performFundTransfer(account); 
                break; 
            case 4: 
                print("Exiting...\n"); 
                return; 
            default: 
                print("Invalid choice. Please try again.\n"); 
        } 
    } 
} 
 
void performWithdrawal(Account &account) { 
    print("Enter the amount to withdraw: "); 
    double withdrawalAmount = check_int_input(0, account.balance); 
    cout << endl; 
 
    if (withdrawalAmount <= account.balance) { 
        account.balance -= withdrawalAmount; 
        format_on(); 
        cout << "Withdrawal successful. Updated balance: $" << account.balance << "\n"; 
        format_off(); 
    } else { 
        print("Insufficient funds. Withdrawal canceled.\n"); 
    } 
} 
 
void performDeposit(Account &account) { 
    print("Enter the amount to deposit: "); 
    double depositAmount = check_int_input(0, numeric_limits<int>::max()); 
    cout << endl; 
 
    account.balance += depositAmount; 
    format_on(); 
    cout << "Deposit successful. Updated balance: $" << account.balance << "\n"; 
    format_off(); 
} 
 
void performFundTransfer(Account &sourceAccount) { 
    print("Enter the account number to transfer funds to: "); 
    int targetAccountNumber = check_int_input(1, 100); 
    cout << endl; 
 
    auto targetAccountIterator = find_if( 
        accounts_vector.begin(), 
        accounts_vector.end(), 
        [targetAccountNumber](const Account &account) { 
            return targetAccountNumber == account.acc_number; 
        }); 
 
    if (targetAccountIterator != accounts_vector.end()) { 
        print("Enter the amount to transfer: "); 
        double transferAmount = check_int_input(0, sourceAccount.balance); 
        cout << endl; 
 
        if (transferAmount <= sourceAccount.balance) { 
            sourceAccount.balance -= transferAmount; 
            targetAccountIterator->balance += transferAmount; 
 
            print("Fund transfer successful.\n"); 
            printAccountInfo(sourceAccount); 
            format_on(); 
            cout << "Updated balance for target account (#" << targetAccountNumber << "): $" << 
targetAccountIterator->balance << "\n"; 
            format_off(); 
        } else { 
            print("Insufficient funds. Fund transfer canceled.\n"); 
        } 
    } else { 
        print("Invalid target account number. Fund transfer canceled.\n"); 
    } 
} 
 
// Print account information 
void printAccountInfo(const Account &account) { 
    format_on(); 
    print("Account Information:\n"); 
    cout << "Account Holder: " << account.name << "\n"; 
    cout << "Account Type: " << ((account.balance >= 1000000) ? "GOLD" : "SILVER") << "\n"; 
    cout << "Account Balance: $" << account.balance << "\n\n"; 
    format_off(); 
} 
 
// Print formatted text 
void print(string text) { 
    cout << "\033[1m\033[32;3m" << text; 
    cout << "\033[0m"; 
} 
 
// Enable text formatting 
void format_on() { 
    cout << "\033[1m\033[32;3m"; 
} 
 
// Disable text formatting 
void format_off() { 
    cout << "\033[0m"; 
} 
 
// Check passcode for correctness and convert to integer 
int check_passcode() { 
    string passcode; 
    cin >> passcode; 
 
    if (passcode.length() != 4) { 
        print( 
            "Passcode must be exactly four digits long.\n" 
            "Please create a valid Passcode: "); 
        return check_passcode(); 
    } 
 
    for (char c : passcode) { 
        if (!isdigit(c)) { 
            print("Passcode must be a four-digit number.\n"); 
            return check_passcode(); 
        } 
    } 
 
    return stoi(passcode); 
} 
 
// Check if user input is between min and max 
int check_int_input(int min, int max) { 
    int input; 
    cin >> input; 
 
    if (cin.fail()) { 
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        print("Invalid input. Please enter a valid number: "); 
        return check_int_input(min, max); 
    } else if (input < min || input > max) { 
        format_on(); 
        cout << "Invalid input. Please try again:"; 
        format_off(); 
        return check_int_input(min, max); 
    } else { 
        return input; 
    } 
} 