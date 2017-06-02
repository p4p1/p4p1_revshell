# usage functions for p4p1
import sys

def usage_shell():
    print "HELP | Server:"
    print "commands with the * 'astericks' in front are not available yet"
    print "they are comming soon :D"
    print "ip -> print the ip of the client connected"
    print "help -> show this message"
    print "wget [url] -> download file from the web"
    print "get-file [file] -> download a file that is on the connected computer"

def usage_bin():
    print "./p4p1.py [port]"
    print " - This program is the server for p4p1.exe"
    print " - please port forward you're router if you are not using this"
    print " - on you're local network."
    sys.exit(1)

