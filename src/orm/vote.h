#pragma once
#include "db.h"

#include <string>

enum class EVoteResult {
    Unknown,
    Left,
    Right,
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

    const std::string& getId() const {
        return voteId;
    }

    EVoteResult getResult() const {
        return result;
    }

    bool createNew();
    bool getById(const std::string& id);
    bool putResult(EVoteResult result);

private:
    std::string leftId, rightId;
    std::string voteId;
    EVoteResult result;

    const Db& db;
};
