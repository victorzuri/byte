from pynput import mouse
import serial
import time
import threading

ser = serial.Serial('COM3', 115200, timeout=0.01)
time.sleep(2)

last_y = None
last_recoil_time = 0
recoil_cooldown = 0.2
recoil_ativo = False
botao_pressionado = False

def controlar_neck_loop():
    global recoil_ativo
    start_time = time.time()
    while time.time() - start_time < 1.0:
        ser.write(b'N120\n')
        time.sleep(0.05)
        ser.write(b'N100\n')
        time.sleep(0.05)
    recoil_ativo = False

def on_click(x, y, button, pressed):
    global botao_pressionado
    if button == mouse.Button.left:
        botao_pressionado = pressed

def on_move(x, y):
    global last_y, recoil_ativo, last_recoil_time, botao_pressionado

    if last_y is None:
        last_y = y
        return

    delta_y = last_y - y
    last_y = y

    agora = time.time()

    print(f"[DEBUG] delta_y={delta_y:.2f} | pressed={botao_pressionado} | cooldown={agora - last_recoil_time:.2f}")
    time.sleep(0.5)
    if (
        botao_pressionado and
        delta_y > 6 and
        not recoil_ativo and
        (agora - last_recoil_time) > recoil_cooldown
    ):
        print("[DEBUG] >>> RECOIL DETECTADO <<<")
        time.sleep(5)
        recoil_ativo = True
        last_recoil_time = agora
        threading.Thread(target=controlar_neck_loop, daemon=True).start()


mouse_listener = mouse.Listener(on_move=on_move, on_click=on_click)
mouse_listener.start()

try:
    while True:
        time.sleep(0.01)
except KeyboardInterrupt:
    pass
