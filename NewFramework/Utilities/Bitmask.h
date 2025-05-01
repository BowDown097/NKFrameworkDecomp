#pragma once

template<typename U, typename T>
class Bitmask {
public:
    Bitmask() = default;
    Bitmask(T value) : m_value(value) {}
    inline bool operator==(T other) const {
        return m_value == other;
    }
    inline bool operator==(U underlying) const {
        return m_value == static_cast<T>(underlying);
    }
    inline bool operator!=(T other) const {
        return m_value != other;
    }
    inline bool operator!=(U underlying) const {
        return m_value != static_cast<T>(underlying);
    }
    inline bool operator<(T value) const {
        return static_cast<U>(m_value) < static_cast<U>(value);
    }
    inline bool operator<(U underlying) const {
        return static_cast<U>(m_value) < underlying;
    }
    inline bool operator<=(T value) const {
        return static_cast<U>(m_value) <= static_cast<U>(value);
    }
    inline bool operator<=(U underlying) const {
        return static_cast<U>(m_value) <= underlying;
    }
    inline bool operator>(T value) const {
        return static_cast<U>(m_value) > static_cast<U>(value);
    }
    inline bool operator>(U underlying) const {
        return static_cast<U>(m_value) > underlying;
    }
    inline bool operator>=(T value) const {
        return static_cast<U>(m_value) >= static_cast<U>(value);
    }
    inline bool operator>=(U underlying) const {
        return static_cast<U>(m_value) >= underlying;
    }
    inline Bitmask<U, T> operator|(T other) const {
        return static_cast<T>(static_cast<U>(m_value) | static_cast<U>(other));
    }
    inline Bitmask<U, T> operator&(T other) const {
        return static_cast<T>(static_cast<U>(m_value) & static_cast<U>(other));
    }
    inline Bitmask<U, T>& operator|=(T other) {
        return *this = *this | other;
    }
    inline Bitmask<U, T>& operator&=(T other) {
        return *this = *this & other;
    }
private:
    T m_value{};
};
