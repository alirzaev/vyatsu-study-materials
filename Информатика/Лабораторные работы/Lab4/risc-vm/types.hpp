#pragma once
#include "includes.hpp"

namespace risc {
    enum class val_types { ABS, REL, REG, LBL, VAL, NONE };
    using size_type = std::vector<uint8_t>::size_type;
    struct cell {
        size_type index;
        val_types type;
    };
    struct command {
        command() = default;
        command(const std::string &n, std::array<cell, 3> &a) : name(n), addrs(a) { }
        std::string name;
        std::array<cell, 3> addrs;
    };
    using token_str = std::vector<std::string>;
    using operation = size_type (*)(std::array<cell, 3> &, size_type);

    class parse_exception : std::exception {
    public:
        parse_exception() : m_msg()
        { }
        parse_exception(const std::string& str) : m_msg(str.c_str())
        { }
        parse_exception(const parse_exception &other) : m_msg(other.m_msg)
        { }
        parse_exception &operator=(const parse_exception &other)
        {
            if (m_msg != other.m_msg) {
                delete[] m_msg;
                m_msg = other.m_msg;
            }
            return *this;
        }
        virtual ~parse_exception()
        {
            delete[] m_msg;
        }
        const char* what()
        {
            return m_msg;
        }
    private:
        const char* m_msg;
    };
}
