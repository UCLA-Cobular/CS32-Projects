#include <iostream>
#include <string>

using namespace std;


////////////////////////////////////////////////////
/// Investment
////////////////////////////////////////////////////
class Investment {
public:
    Investment(const string &Name, int price) {
        m_name = Name;
        m_purchasePrice = price;
    }

    virtual ~Investment() {};

    string name() const { return m_name; }

    virtual string description() const = 0; // Should print the type of thing

    int purchasePrice() const { return m_purchasePrice; }

    virtual bool fungible() const { return false; }
private:
    string m_name;
    int m_purchasePrice;

};

////////////////////////////////////////////////////
/// Stock
////////////////////////////////////////////////////
class Stock : public Investment {
public:
    Stock(const string &Name, int Price, const string &Ticker) : Investment(Name, Price) { m_ticker = Ticker; }

    virtual ~Stock() { cout << "Destroying " + name() + ", a stock holding" << endl; }

    virtual string description() const { return "stock trading as " + m_ticker; }

    virtual bool fungible() const { return true; }

private:
    string m_ticker;
};

class Painting : public Investment {
public:
    Painting(const string &Name, int Price) : Investment(Name, Price) {}

    virtual ~Painting() { cout << "Destroying " + name() + ", a painting" << endl; }

    virtual string description() const { return "painting"; }
};

class House : public Investment {
public:
    House(const string &Name, int Price) : Investment(Name, Price) {}

    virtual ~House() { cout << "Destroying " + name() + ", a painting" << endl; }

    virtual string description() const { return "house"; }
};


void display(const Investment *inv) {
    cout << inv->name();
    if (inv->fungible())
        cout << " (fungible)";
    cout << ": " << inv->description() << " bought for $" << inv->purchasePrice() << endl;
}

int main() {
    Investment *portfolio[4];
    portfolio[0] = new Painting("Salvator Mundi", 450300000);
    // Stock holdings have a name, value, and m_ticker symbol
    portfolio[1] = new Stock("Lyft", 50000, "LYFT");
    portfolio[2] = new Stock("GameStop", 10000, "GME");
    portfolio[3] = new House("4 Privet Drive", 660000);

    for (int k = 0; k < 4; k++)
        display(portfolio[k]);

    // Clean up the investments before exiting
    cout << "Cleaning up" << endl;
    for (int k = 0; k < 4; k++)
        delete portfolio[k];
}
