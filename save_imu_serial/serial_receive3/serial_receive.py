import serial
import csv
import keyboard

ser = serial.Serial('/dev/ttyACM2', 9600, timeout=None)
print('input file name')
file_name = input()
with open(file_name, 'a') as csvfile:
    #writer = csv.writer(csvfile, lineterminator='\n')
    csvfile.write('save_start' + "\n")
    #writer.writerow('save start')
    while True:
        line = ser.readline()
        print(line)
        try:
            if keyboard.is_pressed('s'):
                print('key pressed')
                #x = float(line.rstrip().decode(encoding='utf-8'))
                csvfile.write(line.rstrip().decode('utf-8') + "\n")
                #writer.writerow(line.rstrip().decode('utf-8'))
                pass
            else:
                pass
        except:
            pass 
    ser.close()
