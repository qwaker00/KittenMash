#pragma once
#include "db.h"

#include <string>
#include <vector>

class DbRating {
public:
    DbRating(Db& db)
        : db(db)
    {}

    double getRating() const {
        return rating;
    }

    size_t getGameCount() const {
        return gameCount;
    }

    const std::string& getId() const {
        return id;
    }

    bool getById(const std::string& id);
    bool addToRating(double value);

    static std::vector<DbRating> getTop(Db& db, size_t n);


private:
    double rating;
    long long gameCount;
    std::string id;

    const Db& db;
};
