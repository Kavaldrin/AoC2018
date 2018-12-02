from collections import Counter

data = []
with open("input.txt") as file:
    data = file.read().split("\n")


def taskA(data):
    twos = 0
    threes = 0
    for line in data:
        counter = [j for i,j in Counter(line).most_common()]
        if 3 in counter:
            twos+=1
        if 2 in counter:
            threes+=1
    return twos*threes


def taskB(data):
    for i in range(len(data)):
        for j in range(len(data) - i):
            count = 0
            for k in range(len(data[0])):
                if data[i][k] != data[i+j][k]:
                    count += 1

            if count == 1:
                s = str()
                for k in range(len(data[0])):
                    if data[i][k] == data[i+j][k]:
                        s += data[i][k]
                return s


print(taskA(data))
print(taskB(data))