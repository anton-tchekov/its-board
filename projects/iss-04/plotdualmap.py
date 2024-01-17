# 2D Karte fuer beide Sensor generieren

import matplotlib.pyplot as plt
import numpy as np
import csv

def pol2cart(rho, phi):
	x = rho * np.cos(phi)
	y = rho * np.sin(phi)
	return(x, y)

sensor2_angle = np.radians(130)
sensor2_translation = np.array((0.5, 0.1))
c = np.cos(sensor2_angle)
s = np.sin(sensor2_angle)
sensor2_rotation = np.array(((c, -s), (s, c)))

print("Sensor 2 Translation:")
print(sensor2_translation)
print("Sensor 2 Rotation:")
print(sensor2_rotation)

x1 = np.array(())
y1 = np.array(())
s1_angle = np.array(())
s1_dist = np.array(())
with open('testdata.csv', newline='') as csvfile:
	dreader = csv.reader(csvfile, delimiter=',', quotechar='|')
	for row in dreader:
		a = float(row[0])
		d = float(row[1])
		s1_angle = np.append(s1_angle, a)
		s1_dist = np.append(s1_dist, d)
		(x, y) = pol2cart(d, a)
		x1 = np.append(x1, x)
		y1 = np.append(y1, y)

x2 = np.array(())
y2 = np.array(())
s2_angle = np.array(())
s2_dist = np.array(())
with open('testdata.csv', newline='') as csvfile:
	dreader = csv.reader(csvfile, delimiter=',', quotechar='|')
	for row in dreader:
		a = float(row[0])
		d = float(row[1])
		s2_angle = np.append(s2_angle, a)
		s2_dist = np.append(s2_dist, d)
		(x, y) = pol2cart(d, a)
		out = sensor2_rotation.dot(np.array((x, y)))
		x = out[0]
		y = out[1]
		x += sensor2_translation[0]
		y += sensor2_translation[1]
		x2 = np.append(x2, x)
		y2 = np.append(y2, y)

def calculate_kn_distance(X, k):
	kn_distance = []
	for i in range(len(X)):
		eucl_dist = []
		for j in range(len(X)):
			eucl_dist.append(
				math.sqrt(
					((X[i,0] - X[j,0]) ** 2) +
					((X[i,1] - X[j,1]) ** 2)))

		eucl_dist.sort()
		kn_distance.append(eucl_dist[k])

	return kn_distance

fig, ax = plt.subplots()
ax.grid(True)

ax.set_aspect('equal', adjustable='box')
plt.xlim(-3, 3)
plt.ylim(-3, 3)
ax.plot(x1, y1, '.')
ax.plot(x2, y2, '.')
plt.show()

# eps_dist = calculate_kn_distance(X[1], 4)
# plt.hist(eps_dist, bins=30)
# plt.ylabel('n')
# plt.xlabel('Epsilon distance')
