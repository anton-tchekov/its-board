from sklearn import metrics
from sklearn.cluster import DBSCAN
import matplotlib.pyplot as plt
import numpy as np
import csv

def pol2cart(rho, phi):
	phi += np.pi / 2
	x = rho * np.cos(phi)
	y = rho * np.sin(phi)
	x = -x
	return(x, y)

X = np.zeros((682-79-19-8-21, 2))
i = 0

with open('ACM0_result.txt', newline='') as csvfile:
	dreader = csv.reader(csvfile, delimiter=',', quotechar='|')
	for row in dreader:
		a = np.radians(float(row[0]))
		d = float(row[1])
		(x, y) = pol2cart(d, a)
		if not (x < 30 and y < 30 and x > -30 and y > -30):
			X[i,:] = [ x, y ]
			i += 1

# X = X.reshape(-1, 1)

plt.scatter(X[:, 0], X[:, 1])
plt.show()

print(X)

db = DBSCAN(eps=180, min_samples=4).fit(X)
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


# def calculate_kn_distance(x, y, k):
# 	kn_distance = []
# 	for i in range(x.size):
# 		eucl_dist = []
# 		for j in range(x.size):
# 			eucl_dist.append(
# 				np.sqrt(
# 					((x[i] - x[j]) ** 2) +
# 					((y[i] - y[j]) ** 2)))
# 
# 		eucl_dist.sort()
# 		kn_distance.append(eucl_dist[k])
# 
# 	return kn_distance
# 
# 
# 
# plt.figure()
# eps_dist = calculate_kn_distance(x1, y1, 4)
# plt.hist(eps_dist, bins=30)
# plt.ylabel('n')
# plt.xlabel('Epsilon distance')
# plt.show()

