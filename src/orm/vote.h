#pragma once
#include "db.h"

#include <string>

enum class EVoteResult {
    Left,
    Right
};

class DbVote {
public:
    DbVote(Db& db)
        : db(db)
    {}

    const std::string& getLeftId() const {
        return leftId;
    }

    const std::string& getRightId() const {
        return rightId;
    }

    void createNew();
    void getById(const char* id);
    bool putResult(EVoteResult result);

private:
    std::string leftId, rightId;
    std::string voteId;

    const Db& db;
};
