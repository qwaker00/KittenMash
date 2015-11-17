#include "image.h"

#include <orm/image.h>

#include <iostream>

void ImageHandler::handleRequest(fastcgi::Request *req, fastcgi::HandlerContext*) {
    if (req->getRequestMethod() == "GET") {
        const auto& filename = req->getScriptFilename();
        if (filename.length() <= 7) {
            return req->sendError(400); // No id provided
        }
        const auto& imageId = req->getScriptFilename().substr(7);
        Db db;
        DbImage image(db);
        if (!image.GetById(imageId.c_str())) {
            return req->sendError(404); // Not found
        }
        size_t len = 0;
        const char* buf = image.getData(len);
        req->write(buf, len);
        return;
    } else
    if (req->getRequestMethod() == "POST") {
        if (req->hasFile("image")) {
           if (req->remoteFile("image").size() > 1 * 1024 * 1024) {
                return req->sendError(413); // Too large
            }
            std::string data;
            req->remoteFile("image").toString(data);
            Db db;
            DbImage image(db);
            image.PutWithData(data);
            req->setHeader("Location", std::string("/image/") + image.getId());
            return req->setStatus(302); // Redirect
        } else {
            return req->sendError(400); // Filename should be image
        }
    } else {
        return req->sendError(405); // Invalid method
    }
}

