import serial
import time
import socket

ser = serial.Serial('COM3', 9600, timeout=1)
time.sleep(2)

def send_ue5(message, host='127.0.0.1', port=12345):
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
        s.connect((host, port))
        s.sendall(message.encode('utf-8'))

try:
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            print(f"Received: {line}")
            try:
                send_ue5(line)
            except Exception as e:
                print(f"Error sending data to UE5: {e}")
        time.sleep(0.1)
        
except KeyboardInterrupt:
    print("Program terminated!")
finally:
    ser.close()