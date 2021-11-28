#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <cmath>
#include <exception>
#include <optional>
#include <type_traits>

template <typename Т>
class TD;



template <typename ReturnType>
class ReturnWrapper {
    template <typename R, typename E> friend class Result;
    using ValueType = std::conditional_t<std::is_rvalue_reference_v<ReturnType>, std::remove_reference_t<ReturnType>, ReturnType>; 
public:
    
    template <typename T>
    ReturnWrapper(T&& returned): m_returned(std::forward<T>(returned))
    {
//        TD<ValueType> c;
  //      TD<decltype(returned)> b;
    //    TD<decltype(std::forward<T>(returned))> a;
    }

    ReturnWrapper(ReturnWrapper const&) = default;
    ReturnWrapper(ReturnWrapper&&) = default;
    ~ReturnWrapper() = default;
private:
    ValueType m_returned;
};

template <typename T>
ReturnWrapper<T> Ok(T&& returned) {
//    TD<T> a;
    return ReturnWrapper<T>(std::forward<T>(returned));
}


template <typename ErrorType>
class ErrorWrapper {
    template <typename R, typename E> friend class Result; 
    using ValueType = std::conditional_t<std::is_rvalue_reference_v<ErrorType>, std::remove_reference_t<ErrorType>, ErrorType>; 
public:

    template <typename T>
    ErrorWrapper(T&& error): m_error(std::forward<T>(error)) {}

    ErrorWrapper(ErrorWrapper const&) = default;
    ErrorWrapper(ErrorWrapper&&) = default;
    ~ErrorWrapper() = default;
private:
    ValueType m_error;
};

template <typename T>
ErrorWrapper<T> Error(T&& error) {
    return ErrorWrapper<T>(std::forward<T>(error));
}


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
        double s = sqrt(x);
        return Ok(s);
    }
}




struct A {
    std::string m_name;
    A(std::string name) {
        m_name = name; 
        std::cout << "default constructor: " << m_name << '\n';
    }
    A(A const& other) {
        std::cout << "copy constructor\n";
    }
    A(A&& other) {
        m_name = other.m_name;
        other.m_name = m_name + " old";
        std::cout << "move constructor: " << m_name << '\n';
    }
    ~A() {
        std::cout << "destructor: " << m_name << '\n';
    }
};


int main() {
   int n;
    std::cin >> n;
    auto s = safe_sqrt(n);
    if (s.is_ok()) {
        std::cout << s.unwrap() << '\n';
    } else {
        std::cout << "safe_sqrt error: " << s.error() << '\n';
    }
//    ReturnWrapper<int&&> a(5);
    return 0;
}
