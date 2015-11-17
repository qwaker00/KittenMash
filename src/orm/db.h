#pragma once

#include <sstream>
#include <iostream>

class DbImpl;

class Db {
public:
    Db();
    ~Db();

    DbImpl* operator->() const {
        return impl;
    }

private:
    DbImpl* impl;
};


class DbException : public std::exception {
protected:
    std::string message;

public:
    virtual const char* what() const throw () {
        return message.c_str();
    }

    template<typename T>
    DbException& operator<<(const T& value) {
        std::ostringstream messageStream;
        messageStream << value;
        message += messageStream.str();
        return *this;
    }
};

