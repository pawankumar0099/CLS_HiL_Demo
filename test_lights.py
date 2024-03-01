from emulator_library import Consumer,Publisher
import config
import time

publisher = Publisher(config.producer, config.topic_services)
consumer = Consumer(config.consumer)

def test_zone1():
    # person enter zone 1
    sensor_data = b'\x10'
    publisher.send(data=sensor_data)
    time.sleep(3)
    response = consumer.wait_for_data(10)

    assert response == b'\x10'
   # time.sleep(2)
    # person leave zone 1
    sensor_data = b'\x00'
    publisher.send(data=sensor_data)
    response = consumer.wait_for_data(10)
    assert response == b'\x00'
   # time.sleep(2)
    #wait for led to turn off
    #publisher.send(b'\x0A')
    response = consumer.wait_for_data(10)
    assert response == b'\x00'

def test_from_zone1_to_zone2():
    # person enter zone 1
    sensor_data = b'\x10'
    publisher.send(data=sensor_data)
    response = consumer.wait_for_data(10)
    assert response == b'\x10'

    # person leave zone 1 and enter zone 2
    sensor_data = b'\x01'
    publisher.send(data=sensor_data)
    time.sleep(3)
    response = consumer.wait_for_data(10)
    assert response == b'\x01'
    
    
    sensor_data = b'\x00'
    publisher.send(data=sensor_data)
    response = consumer.wait_for_data(10)
    assert response == b'\x00'
    
    response = consumer.wait_for_data(10)
    assert response == b'\x00'
    
    # person leave zone 2
    sensor_data = b'\x11'
    publisher.send(data=sensor_data)
    response = consumer.wait_for_data(10)
    assert response == b'\x11'

    #wait for led to turn off
    #publisher.send(b'\x0A')
    response = consumer.wait_for_data(10)
    assert response == b'\x11'

def test_Bright_cntl():
    # #Brightness control Test
    # sensor_data = b'\xFF' # 255
    # publisher.send(data=sensor_data)
    # response = consumer.wait_for_data(2)
    # assert response == b'\xFF' 

    # #Brightness control Test
    # sensor_data = b'\xAD' # 173
    # publisher.send(data=sensor_data)
    # response = consumer.wait_for_data(2)
    # assert response == b'\xAD'

    # #Brightness control Test
    # sensor_data = b'\x3F' #63
    # publisher.send(data=sensor_data)
    # response = consumer.wait_for_data(2)
    # assert response == b'\x3F'

    # #Brightness control Test
    # sensor_data = b'\x00' #0
    # publisher.send(data=sensor_data)
    # response = consumer.wait_for_data(2)
    # assert response == b'\x00'

    #Reset add if required
    #Brightness control Test LED1
    sensor_data = b'\x10' 
    publisher.send(data=sensor_data)
    time.sleep(3)
    response = consumer.wait_for_data(5)
    assert response == b'\x10'
    
    #Brightness control Test
    sensor_data = b'\x00' 
    publisher.send(data=sensor_data)
    response = consumer.wait_for_data(5)
    assert response == b'\x00'

    response = consumer.wait_for_data(10)
    assert response == b'\x00'

    #Brightness control Test LED2
    sensor_data = b'\x01' 
    publisher.send(data=sensor_data)
    time.sleep(3)
    response = consumer.wait_for_data(5)
    assert response == b'\x01'

    #Brightness control Test
    sensor_data = b'\x11'
    publisher.send(data=sensor_data)
    response = consumer.wait_for_data(5)
    assert response == b'\x11'

def test_temp():
    #LED will be driven on the basis of Temperature -30, -20, 0, 40, 60 degree celcius, and LDR will report the exact brightness


    #Temperature 25 degree, Expected output Led Dimmmed at 25% Brightness
    sensor_data = b'\x19' #25 degree Temp
    publisher.send(data=sensor_data)
    response = consumer.wait_for_data(2)
    assert response == b'\x19' #record the response on the basis of LED output, Expected output LED should not Turn on or at AnalogWrite(255)

    #Temperature 60 degree, Expected output Led Dimmmed at 25% Brightness and should be decreasing over the time period
    sensor_data = b'\x3C' #60 degree Temp
    publisher.send(data=sensor_data)
    response = consumer.wait_for_data(2)
    assert response == b'\x3C' #record the response on the basis of LED output, Expected output LED should not Turn on or at AnalogWrite(173)

    #Temperature -20 degree, Expected output Led Dimmmed at 75% Brightness
    sensor_data = b'\x14' #-20 degree celicus
    publisher.send(data=sensor_data)
    response = consumer.wait_for_data(2)
    assert response == b'\x14' #record the response on the basis of LED output, Expected output LED should not Turn on at AnalogWrite(63)

    #Temperature -30 degree, Expected output Led High at full 100% Brightness
    sensor_data = b'\x1E' #-30 degree Temp
    publisher.send(data=sensor_data)
    response = consumer.wait_for_data(2)
    assert response == b'\x1E' #record the response on the basis of LED output, Expected output LED should not Turn on or at AnalogWrite(0)

