# 2D Karte fuer ein Sensor generieren

import matplotlib.pyplot as plt
import numpy as np
import csv

with open('eggs.csv', newline='') as csvfile:
	spamreader = csv.reader(csvfile, delimiter=' ', quotechar='|')
	for row in spamreader:


sensor2_angle = np.radians(0)
sensor2_translation = np.array((0, 0))
c = np.cos(sensor2_angle)
s = np.sin(sensor2_angle)
sensor2_rotation = np.array(((c, -s), (s, c)))

print("Sensor 2 Translation:")
print(sensor2_translation)
print("Sensor 2 Rotation:")
print(sensor2_rotation)

title = 'Lidar Messdaten 2D Karte'

# TODO
dist = np.random.rand(360) / 5 + 1
angle = 2 * np.pi * (np.arange(0, 360, 1) / 360)

fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})
fig.canvas.manager.set_window_title(title)

ax.plot(angle, dist, '.')
ax.set_rmax(2)

ax.set_rticks([0.5, 1, 1.5, 2])
ax.set_rlabel_position(-22.5)
ax.grid(True)

ax.set_title(title, va='bottom')
plt.show()
