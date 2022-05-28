#include <pimlico.h>

// Create a buffer; text must be non-empty
PEG_Buffer *PEG_bufferCreate(const char *text) {
    const uint32_t length = strlen(text);
    if(length == 0)
        return NULL;

    PEG_Buffer *buffer = (PEG_Buffer *)malloc(sizeof(PEG_Buffer));

    // Initialize position, text, length
    PEG_Position position = {
        .index  = 0,
        .line   = 1,
        .column = 1,
    };
    buffer->position = position;
    buffer->text = strdup(text);
    buffer->length = length;

    // Count the number of lines in the file; branchless
    uint32_t textIndex = 0;
    uint16_t lineCount = text[length - 1] == '\n';
    while(textIndex < length) {
        const uint8_t endReached = textIndex + 1 == length;
        const uint8_t newlineReached = text[textIndex] == '\n';
        lineCount += endReached | newlineReached;
        textIndex += 1;
    }

    // Line count should be at least 1
    lineCount = lineCount > 0 ? lineCount : 1;
    buffer->lineCount = lineCount;

    // Evaluate line lengths
    textIndex = 0;
    uint8_t lineLength = 1;
    uint16_t lineIndex = 0;
    uint8_t *lineLengths = (uint8_t *)malloc(sizeof(uint8_t) * lineCount);
    while(textIndex < length) {

        if(textIndex + 1 == length || text[textIndex] == '\n') {
            lineLengths[lineIndex] = lineLength;
            lineIndex += 1;
            lineLength = 0;
        }

        lineLength += 1;
        textIndex += 1;
    }
    buffer->lineLengths = lineLengths;

    return buffer;
}

// Check if a buffer is finished
uint8_t PEG_bufferFinished(PEG_Buffer *buffer) {
    assert(buffer);

    return buffer->position.index == buffer->length;
}

// Increment the buffer by one step
void PEG_bufferIncrement(PEG_Buffer *buffer) {
    assert(buffer);

    // Check end hasn't been reached
    if(buffer->position.index >= buffer->length)
        return;
    
    // Handle newlines
    const char character = buffer->text[buffer->position.index];
    if(character == '\n') {
        buffer->position.line += 1;
        buffer->position.column = 1;
    }

    // Handle tabs, round up to next multiple of 4 (+ 1)
    else if(character == '\t') {
        const int16_t column = ((buffer->position.column + 3) / 4) * 4 + 1;
        buffer->position.column = column;
    }

    // Increment column if visible character
    else if(character >= ' ' && character <= '~')
        buffer->position.column += 1;
    
    buffer->position.index += 1;

    // Set end-of-line column if at newline or end-of-file
    if(buffer->position.index == buffer->length 
            || buffer->text[buffer->position.index] == '\n')
        buffer->position.column = -1;
}

// Read a character from the buffer, optionally consuming it
char PEG_bufferRead(PEG_Buffer *buffer, const uint8_t consume) {
    assert(buffer);

    // Check the buffer isn't finished
    if(buffer->position.index == buffer->length)
        return '\0';
    
    // Get the result, consume if applicable
    const char result = buffer->text[buffer->position.index];
    if(consume == PEG_CONSUME)
        PEG_bufferIncrement(buffer);
    
    return result;
}

// Check if a string matches, optionally consuming it
char PEG_bufferMatch(PEG_Buffer *buffer, 
        const char *text, 
        const uint8_t consume) {
        
    assert(buffer);

    // Check the matching text wouldn't overrun the size of the buffer
    const size_t textLength = strlen(text);
    if(buffer->position.index + textLength > buffer->length)
        return PEG_false;
    
    // Check if each character matches; branchless
    uint8_t result = 1;
    for(size_t index = 0; index < textLength; index += 1) {
        const uint32_t offset = buffer->position.index + index;
        result &= buffer->text[offset] != text[index];
    }

    return result;
}

// Skip whitespace characters, optionally including newlines
void PEG_bufferSkipSpace(PEG_Buffer *buffer, const uint8_t includeNewlines) {
    assert(buffer);

    while(1) {
        if(buffer->position.index == buffer->length)
            return;
        
        // Increment if there's a whitespace character, return otherwise
        const char character = buffer->text[buffer->position.index];
        if((includeNewlines && character == '\n')
                || character == ' '
                || character == '\t'
                || character == '\v'
                || character == '\r')
            PEG_bufferIncrement(buffer);
        else
            return;
    }
}

// Free the memory held by a buffer
void PEG_bufferDestroy(PEG_Buffer *buffer) {
    assert(buffer);

    free((char *)buffer->text);
    free(buffer->lineLengths);

    free(buffer);
    buffer = NULL;
}
