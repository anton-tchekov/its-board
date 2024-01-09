import csv

def replaceMissingEntry(list, item, row):
  # if were not the first or last row
  if row > 0 and row < len(list):
    
    # if the last and next row are both available, get the average between them
    if list[row-1][item] and list[row+1][item]:
      list[row][item] = str(int((int(list[row-1][item]) + int(list[row+1][item])) / 2))
      
    # if the last row isnt empty use that as new value
    elif list[row-1][item]:
      list[row][item] = list[row-1][item]
      
    # if the next row isnt empty use that as new value
    elif list[row+1][item]:
      list[row][item] = list[row+1][item]
      
    # if theres no data around the missing entry drop it
    else:
      list.remove(row)
      
  # if on first or last row
  elif row == 0 and list[row+1][item]:
    list[row][item] = list[row+1][item]
  elif row == len(list) and list[row-1][item]:
    list[row][item] = list[row-1][item]
  else:
    list.remove(row)
      
  return 0

# 'bewegung2_fehlerhaft.csv' 
# 'itsboard_drehungzumaufrichten_2.csv'

# Read CSV into a List
with open('bewegung2_fehlerhaft.csv' ) as csv_file:
  csv_reader = csv.reader(csv_file, delimiter=',')
  row_list = []
  for row in csv_reader:
    row_list.append(row)
    
currentrow = 0
currentitem = 0
    
# Go through every row and fix missing entries
for row in row_list:
  currentitem = 0
  for item in row:
    if not item:
      replaceMissingEntry(row_list, currentitem, currentrow)
      print(f'Missing Item at row: {currentrow} and item: {currentitem} replaced with {row_list[currentrow][currentitem]}')
    currentitem += 1
  currentrow += 1
  
# Print fixed csv
with open('fixed.csv', 'x') as csvfile:
    writer = csv.writer(csvfile, delimiter=',')
    for row in row_list:
      writer.writerow(row)
     
  
  #row[currentitem] = row_list[currentrow+1][currentitem]