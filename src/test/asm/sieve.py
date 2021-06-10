#!/usr/local/bin/python 
# $Id: sieve.python,v 1.10 2001/05/06 04:37:45 doug Exp $
# http://www.bagley.org/~doug/shootout/
# with help from Brad Knotwell

import sys
from time import time

def main(NUM):
	for foo in range(NUM):
		flags = (8192+1) * [1]
		count = 0
		for i in range(2,8192+1):
			if flags[i]:
				# remove all multiples of prime: i
				k = i + i
				while k <= 8192:
					flags[k] = 0
					k = k + i
				count = count + 1    
	print("Count:", count)

t0=time()
main(5000)
print(time()-t0)
