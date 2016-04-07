#include "ringbuffer.h"

RingBuffer::RingBuffer()
{
    // Set head and tail to begin
    head = 0;
    tail = 0;

    // Set buffer elements
    for(int i = 0; i < BUFMAX; i++)
    {
        char *tmp = new char[BUFSIZE];
        buffer[i] = tmp;
        std::memset(buffer[i], '\0', BUFSIZE);
    }
}

RingBuffer::~RingBuffer()
{

}

bool RingBuffer::push(char * data)
{
    // Get the next QBuffer
    int next = head + 1;

    // Check if it made a full cycle
    if (next >= BUFMAX)
        next = 0;

    // Check if RingBuf is full
    if (next == tail)
        return false;

    // Push data
    std::memcpy(buffer[head], data, BUFSIZE);

    head = next;

    return true;
}

bool RingBuffer::pop(char * data)
{
    // Check for if there's unread data
    if (head == tail)
        return false;

    // Pop data and place it in data
    std::memcpy(data, buffer[tail], BUFSIZE);

    // Clear the buffer
    std::memset(buffer[tail], '\0', BUFSIZE);

    // Check if it made a full cycle
    int next = tail + 1;
    if (next >= BUFMAX)
        next = 0;

    tail = next;

    return true;
}

// Check if there is data to read. If no data, returns true.
bool RingBuffer::isEmpty()
{
    if (head == tail)
        return true;
    return false;
}
