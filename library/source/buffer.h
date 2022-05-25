#ifndef _PEG_BUFFER_H
#define _PEG_BUFFER_H

#include <stdint.h>

#define PEG_TRUE  1
#define PEG_FALSE 0

#define PEG_CONSUME    1
#define PEG_NO_CONSUME 0

#define PEG_SKIP_NEWLINES    1
#define PEG_NO_SKIP_NEWLINES 0

typedef struct PEG_Position {
    
    uint32_t index;
    
    int16_t line;
    int16_t column;

} PEG_Position;

typedef struct PEG_Buffer {
    
    PEG_Position position;
    
    const char *text;
    
    uint32_t length;

    uint8_t *lineLengths;
    uint16_t lineCount;

} PEG_Buffer;

// Create a buffer; text must be non-empty
PEG_Buffer *PEG_bufferCreate(const char *text);

// Check if a buffer is finished
uint8_t PEG_bufferFinished(PEG_Buffer *buffer);

// Increment the buffer by one step
void PEG_bufferIncrement(PEG_Buffer *buffer);

// Read a character from the buffer, optionally consuming it
char PEG_bufferRead(PEG_Buffer *buffer, const uint8_t consume);

// Check if a string matches, optionally consuming it
char PEG_bufferMatch(PEG_Buffer *buffer, 
        const char *text, 
        const uint8_t consume);

// Skip whitespace characters, optionally including newlines
void PEG_bufferSkipSpace(PEG_Buffer *buffer, const uint8_t includeNewlines);

// Free the memory held by a buffer
void PEG_bufferDestroy(PEG_Buffer *buffer);

#endif // _PEG_BUFFER_H