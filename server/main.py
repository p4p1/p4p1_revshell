#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Script made by papi
import p4p1
from sys import argv
script, port = argv

print """[*] p4p1 - server listening on 4441
[*] waiting for client to connect...."""
p4p1.usage()
s = p4p1.server(int(port))
s.main()
