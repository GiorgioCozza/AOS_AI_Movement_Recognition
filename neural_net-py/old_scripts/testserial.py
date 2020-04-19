import serial
from datetime import date

# you need pip install pyserial

# and set the port that you are using the COM
ser = serial.Serial("COM5", 19200, timeout=3)
# ser.open()
# ser.close()


while 1:
    if (ser.is_open):
        line = ser.readline()  # read a '\n' terminated line
        line2 = line.decode(encoding='utf-8')
        if (len(line2) > 50):
            data = line2.split()
            file_handle = open("data.txt", mode='a')
            file_handle.write(line2)
            file_handle.close()
            print(data)
        else:
            continue
    else:
        print("closed")
        break
