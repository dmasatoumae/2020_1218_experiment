import serial
import csv
import keyboard

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=None)
with open('save.csv', 'w') as csvfile:
    writer = csv.writer(csvfile, lineterminator='\n')

    while True:
        line = ser.readline()
        print(line)
        try:
            if keyboard.is_pressed('s'):
                print('key pressed')
                #x = float(line.rstrip().decode(encoding='utf-8'))
                writer.writerow(line.rstrip().decode('utf-8'))
                pass
            else:
                pass
        except:
            pass 
    ser.close()
