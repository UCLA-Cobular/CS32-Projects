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
