#pragma once
#include <iostream>

// Parent class
class A
{
public:
    A() = default;

    virtual ~A() = default;

    virtual void print() const = 0;
};

// First derived class
class BA : public A
{
    int v1_;
    int v2_;

public:
    BA( int v1, int v2 ) : A(), v1_(v1), v2_(v2) {}

    virtual void print() const override
    {
        std::cout << "BA (v1 = " << v1_ << ", v2 = " << v2_ << ")" << std::endl;
    }
};


// Second derived class
class CA : public A
{
    int v1Multiplied_;
    int v2Multiplied_;

public:
    CA( int v1, int v2 ) : v1Multiplied_( v1 * 10 ), v2Multiplied_( v2 * 10 ) {}

    virtual void print() const override
    {
        std::cout << "CA (v1 = " << v1Multiplied_ << ", v2 = " << v2Multiplied_ << ")" << std::endl;
    }
};


// Example without arguments -----------------------------------

class AwithoutArgs
{
public:
    virtual ~AwithoutArgs() = default;

    virtual void print() const = 0;
};

class BAwithoutArgs : public AwithoutArgs
{
    int v_ { 10 };

public:
    BAwithoutArgs() = default;

    virtual void print() const override
    {
        std::cout << "BA v = " << v_ << std::endl;
    }
};

class CAwithoutArgs : public AwithoutArgs
{
    int v_ { 20 };

public:
    CAwithoutArgs() = default;

    virtual void print() const override
    {
        std::cout << "CA v = " << v_ << std::endl;
    }
};

