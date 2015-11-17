#include "rating.h"
#include "db_impl.h"

bool DbRating::InitById(const char* id) {
    const mongo::Query& q = mongo::Query( BSON("_id" << mongo::OID(id) ) );
    const auto& fields = BSON("_id" << 1 << "rating" << 1 << "gameCount" << 1);
    const auto& b = db->getConnection()->findOne("test.images", q, &fields);
    if (b.isEmpty()) {
        return false;
    }
    this->rating = b["rating"].Double();
    this->gameCount = b["gameCount"].numberLong();
    this->id = b["_id"].OID().toString();
    return true;
}

std::vector<DbRating> DbRating::getTop(Db& db, size_t n) {
    std::vector<DbRating> result;
    const auto& fields = BSON("_id" << 1 << "rating" << 1 << "gameCount" << 1);
    std::vector<mongo::BSONObj> out;
    db->getConnection()->findN(out, "test.images", mongo::Query().sort("rating", -1), n, 0, &fields);
    for (const auto& b : out) {
        result.emplace_back(db);
        result.back().rating = b["rating"].Double();
        result.back().gameCount = b["gameCount"].numberLong();
        result.back().id = b["_id"].OID().toString();
    }
    return result;
}

