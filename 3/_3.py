import re
import numpy as np

def taskA(data,grid):
    for (num,x,y,w,h) in data:
        for x_ in range(w):
            for y_ in range(h):
                grid[x_ + x][y_ + y] += 1

    sum = 0
    for x in range(len(grid)):
        for y in range(len(grid[x])):
            if grid[x][y] > 1:
                sum+=1
    return sum


def taskB(data,grid):
    unique = True
    for (num,x,y,w,h) in data:
        for x_ in range(w):
            for y_ in range(h):
                if grid[x_ + x][y_ + y] > 1:
                    unique = False
                if not unique:
                    break
            if not unique:
                break
        if unique:
            return num
        unique = True
       
                    
    



with open("input.txt") as file:
    input = file.read().splitlines()
    parsedData = []
    for line in input:
        parsedLine = tuple(int(val) for val in re.findall(r'-?\d+',line))
        parsedData.append(parsedLine)
    
    grid = np.zeros((1000,1000),dtype = int)

    print(taskA(parsedData,grid))
    print(taskB(parsedData,grid))
    

