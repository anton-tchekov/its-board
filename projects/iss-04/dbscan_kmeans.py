from sklearn.preprocessing import StandardScaler
from sklearn import metrics
from sklearn.cluster import DBSCAN
from sklearn import preprocessing
from sklearn.decomposition import PCA
from sklearn.cluster import KMeans

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# Einlesen der Senordaten aus der csv-Datei
dateipfad = 'imu.txt'
anzahl_splaten = 12
sensordaten = pd.read_csv(dateipfad, header=None, usecols= range(1,anzahl_splaten+1))

# Standardisiert
scaler = StandardScaler()
sensordaten_scaled = sensordaten.copy()
sensordaten_scaled = pd.DataFrame(scaler.fit_transform(sensordaten_scaled), columns=sensordaten_scaled.columns)

# PCA Hauptkomponentenanalyse
pca = PCA(n_components=3)
sensordaten_pca = pca.fit_transform(sensordaten_scaled)

X = sensordaten_pca

# DBSCAN
db = DBSCAN(eps=0.3, min_samples=10).fit(X)
y_dbscan = DBSCAN(eps=0.3, min_samples=10).fit_predict(X)
labels = db.labels_
n_clusters_ = len(set(labels)) - (1 if -1 in labels else 0)
n_noise_ = list(labels).count(-1)
print("Estimated number of clusters: %d" % n_clusters_)
print("Estimated number of noise points: %d" % n_noise_)

# kmeans
kmeans = KMeans(n_clusters=2, random_state=0, n_init="auto").fit(X)
y_kmeans = KMeans(n_clusters=2, random_state=0, n_init="auto").fit_predict(X)

# Ergebnis in Achsen aufteilen für die Plotts
x_pca = sensordaten_pca[:,0]
y_pca = sensordaten_pca[:,1]
z_pca = sensordaten_pca[:,2]

# Plott von t-SNE
fig = plt.figure()
ax = fig.add_subplot(1, 2, 1,projection='3d')
ax.scatter(x_pca, y_pca, z_pca, c=y_kmeans)
ax.set_xlabel("PCA 1")
ax.set_ylabel("PCA 2")
ax.set_zlabel("PCA 3")
ax.set_title("k-means")
# Plott von DBSCAN
ax1 = fig.add_subplot(1, 2, 2,projection='3d')
ax1.scatter(x_pca, y_pca, z_pca, c=y_dbscan)
ax1.set_xlabel("PCA 1")
ax1.set_ylabel("PCA 2")
ax1.set_zlabel("PCA 3")
ax1.set_title("DBSCAN")

fig.suptitle("Plott des Datensatzes (3D)", fontsize=14)
fig.set_figheight(9)
fig.set_figwidth(15)
plt.show()