#pragma once

// you say you don't have the std namespace available
// if you really don't have std::move or std::forward, just write them yourself
enum class EitherSide { left, right };
struct LeftTagT {};
struct RightTagT {};

template<class L, class R>
class Either {
    // self explanatory
    EitherSide tag;
    union {
        L left;
        R right;
    };

    public:
    // "in-place" constructors: according to the tag, initialize with whatever arguments
    template<class... U>
    Either(LeftTagT, U&&... args) : tag(EitherSide::left), left(std::forward<U>(args)...) { }
    template<class... U>
    Either(RightTagT, U&&... args) : tag(EitherSide::right), right(std::forward<U>(args)...) { }
    // may want std::initializer_list "forwarding" constructors, too

    // initialize by type
    Either(L x) : Either(LeftTagT(), std::move(x)) { }
    Either(R x) : Either(RightTagT(), std::move(x)) { }

    // sometimes we don't know which variant member to initialize immediately
    // so we leave the union uninitialized and then placement-new into it
    Either(Either const &other) : tag(other.tag) {
       if(tag == EitherSide::left) new(&left) L(other.left);
       else new(&right) R(other.right);
    }
    Either(Either &&other) : tag(other.tag) {
       if(tag == EitherSide::left) new(&left) L(std::move(other.left));
       else new(&right) R(std::move(other.right));
    }
    // and when we need to destroy the object, we must explicitly call the destructor
    Either &operator=(Either const &other) {
       if(tag == EitherSide::left && other.tag == EitherSide::left) left = other.left;
       else if(tag == EitherSide::right && other.tag == EitherSide::right) right = other.right;
       else if(tag == EitherSide::left && other.tag == EitherSide::right) {
           left.~L();
           // NOT exception safe! if this constructor were to throw and ~either to be called during the unwinding
           // then ~either would see the tag set but neither object constructed
           // and would try to destroy it a second time
           // ...but you say we don't have exceptions, so I suppose we can be sloppy (the standard library's solution is to add a third tag meaning "empty because of exception" to std::variant)
           new(&right) R(other.right);
       } else {
           right.~R();
           new(&left) L(other.left);
       }
       tag = other.tag;
    }
    Either &operator=(Either &&other) {
       if(tag == EitherSide::left && other.tag == EitherSide::left) left = std::move(other.left);
       else if(tag == EitherSide::right && other.tag == EitherSide::right) right = std::move(other.right);
       else if(tag == EitherSide::left && other.tag == EitherSide::right) {
           left.~L();
           new(&right) R(std::move(other.right));
       } else {
           right.~R();
           new(&left) L(std::move(other.left));
       }
       tag = other.tag;
    }
    ~Either() {
        if(tag == EitherSide::left) left.~L();
        else right.~R();
    }

    L *getIfLeft() { return tag == EitherSide::left ? &left : nullptr; }
    L const *getIfLeft() const { return tag == EitherSide::left ? &left : nullptr; }
    R *getIfRight() { return tag == EitherSide::right ? &right : nullptr; }
    R const *getIfRight() const { return tag == EitherSide::right ? &right : nullptr; }
    bool isLeft() const { return bool(getIfLeft()); }
    bool isRight() const { return bool(getIfRight()); }
};