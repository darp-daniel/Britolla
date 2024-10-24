import socket
import struct
import time
import serial
import numpy as np

#Função para ler o do arduino
def conexaoArduino(portaCom, rate):
    #COM and RATE
    SERIAL_PORT = portaCom
    BAUD_RATE = rate

    #CONEXAO
    conser = serial.Serial(SERIAL_PORT, BAUD_RATE)
    return conser

#Leitura do sensor
def leiturasensor(conser):
    linha = conser.readline().decode('utf-8').strip()
    valores = linha.split(', ')
    dedos = np.array([float(val) for val in valores[1:]])
    return dedos
def conexaoUnity(host, porta):
    # Configurações do servidor
    HOST = '127.0.0.1'  # Endereço local
    PORT = 65432        # Porta

    # Criar socket (TCP/IP)
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((HOST, PORT))
    server_socket.listen(1)

    print('Aguardando conexão')
    conn, addr = server_socket.accept()
    print('Conectado ', addr)
    return conn, server_socket

try:
    con = conexaoArduino('127.0.0.1', 65432)
    connserial = conexaoArduino('COM5', 9600)
    while True:
        #Dados potenciometros coletados
        dedos = leiturasensor(connserial)
        
        # Enviar dados como bytes e estruturados
        data = struct.pack('fffff', *dedos)
        con[0].sendall(data)
        
        time.sleep(0.1)  # Intervalo entre os envios de dados
except Exception:
    print('Deu erro!!')
finally:
    con[0].close()
    con[1].close()
