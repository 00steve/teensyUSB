#ifndef USB_H
#define USB_H

#include "SPI.h"
#define UBC 63 //the Universal Byte Constant (it exists and determines the way this class acts)

class USB{
private:
	unsigned short sendBufferLength;
	unsigned short recvBufferLength;
	unsigned short sendBufferInputCursor;
	unsigned short sendBufferOutputCursor;
	unsigned short recvBufferCursor;
	unsigned short sendBufferCount;
	unsigned short recvBufferCount;
	char *sendBuffer;
	char *recvBuffer;
	/*different than the send buffer. This buffer is literally just 
	the stuff that is being imminently sent*/
	char *outputBuffer;
	
	/*is populated whenever a boolean function returns false,
	which signifies that the function failed, for whatever reason.*/
	String lastError;
	
	/*runtime variables, for the sake of maybe using slightly more 
	ram, but not having to declare things, and have the memory being
	used by the class move around, they are declared here, so the 
	class should never use more or less memory.*/
	unsigned short bytesSent;
	unsigned short bytesLeft;
	unsigned short sendByteCount;
	unsigned short iterator;
	
	/*the default is true. If this is set to true, at the end of every
	Send() call, if there are 64 or more bytes stored in the send buffer,
	it will call the Flush() function. If it is set to false, this will
	not be called. It will be up to the programmer to flush when they are 
	ready to send data.*/
	bool autoflush;
	
	bool send(const char *data,unsigned short length);
	
	
	
public:
	/*the constructor for the usb class. The size of buffer that should 
	be used must be passed in for the incomming packets (sendBufferLength)
	and the outgoing packets (receiveBufferLength). The buffer length 
	should also be a multiple of 64, but if it isn't, it can be our little
	secret that noone else will know.*/
	USB(unsigned short sendBufferLength, unsigned short receiveBufferLength);
	
	/*cleans up anything left over when the class should be destroyed. 
	Pretty much, that means any pointers.*/
	~USB();
	
	/*change the autoflush property. To learn more about the autoflush property,
	look for the "bool autoflush" variable declaration above.*/
	bool AutoFlush(bool autoFlush);

	bool Error(String errorMessage);
	
	/*attempts to send data to the computer. It will attempt to send it in 
	64 byte chunks. This function is automatically called at the end of 
	every Send() function if the added bytes result in a buffer with more 
	than 64 bytes being held.*/
	bool Flush(unsigned short &bytesSent);
	
	bool Receive(char *byte,unsigned short length);
	
	bool Send(const char *byte,unsigned short length);
	
	bool StoreSendBufferData(const char *byte,unsigned short length);
};


#endif