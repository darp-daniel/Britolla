import serial
import time
from math import cos, sin, radians, pi

porta = '/dev/ttyUSB0'
baud_rate = 115200
ser = serial.Serial(porta,baud_rate)
time.sleep(2)

def calcular_posicao(linha):
    try:
        dados = linha.split(',')
        if len(dados) < 3:
            return None
        
        ang1 = float(dados[0])
        ang2 = float(dados[2])
        ang1 = radians(ang1)
        ang2 = radians(ang2)
        x = 140 * (cos(ang1) + cos(ang1 + ang2))
        y = 140 * (sin(ang1) + sin(ang1 + ang2))

        
        return (x, y)
    except ValueError:
        return None