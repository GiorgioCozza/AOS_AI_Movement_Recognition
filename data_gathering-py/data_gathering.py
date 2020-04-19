import serial
import datetime
import time
import os
import numpy as np

# you need pip install pyserial

# and set the port that you are using the COM
ser = serial.Serial("/dev/ttyACM0", 19200)
# ser.open()
# ser.close()
datasets_path = "/home/pi/Desktop/"

LABELS = ["running",
          "walking",
          "jumping",
          "standing",
          "sitting",
          "sit_ups",
          "supine",
          "lying_on_side"]

duration = 50
freq = 1000


######################    ACTIVITY SELECTION    #############################

#   Choose the activity for data collection: 

#      1. Running
#      2. Walking
#      3. Jumping
#      4. Standing
#      5. Sitting
#      6. Sit_ups
#      7. Supine
#      8. Lying on Side
#      9. Testing

##################################################################################

# Insert here the code of the activity that you want to collect 
# or if you want to test the network (code: 9)

c = 9


def gather_samples(filename):
    if ser.isOpen():
        line = ser.readline()  # read a '\n' terminated line
        line2 = line.decode(encoding='utf-8')
        data = line2.split()
        filehandle = open(filename, mode='a')
        filehandle.write(line2)
        filehandle.close()


if c != 9:
    f_cnt = 1
    file_dir = os.path.join(datasets_path, LABELS[int(c) - 1])
    if not os.path.exists(file_dir):
        os.makedirs(file_dir)
    now = datetime.datetime.now()
    if len(os.listdir(file_dir)) > 0:
        nur = map(int, [os.listdir(file_dir)[i].split('.')[0].split('_')[-1] for i in range(0, len(os.listdir(file_dir)))])
        f_cnt = int(np.max(list(nur))) + 1

    file_name = os.path.join(file_dir, LABELS[int(c) - 1] + "_" + now.strftime("%d-%m-%Y_%H%M%S") + '_new_' + str(f_cnt) +".txt")
    file_handle = open(file_name, mode='w')
    file_handle.close()

    print("\nWaiting to start data gathering...\n")

while 1:

    if (ser.isOpen()):
        while ser.read(size=1).decode(encoding='utf-8') != "S":
            pass
        
        if c == 9:
            print("Testing...\n")
            while 1:
                if ser.isOpen():
                    line = ser.readline()  # read a '\n' terminated line
                    line2 = line.decode(encoding='utf-8')
                    print(line2)
        else:
            print("Data gathering...\n")
            while 1:
                gather_samples(file_name)

    else:

        print("\nData gathering interrupted!\n")
        ch = input("\nDo you want to close the session? (y/n):")
        if ch == "y":
            print("\nclosing...\n")
            break
        else:
            continue
