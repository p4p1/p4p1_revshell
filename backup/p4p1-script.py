#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Script made by papi
# To replace netcat as server for the p4p1-o.exe
# Usage:
#		./script.py

import os, sys
import socket

buf_size=9999
bind_ip="0.0.0.0"
bind_port=4441
prompt="<p4p1 />"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((bind_ip, bind_port))
s.listen(5)

print "[*] Listening on %s:%d" % (bind_ip, bind_port)

def handle_client(client_socket):
	client_socket.send("5")	#send the id to indicate the mode
	while True:
		buf = raw_input(prompt)
		client_socket.send(buf)
		if buf == "&q":
			break
		buf = client.recv(buf_size)
		print buf

client, addr = s.accept()
print "[*] Accepted connection from %s:%d" % (addr[0], addr[1])
handle_client(client)
