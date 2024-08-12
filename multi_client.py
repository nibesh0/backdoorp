import socket

class Client:
    def __init__(self, server_ip, port):
        self.server_ip = server_ip
        self.port = int(port)
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        try:
            self.sock.connect((self.server_ip, self.port))
            print("Connected to server.")
        except socket.error as e:
            self.sock.close()
            raise Exception(f"Unable to connect to server: {e}")

    def run(self):
        while True:
            command = input("reQ: ")

            if command == "..out":
                break

            self.sock.sendall(command.encode('utf-8'))

            try:
                data = self.sock.recv(1024)
                if data:
                    print("re$: ", data.decode('utf-8'))
                else:
                    print("Connection closed by server.")
                    break
            except socket.error as e:
                print(f"Error receiving data: {e}")
                break

        self.sock.close()

    def __del__(self):
        self.sock.close()

if __name__ == "__main__":
    import sys
    if len(sys.argv) < 2:
        print("Error: IP address required.")
    else:
        try:
            client = Client(sys.argv[1], "6969")
            client.run()
        except Exception as e:
            print(f"Exception: {e}")
