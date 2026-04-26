import socket
import time

HOST = "127.0.0.1"
PORT = 5000

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((HOST, PORT))

temp = 25

while True:
    msg = f"REAL:{temp}"
    client.sendall((msg + "\n").encode())
    print("[SIM] Sent:", msg)

    temp += 1
    time.sleep(1)
