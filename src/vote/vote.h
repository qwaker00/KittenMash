#pragma once

#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>
#include <fastcgi2/stream.h>

class VoteHandler: virtual public fastcgi::Component, virtual public fastcgi::Handler {
public:
    VoteHandler(fastcgi::ComponentContext *context)
        : fastcgi::Component(context)
    {}

    virtual ~VoteHandler() {}

public:
    virtual void onLoad() {}

    virtual void onUnload() {}

    virtual void handleRequest(fastcgi::Request *req, fastcgi::HandlerContext*);
};

