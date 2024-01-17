
sensor2_angle = np.radians(0)
sensor2_translation = np.array((0, 0))
c = np.cos(sensor2_angle)
s = np.sin(sensor2_angle)
sensor2_rotation = np.array(((c, -s), (s, c)))

print("Sensor 2 Translation:")
print(sensor2_translation)
print("Sensor 2 Rotation:")
print(sensor2_rotation)



def calculate_kn_distance(X,k):
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

Then, once you have defined your function, you can choose a k value and plot the histogram to find a knee to define an appropriate epsilon value.

eps_dist = calculate_kn_distance(X[1],4)
plt.hist(eps_dist,bins=30)
plt.ylabel('n');
plt.xlabel('Epsilon distance');
