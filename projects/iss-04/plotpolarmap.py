# 2D Karte fuer ein Sensor generieren

import matplotlib.pyplot as plt
import numpy as np
import csv

title = 'Lidar Messdaten 2D Karte'

dist = np.array(())
angle = np.array(())

with open('sensor1.csv', newline='') as csvfile:
	dreader = csv.reader(csvfile, delimiter=',', quotechar='|')
	for row in dreader:
		angle = np.append(angle, float(row[0]))
		dist = np.append(dist, float(row[1]))

# dist = np.random.rand(360) / 5 + 1
# angle = 2 * np.pi * (np.arange(0, 360, 1) / 360)

fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})
fig.canvas.manager.set_window_title(title)

ax.plot(angle, dist, '.')
ax.set_rmax(2)
ax.set_theta_zero_location("N")
ax.set_theta_direction(-1)

ax.set_rticks([0.5, 1, 1.5, 2])
ax.set_rlabel_position(-22.5)
ax.grid(True)

ax.set_title(title, va='bottom')
plt.show()
