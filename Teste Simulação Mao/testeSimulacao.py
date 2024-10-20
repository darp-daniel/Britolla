import pygame
import sys

# Configurações do Pygame
pygame.init()
screen = pygame.display.set_mode((800, 600))
pygame.display.set_caption('Simulação de Mão com Potenciômetros')

# Função para desenhar os dedos
def draw_fingers(angles):
    finger_width = 20
    finger_length = 100
    for i, angle in enumerate(angles):
        # Calcula a posição do dedo com base no ângulo
        x = 100 + i * 60
        y = 300
        end_x = x + finger_length * pygame.math.cos(pygame.math.radians(angle))
        end_y = y - finger_length * pygame.math.sin(pygame.math.radians(angle))
        
        pygame.draw.line(screen, (255, 0, 0), (x, y), (end_x, end_y), finger_width)

# Loop principal
try:
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
except KeyboardInterrupt:
    pygame.quit()