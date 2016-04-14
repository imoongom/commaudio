/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: circularbuffer.cpp
--
-- PROGRAM:     CommAudio
--
-- FUNCTIONS:
--          void initBuffer(CBufs *buf)
--          void write_buffer(CBufs * Buf, const void * data)
--          void read_buffer(CBufs * Buf, void * data);
--          void clean_buffer(CBufs *Buf)
--
-- DATE:    March 18, 2016
--
-- DESIGNER:    Eunwon Moon
--
-- PROGRAMMER:  Eunwon Moon
--
-- NOTES:
-- Circular buffer to control constant data read and write for voice
--
----------------------------------------------------------------------------------------------------------------------*/
#include "circularbuffer.h"
#include<QDebug>

void initBuffer(CBufs * Buf){
    Buf->_count = 0;
    Buf->_writePtr = &Buf->buffer;
    Buf->_readPtr = &Buf->buffer;
    Buf->_endPtr = &Buf->buffer[CIRBUFSIZE*CIRBUFMAX];
    std::memset(Buf->buffer, '\0',CIRBUFSIZE*CIRBUFMAX);
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	write_buffer
--
-- DATE: 		March 18, 2016
--
-- REVISIONS:
--
-- DESIGNER	 : 	Eunwon Moon
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	void write_buffer(CBufs * Buf, const void * data)
--
-- RETURNS: void.
--
-- NOTES:
--	This function is to write data into buffer.
--  copy data and update the number of buffer
----------------------------------------------------------------------------------------------------------------------*/
void write_buffer(CBufs * Buf, const void * data){


    if(Buf->_count == CIRBUFMAX ){
        return;
    }
    if(Buf->_writePtr == Buf->_endPtr){
        Buf->_writePtr = buffer;
    }

    std::memcpy(Buf->_writePtr, (char*)data, CIRBUFSIZE);
    //qDebug()<<"writeptr: " << (char*)(Buf->_writePtr);
    Buf->_writePtr+=CIRBUFSIZE;

    //move write pointer to first
    if(Buf->_writePtr == Buf->_endPtr){
        Buf->_writePtr = &Buf->buffer;
    }
    Buf->_count++;

}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	write_buffer
--
-- DATE: 		March 18, 2016
--
-- REVISIONS:
--
-- DESIGNER	 : 	Eunwon Moon
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	void write_buffer(CBufs * Buf, const void * data)
--
-- RETURNS: void.
--
-- NOTES:
--	This function is to read data into buffer.
--  copy data and delete in buffer for next data
----------------------------------------------------------------------------------------------------------------------*/
void read_buffer(CBufs * Buf, void * data){
    if(Buf->_count == 0)
        return ;
    std::memcpy(data, Buf->_readPtr, CIRBUFSIZE);
    std::memset(Buf->_readPtr, '\0',CIRBUFSIZE);
    Buf->_readPtr+=CIRBUFSIZE;
    if(Buf->_readPtr == Buf->_endPtr)
        Buf->_readPtr = &Buf->buffer;
    Buf->_count--;

}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	clean_buffer
--
-- DATE: 		March 18, 2016
--
-- REVISIONS:
--
-- DESIGNER	 : 	Eunwon Moon
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	void clean_buffer(CBufs * Buf)
--
-- RETURNS: void.
--
-- NOTES:
--	This function is to make buffer empty and make all value as a default
----------------------------------------------------------------------------------------------------------------------*/
void clean_buffer(CBufs * Buf){
    std::memset(Buf->buffer, '\0', sizeof(Buf->buffer));
    Buf->_writePtr = Buf->buffer;
    Buf->_readPtr = Buf->buffer;
}
