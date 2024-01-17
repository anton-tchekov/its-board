import matplotlib.pyplot as plt
import numpy as np
import csv

def pol2cart(rho, phi):
	phi += np.pi / 2
	x = rho * np.cos(phi)
	y = rho * np.sin(phi)
	x = -x
	return(x, y)

sensor2_angle = np.radians(-180)
sensor2_translation = np.array((0.0, 910))
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
with open('ACM0_result.txt', newline='') as csvfile:
	dreader = csv.reader(csvfile, delimiter=',', quotechar='|')
	for row in dreader:
		a = np.radians(float(row[0]))
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
with open('ACM1_result.txt', newline='') as csvfile:
	dreader = csv.reader(csvfile, delimiter=',', quotechar='|')
	for row in dreader:
		a = np.radians(float(row[0]))
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

fig, ax = plt.subplots()
ax.grid(True)

ax.set_aspect('equal', adjustable='box')
plt.xlim(-4000, 4000)
plt.ylim(-4000, 4000)
ax.plot(x1, y1, '.')
ax.plot(x2, y2, '.')
plt.show()

