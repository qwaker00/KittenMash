#pragma once
#include "db.h"

#include <string>
#include <vector>

enum class EVoteResult : int {
    Unknown = 0,
    Left = 1,
    Right = 2,
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
    bool createNew(const std::string& id);

    bool getById(const std::string& id);
    bool putResult(EVoteResult result);

    bool resetPending() const;

    static std::vector<DbVote> getPending(Db& db, size_t n);

private:
    std::string leftId, rightId;
    std::string voteId;
    EVoteResult result;

    const Db& db;
};
