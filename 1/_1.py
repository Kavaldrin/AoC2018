

data = []
with open("input.txt") as file:
    data_ = file.read()
    data_ = data_.split("\n")
    data = [int(x) for x in data_]

def taskA(data):
    return sum(data)

def taskB(data):
    freqs = set()
    freqs.add(0)
    prevFreq = 0
    
    while True:
        for num in data:
            newFreq = prevFreq + num
            
            if newFreq in freqs:
                return newFreq
            freqs.add(newFreq)
            prevFreq = newFreq


print(taskA(data)," ",taskB(data))

