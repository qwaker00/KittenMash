#include "image.h"
#include "db.h"
#include "db_impl.h"

#include <mongo/client/dbclient.h>

size_t DbImage::getSize() const {
    return data.length();
}

const char* DbImage::getData(size_t& len) const {
    len = data.size();
    return data.c_str();
}

const std::string& DbImage::getId() const {
    return id;
}

void DbImage::putWithData(const std::string& data) {
    const mongo::BSONObj& b = mongo::BSONObjBuilder().genOID()\
        .append("gameCount", (long long)0) \
        .append("rating", (double)1200) \
        .obj();
    db->getConnection()->insert("test.images", b);

    mongo::GridFS gfs(*db->getConnection(), "test");
    gfs.storeFile(data.c_str(), data.length(), b["_id"].OID().toString());

    this->data = data;
    this->id = b["_id"].OID().toString();
}

bool DbImage::getById(const std::string& id) {
    if (id.length() != 24) {
        return false;
    }
    const mongo::Query& q = mongo::Query( BSON("_id" << mongo::OID(id) ) );
    const auto& fields = BSON("_id" << 1 << "data" << 1);
    const auto& b = db->getConnection()->findOne("test.images", q, &fields);
    if (b.isEmpty()) {
        return false;
    }
    int len;
    const char * ptr = b["data"].binData(len);
    this->data = std::string(ptr, len);
    this->id = id;
    return true;
}

