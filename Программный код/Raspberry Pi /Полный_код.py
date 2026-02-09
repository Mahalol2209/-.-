from picamera import PiCamera
import pygame
import picamera.array
import numpy as np
import serial
import time

camera = PiCamera()

camera.rotation = 180

pygame.init()
screen = pygame.display.set_mode((640, 480))
pygame.display.set_caption("Camera + Arduino UART System")

# Инициализация камеры PiCamera
camera.resolution = (640, 480)
camera.start_preview()

# Подключение к Arduino через UART
ser = serial.Serial('/dev/ttyS0', 9600, timeout=1)
time.sleep(2)   # Ждем сброс Arduino

# Функция отправки цвета
def send_color_to_arduino(color):
    ser.write((color + '\n').encode('utf-8'))

try:
    while True:
        # Захватываем RGB кадр из камеры
        image_array = np.empty((480, 640, 3), dtype=np.uint8)
        camera.capture(image_array, 'rgb')
        
        # Анализ среднего цвета по всему изображению
        avg_r = int(np.mean(image_array[:,:,0]))
        avg_g = int(np.mean(image_array[:,:,1]))
        avg_b = int(np.mean(image_array[:,:,2]))
        
        # Определение цвета по RGB порогам
        if avg_r > 135 and avg_g > 135 and avg_b > 135:
            color_name = "white"
        elif avg_b > avg_r + 30 and avg_b > avg_g + 30 and avg_b > 85:
            color_name = "blue"
        elif avg_r > avg_g + 55 and avg_r > avg_b + 40 and avg_r > 100:
            color_name = "red"
        elif avg_g > avg_r + 45 and avg_g > avg_b + 55 and avg_g > 115:
            color_name = "green"
        else:
            color_name = "No"
        
        # Отправляем цвет на Arduino
        send_color_to_arduino(color_name)
        
        # Отображение видео на мониторе
        surface = pygame.surfarray.make_surface(np.rot90(image_array))
        screen.fill((0, 0, 0))
        screen.blit(surface, (0, 0))
        pygame.display.flip()
        
        # Обработка закрытия окна
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                raise KeyboardInterrupt
        
        time.sleep(0.033)   # 30 FPS

except KeyboardInterrupt:
    pass
finally:
    # Освобождаем все ресурсы
    camera.stop_preview()
    camera.close()
    ser.close()
    pygame.quit()
