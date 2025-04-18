const mqtt = require('mqtt');
const mysql = require('mysql');

// Kết nối đến MQTT Broker
const mqttClient = mqtt.connect('mqtt://172.20.10.14:1006', {
    username: 'huy',
    password: '123'
});

// Kết nối đến MySQL
const db = mysql.createConnection({
    host: 'localhost',
    user: 'root',
    password: '1234',
    database: 'esp8266_data'
});

// Kiểm tra kết nối MySQL
db.connect((err) => {
    if (err) {
        console.log('Không thể kết nối MySQL: ', err);
        return;
    }
    console.log('Đã kết nối MySQL');
});

// Lắng nghe tin nhắn từ MQTT
mqttClient.on('connect', () => {
    console.log('Đã kết nối MQTT');
    mqttClient.subscribe('outTopic', (err) => {
        if (err) {
            console.log('Lỗi khi subscribe outTopic: ', err);
        }
    });

    mqttClient.subscribe('inTopic', (err) => {
        if (err) {
            console.log('Lỗi khi subscribe inTopic: ', err);
        }
    });
});

// Hàm xử lý tin nhắn MQTT và cập nhật vào MySQL
mqttClient.on('message', (topic, message) => {
    const data = message.toString();
    console.log('Dữ liệu nhận được:', data);

    if (topic === 'outTopic') {
        const regex = /Time: ([\d\- :]+), Temp: ([\d\.]+) C, Hum: (\d+)%?, Light: (\d+) Lux, Wind: (\d+) km\/h, Soil Moisture: (\d+)%?/;
        const match = data.match(regex);

        if (match) {
            const time = match[1];
            const temp = parseFloat(match[2]);
            const humidity = parseInt(match[3]);
            const light = parseInt(match[4]);
            const wind = parseInt(match[5]);
            const soilMoisture = parseInt(match[6]);

            console.log(`Thời gian: ${time}, Nhiệt độ: ${temp}, Độ ẩm: ${humidity}, Ánh sáng: ${light}, Tốc độ gió: ${wind}, Soil Moisture: ${soilMoisture}`);

            if (
                temp > 0 && temp <= 50 && 
                humidity > 0 && humidity <= 100 && 
                light > 100 && light <= 1000 && 
                wind >= 0 && wind <= 200 &&
                soilMoisture >= 0 && soilMoisture <= 100
            ) {
                const query = 'INSERT INTO sensor_data (Temp, Hum, Light, Wind, SoilMoisture, Time) VALUES (?, ?, ?, ?, ?, ?)';
                db.query(query, [temp, humidity, light, wind, soilMoisture, time], (err, result) => {
                    if (err) {
                        console.log('Lỗi khi chèn dữ liệu vào sensor_data:', err);
                    } else {
                        console.log('Đã chèn dữ liệu vào sensor_data thành công:', result);
                    }
                });
            }
        } else {
            console.log('Dữ liệu không khớp với định dạng mong đợi.');
        }
    }

    // Xử lý tin nhắn từ inTopic để cập nhật trạng thái quạt
    if (topic === 'inTopic') {
        const fanRegex = /Fan: (\d+)/;
        const fanMatch = data.match(fanRegex);

        if (fanMatch) {
            const fanStatus = parseInt(fanMatch[1]);

            console.log(`Trạng thái quạt: ${fanStatus}`);

            if (fanStatus === 0 || fanStatus === 1) {
                const query = 'UPDATE devices SET Fan = ? WHERE id = 1';
                db.query(query, [fanStatus], (err, result) => {
                    if (err) {
                        console.log('Lỗi khi cập nhật trạng thái quạt:', err);
                    } else {
                        console.log('Đã cập nhật trạng thái quạt thành công:', result);
                    }
                });
            }
        } else {
            console.log('Dữ liệu Fan không đúng định dạng.');
        }
    }
});
