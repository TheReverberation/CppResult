#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <cmath>
#include <exception>


struct NullOptional {
} nullopt;

template <typename T>
class Optional {
public:
    Optional() {
        ptr = nullptr;
    }

    Optional(T value):
        ptr(new T(std::move(value)))
    {
    }

    Optional(Optional&& other) {
        ptr = other.ptr;
        other.ptr = nullptr;
    }

    Optional(NullOptional _):
        ptr(nullptr)
    {
    }

    Optional const& operator=(Optional other) {
        *this = std::move(other);
    } 

    Optional const& operator=(Optional&& other) {
        delete ptr;
        ptr = other.ptr;
        other.ptr = nullptr;
        return *this;
    }

    Optional const& operator=(NullOptional _) {
        delete ptr;
        ptr = nullptr;
    }

    ~Optional() {
        delete ptr;
    }

    T* operator->() {
        return ptr;
    }

    T& operator*() {
        return *ptr;
    }

    bool has_value() const {
        return ptr != nullptr;
    }

private:
    T* ptr;
};

template <typename T>
Optional<T> make_optional(T value) {
    return Optional<T>(std::move(value));
}

template <typename ReturnType>
class ReturnWrapper {
    template <typename R, typename E> friend class Result;
public:
    ReturnWrapper(ReturnType returned): returned_(std::move(returned)) {
    }
    ReturnWrapper(ReturnWrapper const&) = default;
    ReturnWrapper(ReturnWrapper&&) = default;
    ~ReturnWrapper() = default;
private:
    ReturnType returned_;
};

template <typename T>
ReturnWrapper<T> Ok(T returned) {
    return ReturnWrapper<T>(std::move(returned));
}


template <typename ErrorType>
class ErrorWrapper {
    template <typename R, typename E> friend class Result; 
public:
    ErrorWrapper(ErrorType error): error_(std::move(error)) {}
    ErrorWrapper(ErrorWrapper const&) = default;
    ErrorWrapper(ErrorWrapper&&) = default;
    ~ErrorWrapper() = default;
private:
    ErrorType error_;
};

template <typename T>
ErrorWrapper<T> Error(T error) {
    return ErrorWrapper<T>(std::move(error));
}


template <typename ReturnType, typename ErrorType>
class Result {
public:
    Result(Result const& other) = default;
    Result(Result&& other) = default;
    Result(ReturnWrapper<ReturnType> returned):
        Result(make_optional(std::move(returned.returned_)), nullopt)
    {}
    Result(ErrorWrapper<ErrorType> error):
        Result(nullopt, make_optional(std::move(error.error_))) {
    }

    bool is_ok() {
        return returned_.has_value();
    }

    bool is_error() {
        return error_.has_value();
    }

    ReturnType unwrap() {
        if (is_ok()) {
            return *returned_;
        } else {
            throw std::string("hui");
        }
    }

    ErrorType error() {
        if (is_error()) {
            return *error_;
        } else {
            return ErrorType();
        }
    }

private:
    
    Result(Optional<ReturnType> returned, Optional<ErrorType> error): 
        returned_(std::move(returned)), 
        error_(std::move(error)) {
    }

    Optional<ReturnType> returned_;
    Optional<ErrorType> error_;
};

Result<double, int> safe_sqrt(double x) {
    if (x < 0) {
        return Error(-1);
    } else {
        return Ok(sqrt(x));
    }
}

int main() {
    int n;
    std::cin >> n;
    auto s = safe_sqrt(n);
    if (s.is_ok()) {
        std::cout << s.unwrap() << '\n';
    } else {
        std::cout << "safe_sqrt error: " << s.error() << '\n';
    }
    return 0;
}
