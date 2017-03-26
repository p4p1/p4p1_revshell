#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Script made by papi
# p4p1 Server, this works on mac and linux and windows if you hve the python
# interpreter. this script will listen on port 4441 and will pop a prompt
# if a connection is received by the p4p1 remote shell.

import sys, os
import socket

def usage():
    print "HELP | Server:"
    print "commands with the * 'astericks' in front are not available yet"
    print "they are comming soon :D"
    print "ip -> print the ip of the client connected"
    print "help -> show this message"
    print "wget [url] -> download file from the web"
    print "*save [file] -> download a file that is on the connected computer"

class server():
    '''server class that received the connection and set it up'''

    def __init__(self, port, encryption=False):
        self.port = port
        self.log_file = open("/tmp/p4p1_server.log", "a")
        self.prompt = "<p4p1 /> "
        self.buf = ""
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    def handle_client(self, client_sock):
        while True:
            self.buf = raw_input(self.prompt)
            if self.buf == "exit":
                break
            elif self.buf == "help":
                usage()
            elif self.buf == "ip":
                self.buf = self.addr[0]
            else:
                client_sock.send(self.buf)
                self.log_file.write("command sent : {0}\n".format(self.buf))
                self.buf = client_sock.recv(2048)
            print self.buf

    def handle_old_client(self, client_sock):
        client_sock.send('0')
        print client_sock.recv(2048)
        while True:
            self.buf = raw_input(client_sock.recv(2048))
            if self.buf == "exit":
                break
            elif self.buf == "help":
                usage()
            elif self.buf == "ip":
                self.buf = self.addr[0]
            else:
                client_sock.send(self.buf)
                self.log_file.write("command sent : {0}\n".format(self.buf))
                self.buf = client_sock.recv(2048)
            print self.buf


    def main(self, old=False):
        self.sock.bind(("0.0.0.0", self.port))
        self.sock.listen(5)
        client, self.addr = self.sock.accept()
        self.log_file.write("connection from : {0}\n".format(self.addr[0]))
        if not old:
            self.handle_client(client)
        else:
            self.handle_old_client(client)

if __name__ == "__main__":
    s = server(4441)
    s.main()
