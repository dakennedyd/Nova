#pragma once

namespace Nova
{
template <typename T> class ISingleton
{
  public:
    static T &getInstance()
    {
        static T mInstance;
        return mInstance;
    }
    ISingleton() = default;
    ~ISingleton() = default;
    ISingleton(T const &) = delete;   // Copy ctor
    ISingleton(T &&) = delete;        // Move ctor
    T &operator=(T const &) = delete; // Copy assign
    T &operator=(T &&) = delete;      // Move assign
};
} // namespace Nova