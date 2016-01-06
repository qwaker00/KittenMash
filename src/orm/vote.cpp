#include "vote.h"
#include "db_impl.h"

#include <random>

namespace {
    static thread_local std::random_device rd;
    static thread_local std::mt19937 getRand(rd());
}

bool DbVote::createNew() {
    return createNew(mongo::OID::gen().toString());
}

bool DbVote::createNew(const std::string& id) {
    size_t count = db->getConnection()->count("test.images", mongo::BSONObj());
    if (count == 0) {
        return false;
    }

    mongo::BSONObj fieldsToReturn = BSON("_id" << 1);
    mongo::BSONObj left, right;

    size_t tries = 3;
    int randomNumber;
    while (tries > 0) {
        randomNumber = getRand() % count;
        const auto& cursor = db->getConnection()->query("test.images", mongo::Query(), 1, randomNumber, &fieldsToReturn);
        if (cursor->more()) {
            left = cursor->next().getOwned();
            break;
        }
        --tries;
    }
    if (tries == 0) {
        return false;
    }

    tries = 3;
    while (tries > 0) {
        randomNumber = getRand() % count;
        const auto& cursor = db->getConnection()->query("test.images", mongo::Query(), 1, randomNumber, &fieldsToReturn);
        if (cursor->more()) {
            right = cursor->next();
            if (right["_id"].OID() != left["_id"].OID()) {
                right = right.getOwned();
                break;
            }
        }
        --tries;
    }
    if (tries == 0) {
        return false;
    }

    const mongo::BSONObj& b =
        mongo::BSONObjBuilder()
        .append("_id", mongo::OID(id))
        .append("leftId", left["_id"].OID().toString())\
        .append("rightId", right["_id"].OID().toString())\
        .obj();

    db->getConnection()->insert("test.votes", b);
    mongo::BSONObj error = db->getConnection()->getLastErrorDetailed();

    if (!error.getField("err").isNull()) {
        return false;
    }

    this->leftId = left["_id"].OID().toString();
    this->rightId = right["_id"].OID().toString();
    this->voteId = b["_id"].OID().toString();
    this->result = EVoteResult::Unknown;

    return true;
}


bool DbVote::getById(const std::string& id) {
    if (id.length() != 24) {
        return false;
    }
    const mongo::Query& q = mongo::Query( BSON("_id" << mongo::OID(id) ) );
    const auto& fields = BSON("_id" << 1 << "leftId" << 1 << "rightId" << 1 << "result" << 1);
    const auto& b = db->getConnection()->findOne("test.votes", q, &fields);
    if (b.isEmpty()) {
        return false;
    }
    this->leftId = b["leftId"].String();
    this->rightId = b["rightId"].String();
    this->voteId = id;
    this->result = b.hasField("result") ? static_cast<EVoteResult>(b.getIntField("result")) : EVoteResult::Unknown;
    return true;
}

std::vector<DbVote> DbVote::getPending(Db& db, size_t n) {
    const mongo::Query& q = mongo::Query( BSON("pending" << BSON("$exists" << true)) );
    const auto& fields = BSON("_id" << 1 << "leftId" << 1 << "rightId" << 1 << "result" << 1);

    std::vector<DbVote> result;
    std::vector<mongo::BSONObj> out;
    db->getConnection()->findN(out, "test.votes", q, n, 0, &fields);
    for (const auto& b : out) {
        result.emplace_back(DbVote(db));
        result.back().voteId = b["_id"].OID().toString();
        result.back().leftId = b["leftId"].String();
        result.back().rightId = b["rightId"].String();
        result.back().result = b.hasField("result") ? static_cast<EVoteResult>(b.getIntField("result")) : EVoteResult::Unknown;
    }
    return result;
}

bool DbVote::putResult(EVoteResult result) {
    const mongo::Query& q = mongo::Query( BSON(
                "_id" << mongo::OID(voteId.c_str())
                << "result" << BSON("$exists" << false)
        ));
    const mongo::BSONObj set = BSON("$set" << BSON("result" << int(result) << "pending" << 1));
    db->getConnection()->update("test.votes", q, set, false, false);
    mongo::BSONObj error = db->getConnection()->getLastErrorDetailed();
    return error.getIntField("n") == 1;
}

bool DbVote::resetPending() const {
    const mongo::Query& q = mongo::Query( BSON(
            "_id" << mongo::OID(voteId.c_str())
            << "pending" << BSON("$exists" << true)
    ));
    const mongo::BSONObj set = BSON("$unset" << BSON("pending" << 1));
    db->getConnection()->update("test.votes", q, set, false, false);
    mongo::BSONObj error = db->getConnection()->getLastErrorDetailed();
    return error.getIntField("n") == 1;
}

