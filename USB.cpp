#include "USB.h" 


USB::USB(unsigned short sendBufferLength, unsigned short receiveBufferLength) : 
		sendBufferLength(sendBufferLength),
		recvBufferLength(receiveBufferLength),
		sendBufferInputCursor(0),
		sendBufferOutputCursor(0),
		recvBufferCursor(0),
		sendBufferCount(0),
		recvBufferCount(0),
		sendBuffer(new char[sendBufferLength]),
		recvBuffer(new char[recvBufferLength]),
		outputBuffer(new char[64]),
		autoflush(true)
		{
	
}

USB::~USB(){
	delete sendBuffer;
	delete recvBuffer;
}

bool USB::AutoFlush(bool autoFlush){
	return autoflush = autoFlush;
}

bool USB::Error(String errorMessage){
	lastError = errorMessage;
	return false;
}

bool USB::Flush(unsigned short &bytesSent){
	bytesSent = 0;
	//Serial.print("send buffer length : ");
	//Serial.println(sendBufferCount);
	while(sendBufferCount > 0){
		if(sendBufferCount > 63){
			sendByteCount = 63;
		} else {
			sendByteCount = sendBufferCount;
		}
		//Serial.print("buffer size : ");
		//Serial.println(sendBufferCount);
		Serial.print("send byte count : ");
		Serial.println(sendByteCount);
		sendBufferCount -= sendByteCount;
		bytesSent += sendByteCount;	
		outputBuffer[0] = bytesSent;
		iterator = 0;
		while(iterator<sendByteCount){
			outputBuffer[1+iterator++] = sendBuffer[sendBufferOutputCursor++];
		}
		int n = RawHID.send(outputBuffer,100);
		if(n == 0){
			return Error("Couldn't write to pc.");
		}
	}
	return bytesSent > 0;
}

bool USB::Receive(char *byte,unsigned short length){
	return false;
}

bool send(const char *data,unsigned short length){
	return false;
}

bool USB::Send(const char *byte,unsigned short length){
	if(length > sendBufferLength) return false;
	StoreSendBufferData(byte,length);
	if(autoflush){
		Flush(bytesSent);
	}
	

	return false;
}

bool USB::StoreSendBufferData(const char *byte,unsigned short length){
	if(sendBufferCount + length > sendBufferLength){
		
		//Serial.print("buffer overflow");
		return false;
	}
	sendBufferCount += length;
	iterator = 0;
	while(iterator < length){
		//Serial.print(iterator);
		sendBuffer[sendBufferInputCursor++] = byte[iterator++];
		if(sendBufferInputCursor == sendBufferLength){
			sendBufferInputCursor = 0;
		}
	}
	
	return true;
}

