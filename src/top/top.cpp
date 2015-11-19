#include "top.h"

#include <orm/rating.h>

#include <iostream>

namespace {
    thread_local Db db;
}

void TopHandler::handleRequest(fastcgi::Request *req, fastcgi::HandlerContext*) {
    if (req->getRequestMethod() == "GET") {
        const auto& ratings = DbRating::getTop(db, 100);
        std::ostringstream output;
        output << '[';
        bool first = true;
        for (const auto& rating : ratings) {
            if (!first) {
                output << ",";
            } else
                first = false;
            output << "{";
            output << "\"image\":\"/image/" << rating.getId() << "\",";
            output << "\"rating\":" << rating.getRating() << ",";
            output << "\"gameCount\":" << rating.getGameCount();
            output << "}";
        }
        output << ']';
        req->setContentType("application/json");

        const auto& str = output.str();
        req->write(str.c_str(), str.length());
        return;
    } else {
        return req->sendError(405); // Invalid method
    }
}

