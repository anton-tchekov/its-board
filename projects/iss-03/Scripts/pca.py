import numpy as np
from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler
import matplotlib.pyplot as plt
import pandas as pd

# Load your data
data = pd.read_csv('itsboard_bewegunggradlinieg_3.csv')
data = data.iloc[:, 2:]

# Convert data to numpy array
x = data


scaler = StandardScaler()
x_standardized = scaler.fit_transform(x)

# Scikit
# Dim festlegen: pca = PCA(n_components=3)
# sonst besser so:
pca = PCA()
pca.fit(x_standardized)
pca_ratio = pca.explained_variance_ratio_
eigVec = pca.components_
pca_score = pca.transform(x)

# Plot
plt.bar(range(1, len(pca_ratio) + 1), pca_ratio)
plt.xlabel('Principal Component')
plt.ylabel('Explained Variance Ratio')
plt.title('Explained Variance Ratio for Each Principal Component')
plt.show()
