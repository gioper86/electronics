from DHT import readDht11
import LCD1602
import time
from datetime import datetime
import csv 

measurements = []
MAX_ATTEMPETS = 300
MAX_MEASUREMENTS = 5

def update_lcd(humidity, temperature):
    LCD1602.init(0x27, 1)
    LCD1602.write(0, 0, 'Temp: ' + str(temperature) + 'C')
    LCD1602.write(1, 1, 'Hum: ' + str(humidity) + '%')
    time.sleep(30)
    LCD1602.closelight()

def write_to_file(humidity, temperature):
    ts = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    fields=[ts,temperature,humidity]
    with open(r'/home/pi/data/temp_hum.csv', 'a') as f:
        writer = csv.writer(f)
        writer.writerow(fields)

def collect_measurements():
    attempts = 0
    while attempts < MAX_ATTEMPETS and len(measurements)<MAX_MEASUREMENTS:
        result = readDht11()
        humidity = None
        temperature = None
        if result:
            humidity, temperature = result
            measurements.append((humidity, temperature))
            print("Measurement collected")
        else:
            print("Bad data")
        attempts = attempts+1
        time.sleep(1)
    
    print("MEASUREMENTS " + str(measurements))

def main():
    collect_measurements()
    humidity, temperature = measurements[len(measurements)-1]
    write_to_file(humidity, temperature)    
    update_lcd(humidity, temperature)

if __name__ == '__main__':
    main()