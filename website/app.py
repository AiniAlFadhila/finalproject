from flask import Flask, request, jsonify, render_template
from flask_cors import CORS
from flask_socketio import SocketIO

app = Flask(__name__)
CORS(app)
socket = SocketIO(app)

# Simpan data sensor di memori
sensor_data = {
    'temperature': None,
    'humidity': None
}

@app.route('/')
def index():
    return render_template('index.html', temperature=sensor_data['temperature'], humidity=sensor_data['humidity'])

@app.route('/api/data', methods=['POST'])
def api_data():
    global sensor_data
    data = request.get_json()
    sensor_data['temperature'] = data.get('temperature')
    sensor_data['humidity'] = data.get('humidity')
    socket.emit("update", sensor_data)
    return jsonify(success=True)

if __name__ == '__main__':
    socket.run(app, host="0.0.0.0", debug=True)
