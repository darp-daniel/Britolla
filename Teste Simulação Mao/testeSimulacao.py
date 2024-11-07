import pygame
import sys
import serial
import numpy as np

# COM port configuration
SERIAL_PORT = 'COM5'
BAUD_RATE = 9600

# Serial connection
conser = serial.Serial(SERIAL_PORT, BAUD_RATE)

def leiturasensor():
    linha = conser.readline().decode('utf-8').strip()
    valores = linha.split(', ')
    valoresR = [float(i) for i in valores]
    matriz = np.array(valoresR)
    return matriz

# Pygame setup
pygame.init()
screen = pygame.display.set_mode((800, 600))
pygame.display.set_caption('Simulação de Mão com Potenciômetros')

# Colors
branco = (255, 255, 255)
pele = (139, 69, 19)

# Dictionary to store finger positions
pos_dedos = {}

def dedosMud(angle):
    return angle * 100 / 90

# Draw the palm of the hand
def desenhar_mao(x, y):
    pygame.draw.rect(screen, pele, (x, y, 100, 150))

# Draw the fingers
def draw_fingers(angles):
    dedos = ['indicador', 'medio', 'anelar', 'minimo', 'polegar']
    x = 358
    y = 300
    distancia_dedos = 26
    finger_width = 15
    
    for i, dedo in enumerate(dedos[:-1]):  # For the four main fingers
        finger_length = dedosMud(angles[i])
        pos = pygame.draw.line(screen, pele, (x, y), (x, y - finger_length), finger_width)
        pos_dedos[dedo] = pos
        x += distancia_dedos

# Main loop
clock = pygame.time.Clock()
try:
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()

        screen.fill(branco)
        desenhar_mao(350, 300)

        # Read angles and draw fingers
        angles = leiturasensor()
        draw_fingers(angles)
        clock.tick(120)

        pygame.display.flip()
except KeyboardInterrupt:
    pygame.quit()
