#! /usr/bin/python

import threading
import sys

class Shell(threading.Thread):

    def __init__(self):
        self.condition = threading.Condition()
        self.mutex = threading.Lock()
        self.command = ""
        self.running = True
        threading.Thread.__init__(self)
        self.setDaemon(True)

    def run(self):
        while self.running:
            print "wwwsh:> ", 
            command = sys.stdin.readline()[:-1]

            self.mutex.acquire()
            self.command = command
            self.mutex.release()

            self.condition.acquire()
            self.condition.wait()
            result = self.result
            self.condition.release()
            print result 
        

    def getCommand(self):
        self.mutex.acquire()
        command = self.command
        self.command = ""
        self.mutex.release()
        return command

    def response(self, response):
        self.condition.acquire()
        self.result = response
        self.condition.notify()
        self.condition.release()
  

shell = None

if __name__ == '__main__':
    print "module is not meant to be started"
else:
    shell = Shell()
    shell.start()
