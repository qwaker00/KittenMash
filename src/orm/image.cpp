#include "image.h"
#include "db.h"
#include "db_impl.h"

#include <mongo/client/dbclient.h>

class DbImage::Impl {
public:
    Impl(const mongo::BSONObj& b)
        : data(b)
    {}

    mongo::BSONObj data;
};

size_t DbImage::getSize() const {
    int len;
    impl->data["data"].binData(len);
    return len;
}

const char* DbImage::getData() const {
    int len;
    return getData(len);
}

const char* DbImage::getData(int& len) const {
    return impl->data["data"].binData(len);
}

std::string DbImage::getId() const {
    return impl->data["_id"].OID().toString();
}

DbImage::DbImage(Db& db)
    : impl(nullptr)
    , db(db)
{}

bool DbImage::InitByData(const std::string& data) {
    const mongo::BSONObj& b = mongo::BSONObjBuilder().genOID()\
        .appendBinData("data", data.length(), mongo::BinDataType::BinDataGeneral, data.c_str())\
        .append("gameCount", (long long)0) \
        .append("rating", (double)1200) \
        .obj();
    db->getConnection()->insert("test.images", b);
    delete impl;
    impl = new Impl(b.getOwned());
    return true;
}

bool DbImage::InitById(const char* id) {
    const mongo::Query& q = mongo::Query( BSON("_id" << mongo::OID(id) ) );
    const auto& fields = BSON("_id" << 1 << "data" << 1);
    const auto& b = db->getConnection()->findOne("test.images", q, &fields);
    if (b.isEmpty()) {
        return false;
    }
    delete impl;
    impl = new Impl(b.getOwned());
    return true;
}

DbImage::~DbImage() {
    delete impl;
}

