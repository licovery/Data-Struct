#include "oop.h"
#include <iostream>

namespace lf_oop
{

size_t Qoute::totalBookNum = 0;

Qoute::Qoute(const string &book, double p): price(p), bookNo(book)
{

}

string Qoute::isbn() const
{
    return bookNo;
}

double Qoute::net_price(size_t n) const
{
    return n * price;
}

void Qoute::set(size_t n)
{
    Qoute::totalBookNum = n;
}

size_t Qoute::get()
{
    return Qoute::totalBookNum;
}

void Qoute::debug() const 
{
    cout << "bookNo" << endl << "price" << endl;
}


Disc_qoute::Disc_qoute(const string &book, double p, size_t n, double d): 
Qoute(book, p), minNum(n), discount(d)
{

}

Bulk_qoute::Bulk_qoute(const string &book, double p, size_t n, double d): 
Disc_qoute(book, p, n, d)
{

}

double Bulk_qoute::net_price(size_t n) const
{
    double totalPrice = 0;
    if (n < minNum)
    {
        totalPrice = n * price; 
    }
    else
    {
        totalPrice = n * price * discount;
    }
    return totalPrice;
}

void Bulk_qoute::debug() const
{
    cout << "bookNo" << endl << "price" << endl << "minNum" << endl << "discount" << endl;
}

}