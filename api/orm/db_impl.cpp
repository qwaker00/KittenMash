#include "db_impl.h"

namespace {
    std::unique_ptr<mongo::DBClientConnection> createConnection() {
        try {
            std::unique_ptr<mongo::DBClientConnection> c( new mongo::DBClientConnection() );
            c->connect("localhost");
            return c;
        } catch(std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
        return nullptr;
    }
}

DbImpl::DbImpl()
    : connection(createConnection())
{
}

