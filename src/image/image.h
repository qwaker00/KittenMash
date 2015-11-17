#pragma once

#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>
#include <fastcgi2/stream.h>

class ImageHandler: virtual public fastcgi::Component, virtual public fastcgi::Handler {
public:
    ImageHandler(fastcgi::ComponentContext *context)
        : fastcgi::Component(context)
    {}

    virtual ~ImageHandler() {}

public:
    virtual void onLoad() {}
    virtual void onUnload() {}
    virtual void handleRequest(fastcgi::Request *req, fastcgi::HandlerContext*);
};

