import serial
import time

porta = '/dev/ttyUSB0'
baud_rate = 115200
arquivo = '10k_coleta1.txt'
linhaPrev = None

ser = serial.Serial(porta,baud_rate)
time.sleep(2)

try:
    while(True):
        with open(arquivo, 'a') as f:
            linha = ser.readline().decode('utf-8').strip()
            if linha != linhaPrev:
                print(linha)
                f.write(linha + '\n')
                linhaPrev = linha
except KeyboardInterrupt:
    print("\nParando")
    ser.close()