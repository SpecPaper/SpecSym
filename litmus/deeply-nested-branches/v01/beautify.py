string1 = 'Leakage Detected'
  
# opening a text file
file1 = open("klee-out-0/messages.txt", "r")
file2 = open("leakage.txt", "w")

flag = 0
index = 0
index_bound = 2
total_num = 0

for line in file1:
    # checking string is present in line or not
    if string1 in line:
      flag = 1
      total_num += 1
    
    if flag == 1:
        index += 1
        file2.write(line)
        if index > index_bound:
            flag = 0
            index = 0

st = "\nTotal number of leakages: " + str(total_num)
file2.write(st)
# closing files
file1.close()
file2.close()
