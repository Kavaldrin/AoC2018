


def taskA(data):
    difference = ord('a') - ord('A')
    stack = []
    for c in data:
        if len(stack) == 0:
            stack.append(c)
        elif c != stack[-1] and (ord(c)+difference == ord(stack[-1]) or ord(c) == ord(stack[-1]) + difference):
            stack.pop()
        else:
            stack.append(c)

    return len(stack)
    

def taskBhelper(data,notC):
    difference = ord('a') - ord('A')
    stack = []
    for c in data:
        if difference + ord(c) != notC and ord(c) != notC:
            if len(stack) == 0:
                stack.append(c)
            elif c != stack[-1] and (ord(c)+difference == ord(stack[-1]) or ord(c) == ord(stack[-1]) + difference):
                stack.pop()
            else:
                stack.append(c)

    return len(stack)



def taskB(data):
    min = 999999999
    for letter in range(ord('a'),ord('z')+1):
        result = taskBhelper(data,letter)
        if min > result:
            min = result
    return min


with open("input.txt") as input:
    data = input.read()
    print(taskA(data))
    print(taskB(data))
