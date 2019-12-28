import serial
import datetime
import winsound
import time

# you need pip install pyserial

# and set the port that you are using the COM
ser = serial.Serial("COM5", 19200, timeout=3)
# ser.open()
# ser.close()
datasets_path = "../datasets/"

LABELS = ["running",
          "walking",
          "jumping",
          "standing",
          "football",
          "basketball"]

duration = 50
freq = 1000


def showMenu():
    print("\n\n\n#####################    ACTIVITY SELECTION    #############################")
    print("\n")
    print("Choose the activity for data collection: \n")
    print("\t1. Running\n")
    print("\t2. Walking\n")
    print("\t3. Jumping\n")
    print("\t4. Standing\n")
    print("\t5. Testing by board data\n")
    print("\n")
    print("##################################################################################")
    print("\n")
    choice = input("\tCHOICE: ")
    return choice


def gather_samples(filename):
    if ser.is_open:
        line = ser.readline()  # read a '\n' terminated line
        line2 = line.decode(encoding='utf-8')
        if (len(line2) > 50):
            data = line2.split()
            filehandle = open(filename, mode='a')
            filehandle.write(line2)
            filehandle.close()
            print(data)
            winsound.Beep(freq, duration)


c = -1

while c < 1 or c > 6:
    c = int(showMenu())

    if c in range(1, 7):
        print("\n\nGOOD CHOICE!\n")
    else:
        print("\n\nINVALID CHOICE! REPEAT!\n")

now = datetime.datetime.now()
file_name = datasets_path + LABELS[int(c) - 1] + "_" + now.strftime("%d-%m-%Y_%H%M%S") + ".txt"
file_handle = open(file_name, mode='w')
file_handle.close()

print("\nWaiting to start data gathering...\n")

while 1:

    if (ser.is_open):
        while ser.read(size=1).decode(encoding='utf-8') != "S":
            pass

        print("Data gathering...\n")
        while 1:
            gather_samples(file_name)

    else:
        for i in range(0, 7):
            winsound.Beep(duration, freq)
            time.sleep(1)

        print("\nData gathering interrupted!\n")
        ch = input("\nDo you want to close the session? (y/n):")
        if ch == "y":
            print("\nclosing...\n")
            break
        else:
            continue
