import serial
import time

porta = '/dev/ttyUSB0'
baud_rate = 115200
arquivo = '1k_coleta.txt'

ser = serial.Serial(porta,baud_rate)
time.sleep(2)

try:
    with open(arquivo, 'w') as f:
        linha = ser.readline().decode('utf-8').strip()
        print(linha)
        f.write(linha + '\n')
except KeyboardInterrupt:
    print("\nParando")
    ser.close()