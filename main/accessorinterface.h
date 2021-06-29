#pragma once

namespace {
template<typename T>
struct AccessorInterface
{
    virtual T getValue() const = 0;
    virtual void setValue(T value) = 0;
};

//! A special type of AccessorInterface that allows for simple variable read/write operations
//! Can be used to read and write global settings for example.
template<typename T>
struct RefAccessor : public virtual AccessorInterface<T>
{
    virtual T& getRef() const = 0;

    T getValue() const override { return getRef(); };
    void setValue(T value) override { getRef() = value; };
};
}
