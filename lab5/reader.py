import serial
s =serial.Serial('/dev/ttyO4')
s.baudrate = 115200
s.timeout = 0
#file = open("output.txt", "w")
while True:
	print()

