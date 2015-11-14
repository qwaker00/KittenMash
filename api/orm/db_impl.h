#pragma once

#include <mongo/client/dbclient.h>
#include <memory>

class DbImpl {
public:
    DbImpl();

    mongo::DBClientConnection* getConnection() {
        return connection.get();
    }

private:
    std::unique_ptr<mongo::DBClientConnection> connection;
};

