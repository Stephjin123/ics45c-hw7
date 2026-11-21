#ifndef INT_ARRAY_HPP
#define INT_ARRAY_HPP

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

class Array {
public:
    Array() : len{0}, buf{nullptr} {
    }

    explicit Array(int len) : len{len}, buf{new int[len]} {
    }

    // Copy constructor: deep copy, allocate a new buffer.
    Array(const Array& other) : len{other.len}, buf{new int[other.len]} {
        for (int i = 0; i < len; ++i) {
            buf[i] = other.buf[i];
        }
    }

    // Move constructor: steal the buffer, allocate nothing.
    Array(Array&& other) noexcept : len{other.len}, buf{other.buf} {
        other.len = 0;
        other.buf = nullptr;
    }

    friend void swap(Array& lhs, Array& rhs) noexcept {
        std::swap(lhs.len, rhs.len);
        std::swap(lhs.buf, rhs.buf);
    }

    // Copy assignment: copy-and-swap; the old buffer is freed when tmp dies.
    Array& operator=(const Array& other) {
        Array tmp{other};
        swap(*this, tmp);
        return *this;
    }

    // Move assignment: move into tmp, swap; old buffer freed when tmp dies.
    Array& operator=(Array&& other) noexcept {
        Array tmp{std::move(other)};
        swap(*this, tmp);
        return *this;
    }

    ~Array() {
        delete[] buf;
    }

    int length() const {
        return len;
    }

    int& operator[](int index) {
        if (!in_bounds(index)) {
            throw "Exception operator[](" + std::to_string(index) + ") Out Of Range";
        }
        return buf[index];
    }

    const int& operator[](int index) const {
        if (!in_bounds(index)) {
            throw "Exception operator[](" + std::to_string(index) + ") Out Of Range";
        }
        return buf[index];
    }

    void fill(int val) {
        for (int i = 0; i < len; ++i) {
            buf[i] = val;
        }
    }

private:
    int len;
    int* buf;

    bool in_bounds(int index) const {
        return index >= 0 && index < len;
    }
};

inline std::ostream& operator<<(std::ostream& out, const Array& array) {
    std::stringstream temp;
    // We don't want to set formatting flags globally.
    temp << std::setprecision(2) << std::fixed << std::right;

    for (int i = 0; i < array.length(); ++i) {
        temp << std::setw(8) << array[i];
    }

    out << temp.str();
    return out;
}

inline std::istream& operator>>(std::istream& in, Array& array) {
    for (int i = 0; i < array.length(); ++i) {
        in >> array[i];
    }

    return in;
}

#endif
