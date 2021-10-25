f = open("TotalResult", 'r')
a = {}
cnt = 0

while True:
    line = f.readline()
    line = line.strip()
    if not line: break
    if not a.has_key(line):
        print(line)
        a[line] = 1
    else:
        a[line] += 1
    cnt += 1

print("The number of edge-pair test requirements covered at least once by all test cases:" +str(len(a)))

f.close()
