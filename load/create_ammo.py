#!/usr/bin/env python
from pymongo import MongoClient
import random
import sys

client = MongoClient()
db = client.test

output = sys.stdout.write

def make_ammo(method, url, headers, case, body):
    """ makes phantom ammo """
    #http request w/o entity body template
    req_template = (
          "%s %s HTTP/1.1\r\n"
          "%s\r\n"
          "\r\n"
    )

    #http request with entity body template
    req_template_w_entity_body = (
          "%s %s HTTP/1.1\r\n"
          "%s\r\n"
          "Content-Length: %d\r\n"
          "\r\n"
          "%s\r\n"
    )

    if not body:
        req = req_template % (method, url, headers)
    else:
        req = req_template_w_entity_body % (method, url, headers, len(body), body)

    #phantom ammo template
    ammo_template = (
        "%d %s\n"
        "%s"
    )

    return ammo_template % (len(req), case, req)

def make_all_ammo(ammo):
    for item in ammo:
        try:
            method, url, case, body = item
            body = body.strip()
        except:
            method, url, case = stdin_line.split("||")
            body = None

        method, url, case = method.strip(), url.strip(), case.strip()

        headers = "Host: hostname.com\r\n" + \
            "User-Agent: tank\r\n" + \
            "Accept: */*\r\n" + \
            "Connection: Close"

        output(make_ammo(method, url, headers, case, body))

random.seed(666)

db.votes.remove()
db.images.update_many({}, {'$set' : {'gameCount' : 0l, 'rating' : 1200.0}})
ammo_post, ammo_put, ammo_get = [], [], []
for item in xrange(80000):
    vote = '%024x' % random.randint(0, 2 ** (8 * 12) - 1)
    ammo_post.append( ('POST', '/vote/%s' % vote, 'create_vote', '') )
    ammo_get.append( ('GET', '/vote/%s' % vote, 'get_vote', '') )
    ammo_put.append( ('PUT', '/vote/%s/result' % vote, 'set_vote', 'left' if random.randint(0, 1) == 0 else 'right') )

ammo = []
deficit1 = 0
deficit2 = 0
deficit_thres = 2000
while len(ammo_post) + len(ammo_put) + len(ammo_get) > 0:
    if len(ammo_put) + len(ammo_get) > 0 and (
            len(ammo_post) == 0 or
            (deficit1 > deficit_thres and random.randint(1, deficit1 - deficit_thres) != 1)
    ):
        if len(ammo_put) > 0 and (
                len(ammo_get) == 0 or
                (deficit2 > deficit_thres and random.randint(1, deficit2 - deficit_thres) != 1)
        ):
            ammo.append( ammo_put[-1] )
            del ammo_put[-1]

            deficit2 -= 1
        else:
            ammo.append( ammo_get[-1] )
            del ammo_get[-1]

            deficit2 += 1

        deficit1 -= 1
    else:
        ammo.append( ammo_post[-1] )
        del ammo_post[-1]

        deficit1 += 1

    if random.randint(1, 20) == 1:
        ammo.append( ('GET', '/top', 'get_top', '') )

make_all_ammo(ammo)
sys.stderr.write('Total %d requests\n' % len(ammo))
