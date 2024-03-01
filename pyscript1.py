import serial
import threading
from kafka import KafkaConsumer, KafkaProducer

bootstrap_servers = 'localhost:9092'
topic_name = 'consumer'

# Define serial port and baud rate
serial_port = '/dev/ttyACM2'  # Adjust to your specific port
baud_rate = 9600

# Initialize the serial connection
ser = serial.Serial(serial_port, baud_rate, timeout=1)

# Kafka Consumer setup
consumer = KafkaConsumer('producer', bootstrap_servers=bootstrap_servers)

# Kafka Producer setup
producer = KafkaProducer(bootstrap_servers=bootstrap_servers)

def read_serial():
    while True:
        # Read a byte from the serial port
        ser.flushInput()
        received_byte = ser.read()

        if received_byte:
            # Print the received byte
            print("Received Byte:", received_byte)

            # Send the received byte to Kafka
            producer.send(topic_name, received_byte)
            

# Create a thread for reading serial data
serial_thread = threading.Thread(target=read_serial)

try:
    # Start the serial thread
    serial_thread.start()

    # Main program continues to run independently
    for message in consumer:
        # Process the received message from Kafka
        data = message.value

        if data:
            # Write the received data to the serial port
            ser.write(data)
            #print("data: ",data)
except KeyboardInterrupt:
    # Stop the serial thread when the program is interrupted
    serial_thread.join()
    ser.close()
    print("Program terminated.")
