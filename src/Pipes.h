#pragma once

namespace Nova
{
template <class INPUT, class OUTPUT> struct Pipe
{
    typedef INPUT inputType;
    typedef OUTPUT outputType;

    INPUT mInputValue;
    Pipe(const inputType &inputValue = inputType()) : mInputValue(inputValue) {}
    virtual auto operator()() const -> outputType { return outputType(); }
};

template <class INPUT, class OUTPUT_INPUT, class OUTPUT>
auto operator>>(const Pipe<INPUT, OUTPUT_INPUT> &lhs, Pipe<OUTPUT_INPUT, OUTPUT> &rhs)
    -> Pipe<OUTPUT_INPUT, OUTPUT> &
{
    rhs = lhs();
    return rhs;
}

template <class INPUT, class OUTPUT_INPUT, class OUTPUT>
auto operator>>(const Pipe<INPUT, OUTPUT_INPUT> &lhs, Pipe<OUTPUT_INPUT, OUTPUT> &&rhs)
    -> Pipe<OUTPUT_INPUT, OUTPUT> &
{
    rhs = lhs();
    return rhs;
}

template <class INPUT, class OUTPUT>
auto operator>>(const Pipe<INPUT, OUTPUT> &lhs, OUTPUT &rhs) -> OUTPUT &
{
    rhs = lhs();
    return rhs;
}

template <class INPUT, class OUTPUT>
auto operator>>(const Pipe<INPUT, OUTPUT> &lhs, OUTPUT &&rhs) -> OUTPUT &
{
    rhs = lhs();
    return rhs;
}

/*==================================================*/

/*struct StringToInt : public Pipe <std::string, int>
{
        StringToInt(const std::string &s = "") : Pipe <inputType, outputType>(s)
        {
        }

        auto operator () () const -> outputType
        {
                return std::stoi(mInputValue);
        }
};

struct IntSquare : public Pipe <int, int>
{
        IntSquare(int n = 0) : Pipe <inputType, outputType>(n)
        {
        }

        auto operator () () const -> outputType
        {
                return mInputValue * mInputValue;
        }
};

struct DivideBy42F : public Pipe <int, float>
{
        DivideBy42F(int n = 0) : Pipe <inputType, outputType>(n)
        {
        }

        auto operator () () const -> outputType
        {
                return static_cast <float> (mInputValue) / 42.0f;
        }
};*/

struct SortRenderPackets : public Pipe<std::vector<int>, std::vector<int>>
{
    SortRenderPackets(const std::vector<int> &packets) : Pipe<inputType, outputType>(packets) {}
    auto operator()() const -> outputType
    {
        std::vector<int> a = std::move(mInputValue);
        std::sort(a.begin(), a.end());
        return a;
    }
};
/*
float out = 0;
StringToInt("42") >> IntSquare() >> IntSquare() >> DivideBy42F() >> out;
std::cout << out << "\n";
LOG_INFO(out);
*/
} // namespace Nova