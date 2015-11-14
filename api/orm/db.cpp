#include "db.h"
#include "db_impl.h"

Db::Db()
 : impl(new DbImpl())
{
}

Db::~Db() {
    delete impl;
}

