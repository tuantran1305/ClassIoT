# print("hello")
# while True:
#   try:
    #   a = int(input("Nhập số thứ 1: "))
    #   print("Bạn đã nhập số:", a)
    #   b = int(input("Nhập số thứ 2: "))
    #   print("Bạn đã nhập số:", b)
    #   print("Tổng 2 số trên là:", a+b)
#   except ValueError:
#       print("Vui lòng nhập số hợp lệ",ValueError)
# for i in range(0, 5, 1):
#     try:
#         a = int(input("Nhập một số: "))
#         if (a % 2 == 0):
#             print("Số đã nhập chia hết cho 2")
#         else:
#             print("Số đã nhập không chia hết cho 2")
#     except ValueError:
#         print("Vui lòng nhập đúng định dạng", ValueError)
# import math
# while True:
#     try:
#         check = False
#         a = int(input("Nhập một số: "))
#         if (a<=1):
#             print(f"Số {a} không phải số nguyên tố")
#         else:    
#             for i in range(2,int(math.sqrt(a))+1,1):
#                 if (a % i == 0 ):
#                     check = True
#                     print(f"Số {a} không phải số nguyên tố")
#                     break
#             if (check == False): 
#                 print(f"Số {a} là số nguyên tố")
#     except ValueError:
#         print("Vui lòng nhập đúng định dạng")
import math
def max_list(array):
    max_value = array[0]
    for i in array:
        if i > max_value:
            max_value = i
    return max_value
def min_list(array):
    min_value = array[0]
    for i in array:
        if i < min_value:
            min_value = i
    return min_value
def even_list(array, even_array):
    k = 0
    for i in array:
        if (i % 2 == 0):
            if i not in even_array[:k]:
                even_array[k] = i
                k = k + 1
    return even_array[:k]
def prime_list(array, prime_array):
    k = 0
    check = False
    for i in array:
        if (i > 2):
            for j in range(2,int(math.sqrt(i))+1,1):
                if (i % j == 0 ):
                    check = True
                    break
                else:
                    check = False
            if (check == False):
                if i not in prime_array[:k]: 
                    prime_array[k] = i
                    k = k + 1
        elif (i == 2):
            if i not in prime_array[:k]:
                prime_array[k] = i
                k = k + 1    
    return prime_array[:k]
def getMuls(array, input_array):
    result = 1
    k = 0
    for i in input_array:
        if (i != 0): 
            result = result * i
        k = k + 1
    if (k<=1):
        array = []
    else:
        for j in range(0,k,1):
            if (input_array[j] == 0):
                array[j] = result
            else:
                array[j] = int(result/input_array[j])
    return array   
while True:
    try:
        n = int(input("Số phần tử là: "))
        numbers = [0]*n
        even_numbers = [0]*n
        prime_numbers = [0]*n
        muls_numbers = [0]*n
        for i in range(0,n,1):
            numbers[i] = int(input(f"Nhập số thứ {i+1}:"))
        print(f"Mảng số chẵn là: {even_list(numbers, even_numbers)}")
        print(f"Mảng số nguyên tố là: {prime_list(numbers, prime_numbers)}")
        print(f"Số nguyên tố lớn nhất là: {max_list(prime_list(numbers, prime_numbers))}")
        print(f"Kết quả là: {getMuls(muls_numbers, numbers)}")
    except ValueError:
        print("Vui lòng nhập đúng định dạng")   
