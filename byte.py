import freenect
import cv2
import numpy as np
import serial
import time

# Configurar a porta serial para comunicação com o Arduino
arduino = serial.Serial('/dev/cu.usbserial-1420', 9600)
time.sleep(2)  # Aguarde a inicialização da conexão serial

# Inicializar o classificador de cascata para detecção facial
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

def get_video():
    array, _ = freenect.sync_get_video()
    array = cv2.cvtColor(array, cv2.COLOR_RGB2BGR)
    return array

def detect_face_and_move_servo():
    frame = get_video()
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, 1.3, 5)

    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 2)
        # Calcular a posição do servo invertida com base na posição do rosto
        center_x = x + w // 2
        position = 180 - int((center_x / frame.shape[1]) * 180)  # Mapeando para o ângulo do servo invertido (180-0)
        print(f'Posição enviada ao Arduino: {position}')
        arduino.write(bytes([position]))

    cv2.imshow('Kinect Face Detection', frame)
    time.sleep(0.5)  # Adicionar um delay de 500 ms entre as leituras

while True:
    detect_face_and_move_servo()
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cv2.destroyAllWindows()
arduino.close()
