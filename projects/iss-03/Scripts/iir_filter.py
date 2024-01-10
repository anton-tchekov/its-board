import csv
import matplotlib.pyplot as plt

# Alpha as a
a = 0.5

last_x = 0
last_y = 0
last_z = 0

sample = []
time = []
filtered_acc_x = []
filtered_acc_y = []
filtered_acc_z = []

current_row = 0
print('Sample, Acc_x, Acc_y, Acc_z')
with open('itsboard_drehungzumaufrichten_2.csv') as csv_file:
  csv_reader = csv.reader(csv_file, delimiter=',')
  for row in csv_reader:
    if current_row > 0:
      
      # Get current value with the help of the last
      x = int(row[2])
      y = int(row[3])
      z = int(row[4])
      one_minus_a = (1-a)
      x1 = a*x+(one_minus_a)*last_x
      y1 = a*y+(one_minus_a)*last_y
      z1 = a*z+(one_minus_a)*last_z
      
      # Print the values and update the last ones
      print(f'{row[0]}, {x1}, {y1}, {z1}')
      last_x = x1
      last_y = y1
      last_z = z1
      
      # Build Lists
      sample.append(row[0])
      time.append(row[1])
      filtered_acc_x.append(x1)
      filtered_acc_y.append(y1)
      filtered_acc_z.append(z1)
    current_row += 1
    
    
plt.plot(sample, filtered_acc_x, sample, filtered_acc_y, sample, filtered_acc_z)
plt.show()
# IIR Filter : aX(i)+(1-a)y(i-1) mit a = Ts / tLP + Ts
# Ts = Sample Period
# tLP filter time constant