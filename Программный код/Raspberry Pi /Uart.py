import serial
import time

# Подключение к Arduino через UART
ser = serial.Serial('/dev/ttyS0', 9600, timeout=1) 
time.sleep(2)   # Ждем сброс Arduino после открытия порта

# Функция отправки слова
def send_color_to_arduino(color):
    ser.write((color + '\n').encode('utf-8'))   # Отправляем цвет + перевод строки

try:
    # Тестовый цикл для проверки связи с Arduino
    while True:
        send_color_to_arduino("white")   # Отправляем белый
        time.sleep(1)
        send_color_to_arduino("blue")   # Отправляем синий
        time.sleep(1)
        send_color_to_arduino("red")   # Отправляем красный
        time.sleep(1)
        send_color_to_arduino("green")   # Отправляем зеленый
        time.sleep(1)
        send_color_to_arduino("No")   # Отправляем "No"
        time.sleep(1)

except KeyboardInterrupt:
    pass
finally:
    ser.close()   # Закрываем UART порт
