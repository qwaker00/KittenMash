#include <orm/image.h>

#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>
#include <fastcgi2/stream.h>

#include <iostream>

class ImageHandler: virtual public fastcgi::Component, virtual public fastcgi::Handler {
public:
    ImageHandler(fastcgi::ComponentContext *context)
        : fastcgi::Component(context)
    {}

    virtual ~ImageHandler() {}

public:
    virtual void onLoad() {}

    virtual void onUnload() {}

    virtual void handleRequest(fastcgi::Request *req, fastcgi::HandlerContext*) {
        if (req->getRequestMethod() == "GET") {
            const auto& filename = req->getScriptFilename();
            if (filename.length() <= 7) {
                return req->sendError(400); // No id provided
            }
            const auto& imageId = req->getScriptFilename().substr(7);
            Db db;
            DbImage image(db);
            if (!image.InitById(imageId.c_str())) {
                return req->sendError(404); // Not found
            }
            int len = 0;
            auto buf = image.getData(len);
            req->write(buf, len);
            return;
        } else
        if (req->getRequestMethod() == "POST") {
            if (req->hasFile("image")) {
                Db db;
                DbImage image(db);
                if (req->remoteFile("image").size() > 1 * 1024 * 1024) {
                    return req->sendError(413); // Too large
                }
                std::string data;
                req->remoteFile("image").toString(data);
                image.InitByData(data);
                req->setHeader("Location", std::string("/image/") + image.getId());
                return req->setStatus(302); // Redirect
            } else {
                return req->sendError(400); // Filename should be image
            }
        } else {
            return req->sendError(405); // Invalid method
        }
    }

};

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
FCGIDAEMON_ADD_DEFAULT_FACTORY("image_factory", ImageHandler)
FCGIDAEMON_REGISTER_FACTORIES_END()
