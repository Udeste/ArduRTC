import datetime;
import smbus
import time

I2C_SET_TIMESTAMP_CMD = 0x0A
I2C_SET_TRIGGER_CMD   = 0x0B
I2C_GET_TIMESTAMP_CMD = 0x0C
I2C_GET_TRIGGER_CMD   = 0x0D
I2C_ADDRESS           = 0x8  # address of the arduino I2C

current_time = datetime.datetime.now()
trigger_time = current_time.replace(hour=10, minute=00, second=00, microsecond=00)

if trigger_time < current_time:
    trigger_time += datetime.timedelta(days=1)

current_timestamp = int(current_time.timestamp())
trigger_timestamp = int(trigger_time.timestamp())

i2c_set_timestamp_bytes = list(current_timestamp.to_bytes(4, 'little'))
i2c_set_trigger_bytes = list(trigger_timestamp.to_bytes(4, 'little'))

print("current_time:___________________", current_time)
print("trigger_time:___________________", trigger_time)
print("current_timestamp:______________", current_timestamp)
print("trigger_timestamp:______________", trigger_timestamp)
print("i2c_set_timestamp_bytes:________", i2c_set_timestamp_bytes)
print("i2c_set_trigger_bytes:__________", i2c_set_trigger_bytes)

i2c = smbus.SMBus(1)

i2c.write_i2c_block_data(I2C_ADDRESS, I2C_SET_TIMESTAMP_CMD, i2c_set_timestamp_bytes)
time.sleep(0.5)
i2c.write_i2c_block_data(I2C_ADDRESS, I2C_SET_TRIGGER_CMD, i2c_set_trigger_bytes)

print('i2c_get_timestamp_bytes:________', i2c.read_i2c_block_data(I2C_ADDRESS, I2C_GET_TIMESTAMP_CMD, 4))
time.sleep(0.5)
print('i2c_get_trigger_bytes:__________', i2c.read_i2c_block_data(I2C_ADDRESS, I2C_GET_TRIGGER_CMD, 4))