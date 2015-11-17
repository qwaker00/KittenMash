#pragma once

#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>
#include <fastcgi2/stream.h>

class TopHandler: virtual public fastcgi::Component, virtual public fastcgi::Handler {
public:
    TopHandler(fastcgi::ComponentContext *context)
        : fastcgi::Component(context)
    {}

    virtual ~TopHandler() {}

public:
    virtual void onLoad() {}

    virtual void onUnload() {}

    virtual void handleRequest(fastcgi::Request *req, fastcgi::HandlerContext*);
};

