#pragma once

#include "db.h"
#include <stddef.h>
#include <string>

class DbImage {
public:
    DbImage(Db& db)
        : db(db)
    {}

    bool getById(const std::string& id);
    void putWithData(const std::string& data);

    size_t getSize() const;
    const char* getData(size_t& len) const;
    const std::string& getId() const;

private:
    std::string data;
    std::string id;

    const Db& db;
};
