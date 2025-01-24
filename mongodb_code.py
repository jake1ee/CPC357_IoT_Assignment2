import pymongo
import json
import paho.mqtt.client as mqtt
from datetime import datetime, timezone

# MongoDB configuration
mongo_client = pymongo.MongoClient("mongodb://localhost:27017/")
db = mongo_client["assignment2"]
collection = db["sensor_data"]

# MQTT configuration
mqtt_broker_address = "34.60.49.123"
mqtt_topic = "sensor_data"

# Define the callback function for connection
def on_connect(client, userdata, flags, reason_code, properties):
    if reason_code == 0:
        print("Successfully connected")
    client.subscribe(mqtt_topic)

# Define the callback function for ingesting data into MongoDB
def on_message(client, userdata, message):
    payload = message.payload.decode("utf-8")
    print(f"Received message: {payload}")

    # Convert MQTT payload to JSON
    try:
        sensor_data = json.loads(payload)
        # Extract individual fields from the JSON payload
        soil_moisture = sensor_data.get("soil_moisture")
        water_level = sensor_data.get("water_level")
        rain_level = sensor_data.get("rain_level")
        relay_status = sensor_data.get("relay_status")
    except json.JSONDecodeError:
        print("Error decoding JSON")
        return

    # Add a timestamp and insert into MongoDB
    timestamp = datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%S.%fZ")
    document = {
        "timestamp": timestamp,
        "soil_moisture": soil_moisture,
        "water_level": water_level,
        "rain_level": rain_level,
        "relay_status": relay_status,
    }
    collection.insert_one(document)
    print(f"Data ingested into MongoDB: {document}")

# Create a MQTT client instance
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)

# Attach the callbacks using explicit methods
client.on_connect = on_connect
client.on_message = on_message

# Connect to MQTT broker
client.connect(mqtt_broker_address, 1883, 60)

# Start the MQTT loop
client.loop_forever()
