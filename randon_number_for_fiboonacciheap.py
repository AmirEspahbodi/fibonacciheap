from random import Random
ran = Random()
with open('fibonacciheapinit.txt', 'r+') as f:
    for i in range(0, 10000):
        num = ran.randint(-5000000, 5000000)
        f.write(str(num)+" ")