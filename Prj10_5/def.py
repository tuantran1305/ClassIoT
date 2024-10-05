import psutil
import platform
import time
# Lấy thông tin hệ điều hành
def get_system_info():
    name = platform.uname()
    system_info={
            'name':name.node,
            'release':name.release,
            'Version':name.version,
            'Machine':name.machine,
        }
    return system_info



# In thông tin
if __name__ == "__main__":
    try:    
        while True:
            print("System Information:")
            for key, value in get_system_info().items():
                print(f"{key}: {value}")
            time.sleep(10)
    except KeyboardInterrupt:
        print("loi")
