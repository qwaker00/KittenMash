#include "vote.h"

#include <orm/vote.h>
#include <orm/rating.h>

#include <iostream>
#include <cmath>

namespace {
    double calcEloAdd(double rA, double , double rB, double score) {
        const double K = rA < 2100 ? 32 : (rA < 2400 ? 24 : 16);
        const double eA = 1.0 / (1.0 + pow(10.0, (rB - rA) / 400));
        return K * (score - eA);
    }

    thread_local Db db;
}

void VoteHandler::handleRequest(fastcgi::Request *req, fastcgi::HandlerContext*) {
    if (req->getRequestMethod() == "POST") {
        const auto& filename = req->getScriptFilename();
        if (filename != "/vote/") {
            return req->sendError(400);
        }
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

        bool needResult = false;
        std::string voteId = filename.substr(6);
        if (filename.length() > 7 && filename.substr(filename.length() - 7) == "/result") {
            voteId = filename.substr(6, filename.length() - 6 - 7);
            needResult = true;
        } else {
            voteId = filename.substr(6);
        }

        DbVote vote(db);
        if (!vote.getById(voteId)) {
            return req->sendError(404); // Not found
        }

        std::ostringstream output;

        if (needResult) {
            if (vote.getResult() == EVoteResult::Left) {
                output << "left";
            } else
            if (vote.getResult() == EVoteResult::Right) {
                output << "right";
            } else {
                return req->sendError(404); // Result not found
            }
        } else {
            output << '{';
            output << "\"leftImage\":\"/image/" << vote.getLeftId() << "\",";
            output << "\"rightImage\":\"/image/" << vote.getRightId() << "\",";
            output << "\"voteId\":\"" << vote.getId() << "\"";
            output << '}';
            req->setContentType("application/json");
        }

        const auto& str = output.str();
        req->write(str.c_str(), str.length());
        return;
    } else
    if (req->getRequestMethod() == "PUT") {
        const auto& filename = req->getScriptFilename();
        if (filename.length() <= 13 || filename.substr(filename.length() - 7) != "/result") {
            return req->sendError(400); // No id provided or /result provided
        }
        const auto& voteId = filename.substr(6, filename.length() - 7 - 6);


        if (req->requestBody().size() > 10) {
            return req->sendError(400); // Too long body
        }
        std::string result;
        req->requestBody().toString(result);
        if (result != "left" && result != "right") {
            return req->sendError(400); // Bad body
        }
        EVoteResult resultValue = (result == "left") ? EVoteResult::Left : EVoteResult::Right;


        DbVote vote(db);
        if (!vote.getById(voteId)) {
            return req->sendError(404); // Not found
        }
        if (!vote.putResult(resultValue)) {
            return req->sendError(400); // already resulted
        }

        DbRating leftRating(db), rightRating(db);
        leftRating.getById(vote.getLeftId());
        rightRating.getById(vote.getRightId());
        leftRating.addToRating( calcEloAdd(leftRating.getRating(), leftRating.getGameCount(), rightRating.getRating(), resultValue == EVoteResult::Left) );
        rightRating.addToRating( calcEloAdd(rightRating.getRating(), rightRating.getGameCount(), leftRating.getRating(), resultValue == EVoteResult::Right) );

        return;
     } else {
        return req->sendError(405); // Invalid method
    }
}

