import socket
import threading

HOST = "0.0.0.0"
PORT = 5000

clients = []

def process_data(data):
    # Simple decision logic
    try:
        if data.startswith("REAL:"):
            value = float(data.split(":")[1])
            if value > 30:
                return "CMD:COOL:ON"
            else:
                return "CMD:IDLE"
    except:
        pass
    return None

def handle_client(conn, addr):
    print(f"[SERVER] Connected: {addr}")
    clients.append(conn)

    while True:
        try:
            data = conn.recv(1024).decode().strip()
            if not data:
                break

            print(f"[SERVER] Received: {data}")

            response = process_data(data)
            if response:
                conn.sendall((response + "\n").encode())
                print(f"[SERVER] Sent: {response}")

        except:
            break

    print(f"[SERVER] Disconnected: {addr}")
    clients.remove(conn)
    conn.close()

def start_server():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((HOST, PORT))
    server.listen(5)

    print(f"[SERVER] Listening on {HOST}:{PORT}")

    while True:
        conn, addr = server.accept()
        thread = threading.Thread(target=handle_client, args=(conn, addr))
        thread.start()

if __name__ == "__main__":
    start_server()
