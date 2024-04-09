#! /usr/bin/env python3

import random
import numpy as np
import matplotlib.pyplot as plt

asize = 1024
psize = 16*1024
validaddresses = np.zeros(asize)


for i in range(10000):
    random.seed(i)
    for limit in range(asize):
        virtualAddress = int(asize * random.random())
        if (virtualAddress < limit):
            validaddresses[limit] += 1

figure = plt.figure()
x = np.array(range(asize))
plt.plot(x, validaddresses/10000)
plt.xlabel('Limit')
plt.ylabel('Valid fraction (Average)')
plt.show()