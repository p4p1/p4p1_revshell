#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Script made by papi
# p4p1 Server, this works on mac and linux and windows if you hve the python
# interpreter. this script will listen on port 4441 and will pop a prompt
# if a connection is received by the p4p1 remote shell.

class server():
	'''server class that received the connection and set it up'''

	def __init__(self, port, encryption=False):
		self.port = port
		self.log_file = open("server.log", "w")
		self.socket = 

	def get_connection(self):
