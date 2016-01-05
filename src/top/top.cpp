#include "top.h"

#include <orm/rating.h>

#include <iostream>

namespace {
    thread_local Db db;
}

void TopHandler::handleRequest(fastcgi::Request *req, fastcgi::HandlerContext*) {
    if (req->getRequestMethod() == "GET") {
        const auto& filename = req->getScriptFilename();
        int topCount = 100;
        if (filename.length() > 5) {
            if (filename.length() > 10 || filename.substr(0, 5) != "/top/") {
                return req->sendError(400); // Bad request;
            }
            try {
                topCount = std::stoi(filename.substr(5));
            } catch(std::exception& e) {
                return req->sendError(400); // Bad number
            }
            if (topCount > 100 || topCount < 0) {
                return req->sendError(400); // Too many or too few
            }
        }
        const auto& ratings = DbRating::getTop(db, topCount);
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

