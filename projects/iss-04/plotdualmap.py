# 2D Karte fuer beide Sensor generieren

import matplotlib.pyplot as plt
import numpy as np
import csv
import sklearn

def pol2cart(rho, phi):
	phi += np.pi / 2
	# phi = -phi
	x = rho * np.cos(phi)
	y = rho * np.sin(phi)
	x = -x
	return(x, y)

sensor2_angle = np.radians(-45)
sensor2_translation = np.array((1.0, 0.0))
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

def calculate_kn_distance(x, y, k):
	kn_distance = []
	for i in range(x.size):
		eucl_dist = []
		for j in range(x.size):
			eucl_dist.append(
				np.sqrt(
					((x[i] - x[j]) ** 2) +
					((y[i] - y[j]) ** 2)))

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

plt.figure()
eps_dist = calculate_kn_distance(x1, y1, 4)
plt.hist(eps_dist, bins=30)
plt.ylabel('n')
plt.xlabel('Epsilon distance')
plt.show()


# DBSCAN Part
from sklearn import metrics
from sklearn.cluster import DBSCAN

X = zip(x1, y1)

print(X)

db = DBSCAN(eps=0.3, min_samples=10).fit(X)
labels = db.labels_

n_clusters_ = len(set(labels)) - (1 if -1 in labels else 0)
n_noise_ = list(labels).count(-1)

print("Estimated number of clusters: %d" % n_clusters_)
print("Estimated number of noise points: %d" % n_noise_)

unique_labels = set(labels)
core_samples_mask = np.zeros_like(labels, dtype=bool)
core_samples_mask[db.core_sample_indices_] = True

colors = [plt.cm.Spectral(each) for each in np.linspace(0, 1, len(unique_labels))]
for k, col in zip(unique_labels, colors):
	if k == -1:
		# Black used for noise.
		col = [0, 0, 0, 1]

	class_member_mask = labels == k

	xy = X[class_member_mask & core_samples_mask]
	plt.plot(
		xy[:, 0],
		xy[:, 1],
		".",
		markerfacecolor=tuple(col),
	)

	xy = X[class_member_mask & ~core_samples_mask]
	plt.plot(
		xy[:, 0],
		xy[:, 1],
		".",
		markerfacecolor=tuple(col),
	)

plt.title(f"Estimated number of clusters: {n_clusters_}")
plt.show()
