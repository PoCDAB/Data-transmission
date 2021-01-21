import os
import sys
import subprocess
import codecs
from time import sleep
import array

sleeptime = 1 #sleeptime between transmissions (in seconds)

def createChecksum(string): #creates checksum so the receive side can determine if all data was received correctly
	string = string.encode()
	bytes = array.array('b', string)
	checksum = 0
	for i in range(len(bytes)):
		checksum ^= bytes[i]
	checksum = hex(checksum)
	return checksum[2:]

filepath = sys.argv[1]	#get filepath from arguments
filename = filepath.rsplit('/', 1)[-1]
#create data format as [filename]|[filedata][checksum]|
data = filename + '|'
with codecs.open(filepath, 'r', encoding='utf-8', errors='ignore') as file:
	data += file.read()
data += createChecksum(data)
data += '|'
data = data.replace('\x00', '\\x00')

splitData = [] #list for all the data
splitamount = 58 #amount of bytes per segment - 1 for the packetnumber
for x in range(0, len(data), splitamount): #splits all data in segments
	splitData.append(data[x : x + splitamount])

packetNumber = 0 #packagenumber starts at 0 and then goes from 1 till 9, then resets to 1 (0 is reserved for start of transmission)

for datasegment in splitData: #sends all segments
	print(packetNumber)
	print(datasegment)
	os.system(r'echo -n "' + str(packetNumber) + datasegment + '" | nc -u 92.70.3.242 4071 -w' + str(sleeptime))
	packetNumber += 1
	if(packetNumber > 9): #resets packetnumber to 1 when it is over 9
		packetNumber = 1
print(data)
print(len(data))
