#pragma once

template <class T> 
class Option
{
public:
    virtual bool isDefined();
    virtual bool isEmpty();

    virtual bool match(T& x);
};

template <class T> 
class Some : public Option<T> {
public:
    T value;

    Some(T value): value(value) {}
    constexpr bool isDefined() override { return true; }
    constexpr bool isEmpty() override { return false; }

    bool match(T& x) override {
        x = value;
        return true;
    }
};

template <class T> 
class None : public Option<T> {
public:

    constexpr bool isDefined() override { return false; }
    constexpr bool isEmpty() override { return true; }

    bool match(T& x) override { return false; }
};