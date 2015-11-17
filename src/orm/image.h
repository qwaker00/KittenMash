#pragma once

#include "db.h"
#include <stddef.h>
#include <string>

class DbImage {
public:
    DbImage(Db& db);
    ~DbImage();

    bool InitById(const char* id);
    bool InitByData(const std::string& data);

    size_t getSize() const;
    const char* getData() const;
    const char* getData(int& len) const;
    std::string getId() const;

private:
    class Impl;
    Impl* impl;

    const Db& db;
};
