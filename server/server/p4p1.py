import os,sys
import socket, server.usage as usage, server.handle_client as handle_client

class server():

    def __init__(self, port):
        self.port = port
        self.log_file = open("/tmp/p4p1_server.log", "a")
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    def main(self, old=False):
        self.sock.bind(("0.0.0.0", self.port))
        self.sock.listen(5)
        client, addr = self.sock.accept()
        hd = handle_client.client_handler(client, addr)

        if not old:
            hd.handle_client()
        else:
            hd.handle_old_client()

if __name__ == "__main__":
    s = server(4441)
    s.main()
