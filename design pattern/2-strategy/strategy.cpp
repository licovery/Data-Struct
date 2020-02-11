// if else可扩展时，考虑使用策略模式，把多个分支算法子类化

class Context
{
};

class TaxStrategy
{
public:
    virtual double calculate(const Context &context) = 0;
    virtual ~TaxStrategy() {}
};

class CNTax : public TaxStrategy
{
    virtual double calculate(const Context &context) override
    {
        // ******
    }
};

class USTax : public TaxStrategy
{
    virtual double calculate(const Context &context) override
    {
        // ******
    }
};

// 可以不断扩展

class SalesOrder
{
private:
    TaxStrategy *strategy;

public:
    // 利用工厂生成对象
    SalesOrder(StrategyFactory *strategyFactory)
    {
        this->strategy = strategyFactory->NewStrategy();
    }
    ~SalesOrder()
    {
        delete this->strategy;
    }

    double CalculateTax()
    {
        //...
        Context context;

        double val = strategy->calculate(context); //多态调用
        //...
    }
};