import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

# Define x range
x = np.linspace(0, 270, 540)

# Define functions
def f1(x):
    return 0.018893*x - 0.150926

def f2(x):
    return 0.018149*x - 0.000068

# Compute y values
y1 = f1(x)
y2 = f2(x)

# Compute error
error = y1 - y2
abs_error = np.abs(error)
squared_error = error**2
mse = np.mean(squared_error)

# Prepare data for seaborn
df = pd.DataFrame({
    'x': x,
    'f1': y1,
    'f2': y2,
    'Absolute Error': abs_error,
    'Squared Error': squared_error
})

# Plot functions
sns.set(style='whitegrid')
plt.figure(figsize=(12, 6))

# Function plot
plt.subplot(1, 2, 1)
sns.lineplot(x='x', y='f1', data=df, label='f1: sin(x)')
sns.lineplot(x='x', y='f2', data=df, label='f2: x/2')
plt.title('Function Comparison')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()

# Error plot
plt.subplot(1, 2, 2)
sns.lineplot(x='x', y='Absolute Error', data=df, label='Absolute Error')
sns.lineplot(x='x', y='Squared Error', data=df, label='Squared Error')
plt.title(f'Error Between f1 and f2\nMSE = {mse:.4f}')
plt.xlabel('x')
plt.ylabel('Error')
plt.legend()

plt.tight_layout()
plt.show()
