#include "vote.h"
#include "db_impl.h"

bool DbVote::createNew() {
    size_t count = db->getConnection()->count("test.images", mongo::BSONObj());
    if (count == 0) {
        return false;
    }

    mongo::BSONObj fieldsToReturn = BSON("_id" << 1);
    mongo::BSONObj left, right;

    size_t tries = 3;
    int randomNumber;
    while (tries > 0) {
        randomNumber = rand() % count;
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
        randomNumber = rand() % count;
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
        mongo::BSONObjBuilder().genOID()\
        .appendDBRef("leftId", "test.images", left["_id"].OID())\
        .appendDBRef("rightId", "test.images", right["_id"].OID())\
        .obj();
    db->getConnection()->insert("test.votes", b);
    this->leftId = left["_id"].OID().toString();
    this->rightId = right["_id"].OID().toString();
    this->voteId = b["_id"].OID().toString();
    return true;
}


bool DbVote::getById(const char* id) {
    const mongo::Query& q = mongo::Query( BSON("_id" << mongo::OID(id) ) );
    const auto& fields = BSON("_id" << 1 << "leftId" << 1 << "rightId" << 1);
    const auto& b = db->getConnection()->findOne("test.votes", q, &fields);
    if (b.isEmpty()) {
        return false;
    }
    this->leftId = b["leftId"].dbrefOID().toString();
    this->rightId = b["rightId"].dbrefOID().toString();
    this->voteId = id;
    return true;
}

