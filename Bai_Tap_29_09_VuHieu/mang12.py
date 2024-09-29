import math
def getMuls(A, input_array):
    n = len(input_array)
    if n <= 1:
        return []
    mang = []
    for i in range(n):
        a = 1
        for j in range(n):
            if i != j:
                a *= input_array[j]
        mang.append(a)
    return mang
def so_chan(arr):
    A = []
    for i in arr:
        if i % 2 == 0:
         A.append(i)
    return A 
def kiem_tra(arr):
    A=[]
    num_max  = 0
    for num in arr:
        if num <= 0:
            check = False
        else:
            check = True
        for i in range (2 , int(math.sqrt(num)+1)):
            if i % i == 0:
                check = False
                break
        if check:
            A.append(num)
            if num > num_max :
                num_max = num  
    return num_max, A 
while True:
    try:
        b = int(input("Nhap so phan tu trong mang: "))
        A = []
        for i in range(b): 
            c = int(input(f"Nhap so {i + 1}: "))
            A.append(c)
        print(A)
    except ValueError:
        print("Nhap so khong hop le",ValueError)
    print(f"{getMuls(A, A)}")
    so_Chan = so_chan(A)
    print(f"Mang so chan la {so_Chan}")
    So_nguyen_to = kiem_tra(A)
    print(f"Mang so nguyen to {So_nguyen_to[1]}")
    print(f"So lon nhat trong mang {So_nguyen_to[0]}")

