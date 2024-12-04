import serial
import math
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import csv
import numpy as np

#COM
SERIAL_PORT = 'COM5'
BAUD_RATE = 9600

#CONEXAO

#controle = 0
conser = serial.Serial(SERIAL_PORT, BAUD_RATE)

#SENSOR
currtime = []
potenciometro = []
# Equação a ser utilizada ==> k = tan(pi*(vout/vin - 1/2))/d

#LEITURA
def leitura_sensor():
    linha = conser.readline().decode('utf-8').strip()
    valores = linha.split(', ')
    currtime.append(float(valores[0]))
    potenciometro.append(float(valores[1])) 

#PLOTAGEM
def plotagem_up(frame):
    leitura_sensor()
    plt.cla()
    plt.scatter(currtime, potenciometro, label='Potenciômetro')
    plt.xlabel('Tempo')
    plt.ylabel('Ângulo')
    plt.legend()

#SALVAMENTO
def salvar(event):
    with open('arduino_data.csv', 'w', newline= '') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(['Time', 'Potenciometro'])
        for t, pot in zip(currtime, potenciometro):
            writer.writerow([t, pot])

#MAIN
fig, ax = plt.subplots()
fig.canvas.mpl_connect('close_event', salvar)

ani = FuncAnimation(fig, plotagem_up, interval=10)
plt.show()