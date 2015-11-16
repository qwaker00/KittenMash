#pragma once

#include "db.h"
#include <stddef.h>

class DbImage {
public:
    DbImage(Db* db, const char* id);
    ~DbImage();

    size_t getSize() const;
    const char* getData() const;

private:
    class Impl;
    Impl* impl;
};
