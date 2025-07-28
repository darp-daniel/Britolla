import socket

HOST = '127.0.0.1'  # ou '0.0.0.0' para aceitar conexões externas
PORT = 12345

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
    server_socket.bind((HOST, PORT))
    server_socket.listen(1)
    print(f"[SERVER] Aguardando conexão em {HOST}:{PORT}...")

    conn, addr = server_socket.accept()
    print(f"[SERVER] Conectado por {addr}")

    with conn:
        try:
            while True:
                data = conn.recv(1024)
                if not data:
                    break
                print(f"[RECEBIDO] {data.decode('utf-8')}")
        except KeyboardInterrupt:
            print("[SERVER] Encerrando o servidor.")
        finally:
            conn.close()
            print("[SERVER] Conexão encerrada.")