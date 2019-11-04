#include "strvec.h"

std::allocator<std::string> StrVec::alloc;

StrVec::StrVec()
{
    element = lastNext = cap = nullptr;
}

size_t StrVec::size() const
{
    return lastNext - element;
}

size_t StrVec::capacity() const
{
    return cap - element;
}

std::string * StrVec::begin() const
{
    return element;
}

std::string * StrVec::end() const
{
    return lastNext;
}



std::pair<std::string *, std::string *> StrVec::alloc_n_copy(const std::string *b, const std::string *e)
{
    std::string *first = alloc.allocate(e - b);
    std::string *last = uninitialized_copy(b, e, first);
    return {first, last};
}

void StrVec::free()
{
    if (element)
    {
        for (auto it = element; it != lastNext; it++)
        {
            alloc.destroy(it);
        }
        alloc.deallocate(element, capacity());
    }
    element = lastNext = cap = nullptr;
}

StrVec::StrVec(const StrVec &rhs)
{
    std::cout << "StrVec CopyConstruct" << std::endl;
    auto range = alloc_n_copy(rhs.begin(), rhs.end());
    element = range.first;
    cap = lastNext = range.second;
}

StrVec & StrVec::operator=(const StrVec &rhs)
{
    std::cout << "StrVec =" << std::endl;
    auto range = alloc_n_copy(rhs.begin(), rhs.end());
    free();
    element = range.first;
    cap = lastNext = range.second;
    return *this;
}

StrVec::~StrVec()
{
    free();
}


void StrVec::reallocate()
{
    size_t oldSize = size();
    size_t newCap = size() ? size() * 2 : 1;
    std::string *newData = alloc.allocate(newCap);
    for (auto i = 0; i < size(); ++i)
    {
        alloc.construct(newData + i, std::move(element[i]));
    }
    free();
    element = newData;
    lastNext = newData + oldSize;
    cap = newData + newCap;
    
}

void StrVec::check_n_alloc()
{
    if (size() == capacity())
    {
       reallocate();
    }
}


void StrVec::push_back(const std::string &s)
{
    check_n_alloc();
    alloc.construct(lastNext++, s);
}

std::ostream & operator<<(std::ostream &out, const StrVec &v)
{
    out << "cap: " << v.capacity() << " size: " << v.size() << " dataAddr: " << v.element << " vector: ";
    for (auto it = v.begin(); it != v.end(); it++)
    {
        out << *it << " ";
    }
    return out;
}

StrVec::StrVec(StrVec &&rhs) noexcept :element(rhs.element), lastNext(rhs.lastNext), cap(rhs.cap) 
{
    std::cout << "StrVec MoveCopyConstruct" << std::endl;
    //rhs置于可析构状态
    rhs.element = rhs.lastNext = rhs.cap = nullptr;
}

StrVec & StrVec::operator=(StrVec &&rhs) noexcept
{
    std::cout << "StrVec Move=" << std::endl;
    if (&rhs == this)
    {
        return *this;
    }
    free();
    element = rhs.element;
    lastNext = rhs.lastNext;
    cap = rhs.cap;
    //rhs置于可析构状态
    rhs.element = rhs.lastNext = rhs.cap = nullptr;
}


StrVec retStrVec()
{
    return StrVec ();
}

StrVec & StrVec::operator=(std::initializer_list<std::string> il)
{
    std::cout << "initializer_list =" << std::endl;
    auto data = alloc_n_copy(il.begin(), il.end());
    free();
    element = data.first;
    cap = lastNext = data.second;
    return *this;
}

std::string & StrVec::operator[](size_t n)
{
    std::cout << "[]" << std::endl;
    return element[n];
}

const std::string & StrVec::operator[](size_t n) const
{
    std::cout << "[] const" << std::endl;
    return element[n];
}

StrVec & StrVec::operator++()
{
    std::cout << "before++" << std::endl;
    // proc
    return *this;
}


StrVec StrVec::operator++(int)
{
    std::cout << "after++" << std::endl;
    StrVec ret = *this;
    ++(*this); //后置的使用前置的来完成工作。
    return ret;
}

StrVec & StrVec::operator*() const
{
    StrVec *p = new StrVec; // only for test
    return *p;
}

StrVec * StrVec::operator->() const
{
    return & this->operator*();
}