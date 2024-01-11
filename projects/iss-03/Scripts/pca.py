import numpy as np
from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler
import matplotlib.pyplot as plt
import pandas as pd

# Load your data
data = pd.read_csv('itsboard_mehrmalsgradlinieg_4.csv')
time = data.iloc[:, 1]
data = data.iloc[:, 2:]

# Standardize the data
scaler = StandardScaler()
x_standardized = scaler.fit_transform(data)

# Perform PCA
pca = PCA()
pca.fit(x_standardized)
pca_score = pca.transform(x_standardized)
pca_ratio = pca.explained_variance_ratio_
eigVec = pca.components_
pca_score = pca.transform(x_standardized)

print(eigVec)

# Plot
plt.bar(range(1, len(pca_ratio) + 1), pca_ratio)
plt.xlabel('Principal Component')
plt.ylabel('Erklärte Varianz')
plt.title('Erklärte Varianz Verhältnis für jeden Principal Component')
plt.show()


# Score plot
plt.scatter(time, pca_score[:, 1])
plt.xlabel('Zeit in Ticks(90MHz)')
plt.ylabel('Principal Component')
plt.title('Score Plot')
plt.show()
