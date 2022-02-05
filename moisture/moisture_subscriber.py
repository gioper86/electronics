import paho.mqtt.client as mqtt
import time
import datetime

def on_message(client, userdata, message):
  msg = str(message.payload.decode("utf-8"))
  try:
    value = msg
    dt_now = x = datetime.datetime.now()
    dt_formatted = dt_now.strftime("%Y-%m-%dT%H:%M:%S%z")
    str_values = str(dt_formatted)+','+msg
    with open('/home/pi/data/moisture/moisture.csv','a') as fd:
      fd.write(str_values+'\n')

    with open('/home/pi/data/moisture/log.txt','a') as fd:
      fd.write("INFO:{}:Added new row to CSV:{} \n".format(dt_formatted, msg))

  except:
     with open('/home/pi/data/moisture/log.txt','a') as fd:
      fd.write("ERROR:{}:Error:{} \n".format(dt_formatted, msg))   

broker_address="localhost"

client = mqtt.Client("moisture_subscriber")
client.on_message=on_message
client.connect(broker_address)
# client.loop_start()

client.subscribe("moisture")
client.loop_forever()