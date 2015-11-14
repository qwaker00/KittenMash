#pragma once

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

