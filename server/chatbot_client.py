import socket
import threading

HOST = "127.0.0.1"
PORT = 5000

def receive(sock):
    while True:
        try:
            data = sock.recv(1024).decode().strip()
            if data:
                print(f"[CLIENT] Received: {data}")
        except:
            break

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((HOST, PORT))

threading.Thread(target=receive, args=(client,), daemon=True).start()

while True:
    msg = input("> ")
    if msg:
        client.sendall((msg + "\n").encode())
