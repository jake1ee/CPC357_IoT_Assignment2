from flask import Flask, render_template, jsonify
from pymongo import MongoClient

app = Flask(__name__)

# MongoDB connection
client = MongoClient("mongodb://localhost:27017/")
db = client.assignment2
collection = db.sensor_data

@app.route('/')
def index():
    return render_template('chart.html')

@app.route('/data')
def data():
    # Fetch data from MongoDB
    sensor_data = collection.find()
    data_list = []
    for data in sensor_data:
        data_dict = {
            "timestamp": data.get("timestamp"),
            "soil_moisture": data.get("soil_moisture"),
            "water_level": data.get("water_level"),
            "rain_level": data.get("rain_level"),
            "relay_status": data.get("relay_status")
        }
        data_list.append(data_dict)
    return jsonify(data_list)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
