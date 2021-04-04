import paho.mqtt.client as mqtt
import time
import datetime

def on_message(client, userdata, message):
  msg = str(message.payload.decode("utf-8"))
  try:
    values = msg.split(",")
    dt_now = x = datetime.datetime.now()
    dt_formatted = dt_now.strftime("%Y-%m-%dT%H:%M:%S%z")
    values.insert(0,dt_formatted)
    str_values = ','.join(values)
    with open('/home/pi/data/mobile_weather/weather.csv','a') as fd:
      fd.write(str_values+'\n')

    with open('/home/pi/data/mobile_weather/log.txt','a') as fd:
      fd.write("INFO:{}:Added new row to CSV:{} \n".format(dt_formatted, msg))

  except:
     with open('/home/pi/data/mobile_weather/log.txt','a') as fd:
      fd.write("ERROR:{}:Error:{} \n".format(dt_formatted, msg))   

broker_address="localhost"

client = mqtt.Client("weather_subscriber")
client.on_message=on_message
client.connect(broker_address)
# client.loop_start()

client.subscribe("mobileTemp")
client.loop_forever()