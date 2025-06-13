#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import CheckButtons
from scipy.optimize import curve_fit

# Set style
plt.style.use('seaborn')
plt.rcParams.update({
    'font.size': 12,
    'axes.grid': True,
    'grid.alpha': 0.3
})

def load_data(filepath):
    """Load and validate CSV data."""
    try:
        data = pd.read_csv(filepath, skipinitialspace=True)
        data = data.apply(pd.to_numeric, errors='coerce').dropna()
        assert {'angle', 'medida_1', 'medida_2', 'medida_3'}.issubset(data.columns)
        return data
    except Exception as e:
        print(f"Data loading error: {str(e)}")
        exit(1)

def linear_fit(x, a, b):
    """Linear fitting function."""
    return a * x + b

# Main analysis
data = load_data('/home/daniel/Desktop/WorkSpace/Britolla/dataCollected/data/10k_multimetro.csv')
x_data = data['angle'].values
measurements = data[['medida_1', 'medida_2', 'medida_3']].values

# Create figure with interactive controls
fig, ax = plt.subplots(figsize=(12, 7))
plt.subplots_adjust(left=0.25)  # Space for checkboxes

# Plot measurements (no error bars)
plot_elements = []
colors = ['#1f77b4', '#ff7f0e', '#2ca02c']  # Blue, Orange, Green
labels = ['Medida 1', 'Medida 2', 'Medida 3']

for i, (color, label) in enumerate(zip(colors, labels)):
    scatter = ax.scatter(x_data, measurements[:, i],
                       color=color,
                       s=80,
                       alpha=0.8,
                       label=label)
    plot_elements.append(scatter)

# Perform linear fit
all_x = np.tile(x_data, 3)
all_y = measurements.T.flatten()

params, pcov = curve_fit(linear_fit, all_x, all_y)
perr = np.sqrt(np.diag(pcov))  # Parameter uncertainties

# Plot fit line
x_fit = np.linspace(min(x_data), max(x_data), 100)
fit_label = f'Função Potenciômetro 10k: {params[0]:.6f}x ±{perr[0]:.6f} + {params[1]:.6f} ±{perr[1]:.6f} '
fit_line, = ax.plot(x_fit, linear_fit(x_fit, *params),
                   'k--', linewidth=2,
                   label=fit_label)
plot_elements.append(fit_line)

# Add interactive checkboxes
checkbox_ax = plt.axes([0.05, 0.4, 0.15, 0.15])
check = CheckButtons(checkbox_ax, labels + ['Ajuste'], 
                    actives=[True]*4)

def toggle_element(label):
    """Callback for checkbox clicks."""
    index = labels.index(label) if label in labels else 3
    plot_elements[index].set_visible(not plot_elements[index].get_visible())
    plt.draw()

check.on_clicked(toggle_element)

# Format plot
ax.set_xlabel('Ângulo (º)', fontsize=12)
ax.set_ylabel('Volts', fontsize=12)
ax.set_title('Resistor 10k', fontsize=14)
ax.legend(loc='upper left', framealpha=1)

# Save and show
plt.savefig('dataCollected/results/resistor10k.png', 
           dpi=300, 
           bbox_inches='tight',
           transparent=False)
plt.show()

# Print results
print(f"\nResultado do ajuste: y = {params[0]:.6f}x + {params[1]:.6f}")
print(f"Incertezas: k = ±{perr[0]:.6f}, b = ±{perr[1]:.6f}")