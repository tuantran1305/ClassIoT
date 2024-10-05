import platform
import time
import threading
millis_previous = 0
def read_information():
    while True:
        print('Platform processor:', platform.processor()) 
        print('Platform architecture:', platform.architecture()) 
        print('Machine type:', platform.machine())
        time.sleep(10)
    
def print_name():
    i = 0
    while True:
        print(f"Quang Hieu: {i}")
        i += 1
        time.sleep(1)
    
thread1 = threading.Thread(target=read_information)
thread2 = threading.Thread(target=print_name)

thread1.start()
thread2.start()
