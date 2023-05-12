import socket
import os
def get(filename, conn):
    conn.sendall(filename.encode())
    file = open(filename, 'wb')
    while True:
        chunk = conn.recv(1024)
        if not chunk:
            break
        file.write(chunk)
    file.close()
    print('File received successfully')
HOST = '127.0.0.1'
PORT = 6969

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind((HOST, PORT))
sock.listen()

print('Server listening on', HOST, 'port', PORT)

conn, addr = sock.accept()
print('Connected by', addr)
conn.sendall(b'Hello, world!')
while True:
    print(f"hero@{addr}:-$ ", end="")
    data = input()
    if data == 'exit':
        break
    elif data.startswith('get'):
        conn.sendall(data.encode())
        get(data[4:].strip(), conn)

    else:
        conn.sendall(data.encode())
        data = conn.recv(1024)
        print(data.decode())

    # print(len(data), 'bytes sent')
