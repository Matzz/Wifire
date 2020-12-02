#pragma once

template <class L, class R> 
class Either
{
    virtual bool isLeft();
    virtual bool isRight();

    virtual bool matchLeft(L& x);
    virtual bool matchRight(R& x);
};

template <class L, class R> 
class Left : public Either<L, R> {
public:
    L left;

    Left(L left): left(left) {}
    constexpr bool isLeft() override { return true; }
    constexpr bool isRight() override { return false; }

    bool matchLeft(L& x) override {
        x = left;
        return true;
    }
    bool matchRight(R& x) override { return false; }
};

template <class L, class R> 
class Right : public Either<L, R> {
public:
    R right;

    Right(R right): right(right) {}
    constexpr bool isLeft() override { return false; }
    constexpr bool isRight() override { return true; }

    bool matchLeft(L& x) override { return false; }

    bool matchRight(R& x) override {
        x = right;
        return true;
    }
};