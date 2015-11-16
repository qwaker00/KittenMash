#include "image.h"
#include "db.h"
#include "db_impl.h"

#include <mongo/client/dbclient.h>

class DbImage::Impl {
public:
    Impl(mongo::BSONObj b)
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
    return impl->data["data"].binData(len);
}

DbImage::DbImage(Db* db, const char* id) {
    try {
        const mongo::Query& q = mongo::Query( BSON("_id" << mongo::OID(id) ) );
        const mongo::BSONObj& b = (*db)->getConnection()->findOne("test.images", q);
        if (b.isEmpty()) {
            throw std::exception();
        } else {
            impl = new Impl(b);
        }
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

DbImage::~DbImage() {
    delete impl;
}

