import math 
import random

def parent(i):
    return math.floor(i/2)
def left(i):
    return i*2
def right(i):
    return i*2+1

def heapify(A, i):
    l = left(i)
    r = right(i)
    i = i 

    length = heapLength(A)
    largest = i 
    if r<=length:
        if A[r]>A[largest]:
            largest=r
    if l<=length:
        if A[l]>A[largest]:
            largest=l
    
    if largest!=i:
        temp = A[i]
        A[i]=A[largest]
        A[largest]=temp
        return heapify(A,largest)
    return A

def heapCheck(A):
    length = heapLength(A)
    for i in range(1,length):
        l = left(i)
        r = right(i)
        if l<=length:
            if A[i]<A[left(i)]:
                print "not a heap: " + str(A[i])+ "<"+ str(A[left(i)])
                return
        if r<=length:
            if A[i]<A[right(i)]:
                print "not a heap: " + str(A[i])+ "<"+ str(A[right(i)])
                return
    print "heap!"
    return

def heapLength( A ): return len( A ) - 1
def BuildHeap( A ): 
    n = heapLength( A )
    for i in range( int(math.floor(n/2)) ,0 ,-1 ): #range does not include 0
        heapify( A, i)
    return A
mylist = []
mylist.append(0)
mylist = mylist + random.sample(range(1, 100), 30)
heap = BuildHeap(mylist)
print heap[1:]
heapCheck(heap)
