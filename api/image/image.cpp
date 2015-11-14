#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>
#include <fastcgi2/stream.h>

#include <iostream>
#include "../orm/image.h"

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
            std::cerr << imageId << std::endl;
            Db db;
            std::cerr << DbImage(&db, imageId.c_str()).getSize() << std::endl;
        } else
        if (req->getRequestMethod() == "POST") {
            std::vector<std::string> fileNames;
            req->remoteFiles(fileNames);
            std::cerr << fileNames.size() << ":\n";
            for (const auto& f : fileNames) {
                std::cerr << f << std::endl;
            }
        } else {
            req->sendError(405);
        }
    }

};

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
FCGIDAEMON_ADD_DEFAULT_FACTORY("image_factory", ImageHandler)
FCGIDAEMON_REGISTER_FACTORIES_END()
