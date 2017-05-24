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
	Serial.println(sendBufferCount);
	while(sendBufferCount > UBC){
		Serial.print("buffer over 63 : ");
		Serial.println(sendBufferCount);
		bytesLeft = sendBufferCount - UBC;
		sendByteCount = bytesLeft > -1 ? UBC : UBC - bytesLeft;
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
	StoreSendBufferData(byte,length);
	if(autoflush){
		Serial.print("flush");
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

