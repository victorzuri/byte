import serial
import time
from pynput import mouse
import threading
import psutil

ser = serial.Serial('COM3', 115200)
time.sleep(2)

neck_move = False

def cs2_rodando():
    """Verifica se o processo cs2.exe tá rodando"""
    for proc in psutil.process_iter(['name']):
        try:
            if 'cs2.exe' in proc.info['name'].lower():
                return True
        except:
            continue
    return False

def mexe_neck():
    while neck_move:
        ser.write(b'N120\n')
        time.sleep(0.1)
        ser.write(b'N90\n')
        time.sleep(0.1)

def on_click(x, y, button, pressed):
    global neck_move
    if button == mouse.Button.left and cs2_rodando():
        if pressed:
            neck_move = True
            threading.Thread(target=mexe_neck, daemon=True).start()
        else:
            neck_move = False
            ser.write(b'N90\n')

listener = mouse.Listener(on_click=on_click)
listener.start()

try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    print("Encerrado pelo usuário.")
