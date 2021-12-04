#include <variant>
#include <type_traits>

template <typename ReturnType>
class ReturnWrapper {
    template <typename R, typename E> friend class Result;
    template <typename T> friend ReturnWrapper<T> Ok(T&&);

private:
    template <typename T>
    ReturnWrapper(T&& returned): m_returned(std::forward<T>(returned)) {}

public:
    ReturnWrapper(ReturnWrapper const&) = default;
    ReturnWrapper(ReturnWrapper&&) = default;
    ~ReturnWrapper() = default;

private:
    ReturnType m_returned;
};

template <typename T>
ReturnWrapper<T> Ok(T&& returned) {
    return ReturnWrapper<T>(std::forward<T>(returned));
}


template <typename ErrorType>
class ErrorWrapper {
    template <typename R, typename E> friend class Result; 
    template <typename T> friend ErrorWrapper<T> Error(T&&);

private:
    template <typename T>
    ErrorWrapper(T&& error): m_error(std::forward<T>(error)) {}

public:
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


template <typename ReturnType, typename ErrorType>
class Result {
public:
    Result(Result const& other) = default;
    Result(Result&& other) = default;
    ~Result() = default;

    
    template <typename T>
    Result(ReturnWrapper<T>&& returned):
        m_value(static_cast<std::remove_reference_t<ReturnType>>(std::move(returned.m_returned)))
    {
    }

    template <typename T>
    Result(ErrorWrapper<T>&& error):
        m_value(static_cast<std::remove_reference_t<ErrorType>>(std::move(error.m_error)))
    {}

    bool is_ok() const {
        return m_value.index() == 0;
    }

    bool is_error() const {
        return !is_ok();
    }

    ReturnType& unwrap() & {
        if (is_ok()) {
            return std::get<0>(m_value);
        } else {
            std::abort(); 
        }
    }
    
    ReturnType const& unwrap() const& {
        if (is_ok()) {
            return std::get<0>(m_value);
        } else {
            std::abort();
        }
    }

    ReturnType&& unwrap() && {
        if (is_ok()) {
            return std::get<0>(std::move(m_value));
        } else {
            std::abort();
        }
    }

    ErrorType& error() & {
        if (is_error()) {
            return std::get<1>(m_value);
        } else {
            std::abort();
        }
    }

    ErrorType const& error() const& {
        if (is_error()) {
            return std::get<1>(m_value);
        } else {
            std::abort();
        }
    }
    
    ErrorType&& error() && {
        if (is_error()) {
            return std::get<1>(std::move(m_value));
        } else {
            std::abort();
        }
    }

private:
    std::variant<
        std::conditional_t<std::is_reference_v<ReturnType>, 
        std::reference_wrapper<std::remove_reference_t<ReturnType>>, 
        ReturnType>,
        std::conditional_t<std::is_reference_v<ErrorType>, 
        std::reference_wrapper<std::remove_reference_t<ErrorType>>, 
        ErrorType>
    > m_value;
};

