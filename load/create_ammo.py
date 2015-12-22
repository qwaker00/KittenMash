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

ammo = []
for item in db.votes.find():
    left, right, vote = item['leftId'], item['rightId'], item['_id']
    ammo.append( ('GET', '/vote/%s' % vote, 'get_vote', '') )
    ammo.append( ('GET', '/image/%s' % left, 'get_image', '') )
    ammo.append( ('GET', '/image/%s' % right, 'get_image', '') )
    ammo.append( ('PUT', '/vote/%s/result' % vote, 'set_vote', 'left' if random.randint(0, 1) == 0 else 'right') )
    if random.randint(1, 20) == 1:
        ammo.append( ('GET', '/top', 'get_top', '') )

make_all_ammo(ammo)
sys.stderr.write('Total %d requests\n' % len(ammo))
