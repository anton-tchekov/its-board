import matplotlib.pyplot as plt
import numpy as np
import csv

Qtime = np.array(())
QaX = np.array(())
QaY = np.array(())
QaZ = np.array(())

QgX = np.array(())
QgY = np.array(())
QgZ = np.array(())

Qa2X = np.array(())
Qa2Y = np.array(())
Qa2Z = np.array(())

QmX = np.array(())
QmY = np.array(())
QmZ = np.array(())

with open('imu.txt', newline='') as csvfile:
	dreader = csv.reader(csvfile, delimiter=',', quotechar='|')
	for row in dreader:
		time = int(row[0])

		aX = int(row[1])
		aY = int(row[2])
		aZ = int(row[3])

		gX = int(row[4])
		gY = int(row[5])
		gZ = int(row[6])

		a2X = int(row[7])
		a2Y = int(row[8])
		a2Z = int(row[9])

		mX = int(row[10])
		mY = int(row[11])
		mZ = int(row[12])

		Qtime = np.append(Qtime, time)

		QaX = np.append(QaX, aX)
		QaY = np.append(QaY, aY)
		QaZ = np.append(QaZ, aZ)

		QgX = np.append(QgX, gX)
		QgY = np.append(QgY, gY)
		QgZ = np.append(QgZ, gZ)

		Qa2X = np.append(Qa2X, a2X)
		Qa2Y = np.append(Qa2Y, a2Y)
		Qa2Z = np.append(Qa2Z, a2Z)

		QmX = np.append(QmX, mX)
		QmY = np.append(QmY, mY)
		QmZ = np.append(QmZ, mZ)


fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2)
ax1.grid(True)
ax2.grid(True)
ax3.grid(True)
ax4.grid(True)
ax1.plot(Qtime, QaX, label="aX")
ax1.plot(Qtime, QaY, label="aY")
ax1.plot(Qtime, QaZ, label="aZ")
ax1.legend(loc="upper left")

ax2.plot(Qtime, QgX, label="gX")
ax2.plot(Qtime, QgY, label="gY")
ax2.plot(Qtime, QgZ, label="gZ")
ax2.legend(loc="upper left")

ax3.plot(Qtime, Qa2X, label="a2X")
ax3.plot(Qtime, Qa2Y, label="a2Y")
ax3.plot(Qtime, Qa2Z, label="a2Z")
ax3.legend(loc="upper left")

ax4.plot(Qtime, QmX, label="mX")
ax4.plot(Qtime, QmY, label="mY")
ax4.plot(Qtime, QmZ, label="mZ")
ax4.legend(loc="upper left")
plt.show()

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(Qtime, QgX, QgY)
plt.show()

