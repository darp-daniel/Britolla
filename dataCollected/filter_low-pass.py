from scipy import signal
import matplotlib.pyplot as plt
import numpy as np
import time
import serial

#Conexão Serial
porta = "/dev/ttyUSB0"
baud_rate = 115200
ser = serial.Serial(porta, baud_rate)
time.sleep(2)

#Captação Serial
y = []

for _ in range(1000):
    try:
        linha = ser.readline().decode().strip()
        if linha:
            print("Recebido:", linha)  # <-- DEBUG
            valor = float(linha)
            y.append(valor)
    except Exception as e:
        print("Erro ao ler valor:", e)



y = np.array(y)



# Parâmetros do sinal
samplingFreq = 400  # 4 kHz
tlims = [0, 1]       # 1 segundo
# signalFreq = [0.2, 0.5] Testa com uma frequência fora da banda também
signalMag = [1, 0.2]

# Tempo e sinal
t = np.arange(len(y)) / samplingFreq
# y = signalMag[0]*np.sin(2*np.pi*signalFreq[0]*t) + signalMag[1]*np.sin(2*np.pi*signalFreq[1]*t)


# FFT original
yhat = np.fft.fft(y)
fcycles = np.fft.fftfreq(len(t), d=1.0/samplingFreq)

# Filtro analógico (1ª ordem passa-baixa com 100 Hz de corte)
fc = 5  # frequência de corte
w0 = 2 * np.pi * fc
num = [w0]
den = [1, w0]
lowPass = signal.TransferFunction(num, den)

# Conversão para filtro digital (bilinear transform)
dt = 1.0 / samplingFreq
discreteLowPass = lowPass.to_discrete(dt, method='bilinear')
print(discreteLowPass)

# Coeficientes do filtro digital
b = discreteLowPass.num
a = discreteLowPass.den
print("Coeficientes b:", b)
print("Coeficientes a:", a)

print("Equação do filtro digital (diferença):")
print(f"y[n] = {b[0]:.6f} * x[n] + {b[1]:.6f} * x[n-1] - {a[1]:.6f} * y[n-1]")

# Aplicar filtro (filtro IIR: y[n] = b0*x[n] + b1*x[n-1] - a1*y[n-1])
yfilt = np.zeros(len(y))
for i in range(1, len(y)):
    yfilt[i] = b[0]*y[i] + b[1]*y[i-1] - a[1]*yfilt[i-1]

# FFT do sinal filtrado
yfilthat = np.fft.fft(yfilt)

# Plot para comparar
plt.figure(figsize=(12, 6))

plt.subplot(2,1,1)
plt.plot(t, y, label="Sinal original")
plt.plot(t, yfilt, label="Sinal filtrado", linestyle='--')
plt.xlabel("Tempo [s]")
plt.ylabel("Amplitude")
plt.legend()
plt.title("Sinal no tempo")

plt.subplot(2,1,2)
plt.semilogy(fcycles[:len(fcycles)//2], np.abs(yhat[:len(yhat)//2]), label="Original")
plt.semilogy(fcycles[:len(fcycles)//2], np.abs(yfilthat[:len(yfilthat)//2]), label="Filtrado")
plt.xlabel("Frequência [Hz]")
plt.ylabel("Magnitude")
plt.legend()
plt.title("FFT")

plt.tight_layout()
plt.show()
