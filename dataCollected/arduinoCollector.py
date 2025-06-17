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
        assert {'angle', 'volts'}.issubset(data.columns)
        return data
    except Exception as e:
        print(f"Data loading error: {str(e)}")
        exit(1)

def linear_fit(x, a, b):
    """Linear fitting function."""
    return a * x + b

# Load the data
data = load_data('10k_coleta.csv')
x_data = data['angle'].values
y_data = data['volts'].values

# Create figure
fig, ax = plt.subplots(figsize=(12, 7))
plt.subplots_adjust(left=0.25)  # Leave space for checkbox

# Plot the measurement data
scatter = ax.scatter(x_data, y_data,
                     color='#1f77b4',
                     s=80,
                     alpha=0.8,
                     label='Medida')
plot_elements = [scatter]

# Fit the data
params, pcov = curve_fit(linear_fit, x_data, y_data)
perr = np.sqrt(np.diag(pcov))  # Parameter uncertainties

# Fit line
x_fit = np.linspace(min(x_data), max(x_data), 100)
y_fit = linear_fit(x_fit, *params)
fit_label = (
    f'Função Potenciômetro 10k:\n'
    f'y = {params[0]:.6f}x ±{perr[0]:.6f} + {params[1]:.6f} ±{perr[1]:.6f}'
)
fit_line, = ax.plot(x_fit, y_fit,
                    'k--', linewidth=2,
                    label=fit_label)
plot_elements.append(fit_line)

# Add checkboxes
labels = ['Medida', 'Ajuste']
checkbox_ax = plt.axes([0.05, 0.4, 0.15, 0.15])
check = CheckButtons(checkbox_ax, labels, actives=[True, True])

def toggle_element(label):
    index = labels.index(label)
    plot_elements[index].set_visible(not plot_elements[index].get_visible())
    plt.draw()

check.on_clicked(toggle_element)

# Final formatting
ax.set_xlabel('Ângulo (º)', fontsize=12)
ax.set_ylabel('Volts', fontsize=12)
ax.set_title('Resistor 10k', fontsize=14)
ax.legend(loc='upper left', framealpha=1)

# Save figure
plt.savefig('results/arduinoColl.png',
            dpi=300,
            bbox_inches='tight',
            transparent=False)
plt.show()

# Print fit result
print(f"\nResultado do ajuste: y = {params[0]:.6f}x + {params[1]:.6f}")
print(f"Incertezas: k = ±{perr[0]:.6f}, b = ±{perr[1]:.6f}")
