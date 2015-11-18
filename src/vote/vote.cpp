#include "vote.h"

#include <orm/vote.h>

#include <iostream>

void VoteHandler::handleRequest(fastcgi::Request *req, fastcgi::HandlerContext*) {
    if (req->getRequestMethod() == "POST") {
        const auto& filename = req->getScriptFilename();
        if (filename != "/vote/") {
            return req->sendError(400);
        }
        Db db;
        DbVote vote(db);
        if (!vote.createNew()) {
            return req->sendError(500); // Ooooops
        }
        req->setHeader("Location", std::string("/vote/") + vote.getId());
        return req->setStatus(302); // Redirect
    } else
    if (req->getRequestMethod() == "GET") {
        const auto& filename = req->getScriptFilename();
        if (filename.length() <= 6) {
            return req->sendError(400); // No id provided
        }
        const auto& voteId = req->getScriptFilename().substr(6);
        Db db;
        DbVote vote(db);
        if (!vote.getById(voteId.c_str())) {
            return req->sendError(404); // Not found
        }

        std::ostringstream output;
        output << '{';
        output << "\"leftImage\":\"/image/" << vote.getLeftId() << "\",";
        output << "\"rightImage\":\"/image/" << vote.getRightId() << "\",";
        output << "\"voteId\":\"" << vote.getId() << "\"";
        output << '}';

        req->setContentType("application/json");
        const auto& str = output.str();
        req->write(str.c_str(), str.length());
        return;
    } else {
        return req->sendError(405); // Invalid method
    }
}

