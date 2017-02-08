#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Script made by papi
import p4p1
import sys, os
from sys import argv

def usage():
	print "./p4p1.py [port]"
	print " - This program is the server for p4p1.exe"
	print " - please port forward you're router if you are not using this"
	print " - on you're local network."
	sys.exit(1)

try:
	script, port = argv
except:
	usage()

print """\
[*] p4p1 - server listening on 4441
[*] waiting for client to connect....\
"""
p4p1.usage()
s = p4p1.server(int(port))
s.main()
