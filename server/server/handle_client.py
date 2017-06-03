#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Script made by papi
# p4p1 Server, this works on mac and linux and windows if you hve the python
# interpreter. this script will listen on port 4441 and will pop a prompt
# if a connection is received by the p4p1 remote shell.

import sys, os
import socket
import server.usage as usage

class client_handler():
    ''' client handler class, handles the different clients '''

    def __init__(self, sock, ip):
        self.log_file = open("/tmp/p4p1_server.log", "a")
        self.prompt = '<p4p1 |%s| />' % ip[0]
        self.addr = ip
        self.sock = sock
        self.buf = ""

    def receive_from(self, socket):
        buffer = ""
        socket.settimeout(1)
        try:
            while True:
                data = socket.recv(4096)
                if not data:
                    break
                buffer += data
        except:
            pass
        return buffer

    def handle_client(self):
        while True:
            self.buf = raw_input(self.prompt)
            if len(self.buf) >= 1024:
                print "[!] buf size to long!"
                continue
            if self.buf == "exit":
                break
            elif self.buf == "help":
                usage.usage_shell()
            elif self.buf == "ip":
                self.buf = self.addr[0]
            elif "get-file" in self.buf:
                self.sock.send(self.buf)
                data = self.receive_from(self.sock)
                if "-1" in str(data):
                    print "Error With the file, this error is client side"
                else:
                    with open("/tmp/" + self.buf[9:], 'wb') as f:
                        f.write(data)
                        f.close()
                self.buf = "[!] File downloaded in /tmp/{0}.".format(self.buf[9:])
            else:
                self.sock.send(self.buf)
                self.buf = self.receive_from(self.sock)
            print self.buf

    def handle_old_client(self):
        self.sock.send('5')
        while True:
            self.buf = raw_input(self.prompt)
            if self.buf == "exit":
                self.sock.send('&')
                break
            elif self.buf == "help":
                usage.usage_shell()
            elif self.buf == "ip":
                self.buf = self.addr[0]
            else:
                self.sock.send(self.buf)
                self.buf = self.sock.recv(2048)
            print self.buf
