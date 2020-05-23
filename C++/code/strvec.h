#ifndef STRVEC_H
#define STRVEC_H

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <iostream>
#include <initializer_list>

namespace lf_vec
{

class StrVec;
std::ostream &operator<<(std::ostream &out, const StrVec &v);

//移动函数（移动构造或者移动赋值）必须声明为noexpect才能让容器去调用，因为容器没有异常处理

class StrVec
{
public:
    StrVec();
    StrVec(const StrVec &rhs);
    StrVec(StrVec &&rhs) noexcept; //移动构造函数，参数是右值引用，声明为noexpect，不抛出异常。
    StrVec &operator=(const StrVec &rhs);
    StrVec &operator=(StrVec &&rhs) noexcept; //移动赋值运算符，参数是右值引用，声明为noexpect，不抛出异常
    StrVec &operator=(std::initializer_list<std::string> il);
    std::string &operator[](size_t n);
    const std::string &operator[](size_t n) const;
    //operator test start
    StrVec &operator++();
    StrVec operator++(int);
    StrVec &operator*() const;
    StrVec *operator->() const;
    //operator test end
    ~StrVec();
    void push_back(const std::string &s);
    size_t size() const;
    size_t capacity() const;
    std::string *begin() const;
    std::string *end() const;
    friend std::ostream &operator<<(std::ostream &out, const StrVec &v);

private:
    static std::allocator<std::string> alloc;
    void check_n_alloc();
    std::pair<std::string *, std::string *> alloc_n_copy(const std::string *b, const std::string *e);
    void free();
    void reallocate();
    std::string *element;
    std::string *lastNext;
    std::string *cap;
};

StrVec retStrVec();

} // namespace lf_vec

#endif