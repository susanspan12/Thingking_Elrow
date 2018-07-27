import serial

opt_1 = b'1'
opt_2 = b'2'
opt_3 = b'3'
opt_4 = b'4'
opt_5 = b'5'

BAUD_RATE = 9600
TEENSY_COM = 'COM7'
ser = None

def serial_init():
	try:
		global ser
		ser = serial.Serial(TEENSY_COM, BAUD_RATE, timeout=1)
	except Exception:
		raise IOError('Could not connect to Teensy')

def send_serial(opt):
	ser.write(opt)
	print('Write option: ' + opt)
	try:
		x = ser.read()
		print('read: ' + x)
	except Exception:
		raise IOError('Could not read from Teensy')

serial_init()
send_serial(opt_4)
