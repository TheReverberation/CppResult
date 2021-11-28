#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <cmath>
#include <exception>
#include <optional>
#include <type_traits>

template<typename Т>
class TD;

template <typename ReturnType>
class ReturnWrapper {
    template <typename R, typename E> friend class Result;
    
public:
    ReturnWrapper(ReturnType&& returned): m_returned(std::forward<ReturnType>(returned)) {}

    using my_type = const std::remove_reference_t<ReturnType>&;
    //ReturnWrapper(my_type returned): m_returned(returned) {}
    TD<ReturnType> c;
    TD<ReturnType&&> b;
    TD<my_type> a;
    ReturnWrapper(ReturnWrapper const&) = default;
    ReturnWrapper(ReturnWrapper&&) = default;
    ~ReturnWrapper() = default;
private:
    ReturnType m_returned;
};

template <typename T>
ReturnWrapper<T> Ok(T&& returned) {
//    TD<T> a;
    return ReturnWrapper<T>(std::forward<T>(returned));
}


template <typename ErrorType>
class ErrorWrapper {
    template <typename R, typename E> friend class Result; 
public:
    ErrorWrapper(ErrorType&& error): m_error(std::move(error)) {}
    ErrorWrapper(ErrorType const& error): m_error(error) {}

    ErrorWrapper(ErrorWrapper const&) = default;
    ErrorWrapper(ErrorWrapper&&) = default;
    ~ErrorWrapper() = default;
private:
    ErrorType m_error;
};

template <typename T>
ErrorWrapper<T> Error(T&& error) {
    return ErrorWrapper<T>(std::forward<T>(error));
}

/*
template <typename ReturnType, typename ErrorType>
class Result {
public:
    Result(Result const& other) = default;
    Result(Result&& other) = default;
    ~Result() = default;

    Result(ReturnWrapper<ReturnType>&& returned):
        m_returned(std::move(returned.m_returned)), 
        m_error(std::nullopt)
    {}

    Result(ReturnWrapper<ReturnType> const& returned):
        m_returned(returned.m_returned), 
        m_error(std::nullopt)
    {}

    Result(ErrorWrapper<ErrorType>&& error):
        m_returned(std::nullopt), 
        m_error(std::move(error.m_error)) 
    {}

    Result(ErrorWrapper<ErrorType> const& error):
        m_returned(std::nullopt), 
        m_error(error.m_error)
    {}

    bool is_ok() const {
        return m_returned.has_value();
    }

    bool is_error() const {
        return m_error.has_value();
    }

    ReturnType unwrap()  {
        if (is_ok()) {
            return *m_returned;
        } else {
#ifdef __cpp_exceptions
            throw std::string("hui");
#else
            std::abort();
#endif
        }
    }
    
    ReturnType unwrap()   {
        if (is_ok()) {
            return *std::move(m_returned);
        } else {
#ifdef __cpp_exceptions
            throw std::string("hui");
#else
            std::abort();
#endif
        }
    }
    
    ErrorType error()  {
        if (is_error()) {
            return *m_error;
        } else {
            return ErrorType();
        }
    }

private:
    std::optional<ReturnType> m_returned;
    std::optional<ErrorType> m_error;
};

Result<double, int> safe_sqrt(double x) {
    if (x < 0) {
        return Error(-1);
    } else {
        return Ok(getG());
        //return Ok(sqrt(x));
    }
}
*/

int g;
const int getG() {
    return 1;
}


int main() {
   /* int n;
    std::cin >> n;
    auto s = safe_sqrt(n);
    if (s.is_ok()) {
        std::cout << s.unwrap() << '\n';
    } else {
        std::cout << "safe_sqrt error: " << s.error() << '\n';
    }*/
    int a = 5;
//   ReturnWrapper<int> w0(a);
//    ReturnWrapper<int&> w(a);
   // ReturnWrapper<int&&> w2(getG());
// ReturnWrapper<const int&> w3(a);
//    Ok(getG());
//
    int const& b = getG();
    int const&& c = getG();
    return 0;
}
