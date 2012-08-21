#ifndef __CONFIG_H
#define __CONFIG_H

/* edit this */

// this is the servers ip and port
#define HOST "http://192.168.0.23:8888/"

// amount of seconds between to getCommand requests
#define POLL_INTERVAL 1

// the maximum size of an URL
// The standard sets no limit. But maybe your browser does
#define MAX_URL_SIZE 400

/* normally you don't want to edit this */
#define POLL "getCommand"
#define RESPONSE "response?info="
#define RESULT "sendResult?data="
#define BEGIN "beginResult"
#define END "endResult"


#endif
