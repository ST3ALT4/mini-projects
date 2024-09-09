#include <iostream>
#include <string>
#include <unordered_map>
#include <random>

using namespace std;

class Account
{
private:
    unsigned int accountno;
    long long balance;
    string holder_name;
    int pin = -1;

public:
    Account(string name)
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distr(100000, 999999);

        accountno = distr(gen);
        balance = 0;
        holder_name = name;
    }

    void info() const
    {
        cout << accountno << endl
             << balance << endl
             << holder_name << endl;
    }

    unsigned int acc() const
    {
        return accountno;
    }

    bool mtpin() const
    {
        if (pin == -1)
        {
            cout << "Please set PIN" << endl;
            return false;
        }
        int val_pin = -1;
        cout << "Enter your PIN" << endl;
        cin >> val_pin;
        return val_pin == pin;
    }

    void setmtpin()
    {
        int new_pin = -1;
        cout << "Enter your new PIN" << endl;
        cin >> new_pin;
        if (new_pin >= 1000 && new_pin <= 9999)
        {
            pin = new_pin;
        }
        else
        {
            cout << "Enter a valid PIN" << endl;
            setmtpin();
        }
    }

    void deposit()
    {
        int val = 0;
        cout<<"Enter Amount"<<endl;
        cin >> val;
        balance += val;
        cout << "Your current balance is " << balance << endl;
    }

    void withdraw()
    {
        if (mtpin())
        {
            int val = 0;
            cout << "Enter amount" << endl;
            cin >> val;
            if (balance - val >= 0)
            {
                balance -= val;
                cout << "Your current balance is " << balance << endl;
            }
            else
            {
                cout << "Insufficient balance" << endl;
            }
        }
        else
        {
            cout << "PIN incorrect" << endl;
        }
    }

    void run()
    {
        while (true)
        {
            cout << "1. Account Info" << endl
                 << "2. Deposit" << endl
                 << "3. Withdraw" << endl
                 << "4. Exit" << endl;

            int menu_num = -1;
            cin >> menu_num;
            switch (menu_num)
            {
            case 1:
                info();
                break;
            case 2:
                deposit();
                break;
            case 3:
                withdraw();
                break;
            case 4:
                return;
            default:
                cout << "Enter a valid input" << endl;
                break;
            }
        }
    }
};

int main()
{
    unordered_map<unsigned int, Account *> account_map;

    while (true)
    {
        cout << "1. Create New" << endl
             << "2. Already Exist" << endl
             <<"3. Exit"<<endl;
        int menu = -1;
        cin >> menu;

        if (menu == 1)
        {
            string name;
            cout << "Enter your name" << endl;
            cin >> name;

            Account *myAccount = new Account(name);
            unsigned int acc_no = myAccount->acc();
            account_map[acc_no] = myAccount;
            cout << "Your Account number is " << acc_no << endl;
            myAccount->setmtpin();
        }
        else if (menu == 2)
        {
            unsigned int acc_num;
            cout << "Enter account number" << endl;
            cin >> acc_num;
            auto it = account_map.find(acc_num);
            if (it != account_map.end())
            {
                if (it->second->mtpin())
                {
                    it->second->run();
                }
                else
                {
                    cout << "PIN incorrect" << endl;
                }
            }
            else
            {
                cout << "Account doesn't exist" << endl;
            }
        }else if(menu == 3){
            break;
        }
        else
        {
            cout << "Invalid menu option" << endl;
        }
    }

    // Clean up dynamically allocated memory
    for (auto &pair : account_map)
    {
        delete pair.second;
    }

    return 0;
}
