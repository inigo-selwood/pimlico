#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "parse_buffer.h"

Position positionCreate() {
    Position position;

    position.index = 0;
    position.line = -1;
    position.column = -1;

    return position;
}

/* Serialize a position object

Arguments
---------
position: Position *
	the position to serialize
buffer: char *
	the buffer to serialize to
size: size_t
	the size of the buffer in bytes

Returns
-------
success: uint8_t
	non-zero if the serialization succeeded
*/
uint8_t positionPrint(Position *position, char *buffer, size_t size) {
    if(position == NULL || buffer == NULL || size == 0)
        return 0;
    
    snprintf(buffer, size, "[%d:%d]", position->line, position->column);
	return 1;
}

/* Create a parse buffer object

Arguments
---------
text: char *
	the text to fill the buffer with

Returns
-------
buffer: ParseBuffer
	the initialized buffer object, or NULL if it failed
*/
ParseBuffer *parseBufferCreate(const char *text) {
    if(text == NULL)
        return NULL;
            
    const uint32_t length = strlen(text);
    if(length == 0)
        return NULL;

    Position position = positionCreate();
    if(length > 0) {
        position.line = 1;
        if(text[0] != '\n')
            position.column = 1;
    }

    uint32_t index = 1;
    uint32_t lineCount = 1;
    while(1) {
        if(index == length)
            break;
        else if(text[index - 1] == '\n')
            lineCount += 1;
        index += 1;
    }

    index = 0;
    uint32_t lineNumber = 0;
    uint32_t lineStartIndex = 0;
    uint32_t *lineIndices = (uint32_t *)malloc(sizeof(uint32_t) * lineCount);
    uint8_t *lineIndentations = (uint8_t *)malloc(sizeof(uint8_t) * lineCount);
    while(index < length) {
        lineStartIndex = index;

        uint8_t indentation = 0;
        while(1) {
            if(index == length)
                break;

            if(text[index] == '\t')
                indentation = ((indentation + 4) & ~0x03);
            else if(text[index] == ' ')
                indentation += 1;
            else
                break;

            index += 1;
        }

        lineIndices[lineNumber] = lineStartIndex;
        lineIndentations[lineNumber] = indentation;

        while(index < length && text[index] != '\n')
            index += 1;

        index += 1;
        lineNumber += 1;
    }
    
    ParseBuffer temporary = {
        strdup(text),
        length,
        position,
        lineCount,
        lineIndices,
        lineIndentations
    };

    ParseBuffer *buffer = (ParseBuffer *)malloc(sizeof(ParseBuffer));
    if(buffer == NULL)
        return NULL;
    memcpy(buffer, &temporary, sizeof(ParseBuffer));

    return buffer;
}

/* Gets a single character from the buffer

Arguments
---------
buffer: ParseBuffer *
    the buffer to get a character from
value: char *
    pointer to fill with the character found

Returns
-------
success: uint8_t
    non-zero if the operation succeeded
*/
uint8_t parseBufferGetCharacter(ParseBuffer *buffer, char *value) {
    if(buffer == NULL 
            || value == NULL
            || buffer->position.index == buffer->length)
        return 0;
    
    *value = buffer->text[buffer->position.index];
    return 1;
}

/* Checks whether the buffer is finished

Arguments
---------
buffer: ParseBuffer *
    the buffer to check
value: char *
    pointer to fill with the result

Returns
-------
success: uint8_t
    non-zero if the operation succeeded
*/
uint8_t parseBufferFinished(ParseBuffer *buffer, uint8_t *finished) {
    if(buffer == NULL
            || finished == NULL)
        return 0;
    
    *finished = buffer->position.index == buffer->length;
    return 1;
}

/* Increments the buffer a given number of steps

Arguments
---------
buffer: ParseBuffer *
    the buffer to increment
steps: uint16_t
    the number of steps to increment the buffer's position by

Returns
-------
success: uint8_t
    non-zero if the operation succeeded
*/
uint8_t parseBufferIncrement(ParseBuffer *buffer, uint16_t steps) {
    if(buffer == NULL)
        return 0;
    
    // Stop when the end of the buffer has been reached
    for(uint16_t index = 0; index < steps; index += 1) {
        if(buffer->position.index >= buffer->length)
            return 1;
        
        // If the current character is a newline, increment the line number and 
        // reset the column
        char character = buffer->text[buffer->position.index];
        if(character == '\n') {
            buffer->position.line += 1;
            buffer->position.column = 1;
        }

        // Handle tabs by rounding up to the next multiple of 4 (+1)
        else if(character == '\t') {
            buffer->position.column = 
                    ((buffer->position.column + 3) / 4) * 4 + 1;
        }

        // Failing the above, and if the character wasn't format-related, 
        // increment the column
        else if(character >= ' ' && character <= '~')
            buffer->position.column += 1;
        
        // Move to the next character, and update the column value
        buffer->position.index += 1;    
        if(buffer->position.index == buffer->length)
            buffer->position.column = -1;
        else if(buffer->text[buffer->position.index] == '\n')
            buffer->position.column = -1;
    }

    return 1;
}

/* Reads a single character from the buffer, optionally incrementing

Arguments
---------
buffer: ParseBuffer *
    the buffer to read from
value: char *
    pointer to a character to fill with the next character in the buffer
consume: uint8_t
    if non-zero, increments past the character being read

Returns
-------
success: uint8_t
    non-zero if the operation succeeded
*/
uint8_t parseBufferRead(ParseBuffer *buffer, char *value, uint8_t consume) {
    if(buffer == NULL 
            || value == NULL 
            || buffer->position.index == buffer->length)
        return 0;
    
    *value = buffer->text[buffer->position.index];
    if(consume)
        parseBufferIncrement(buffer, 1);
    
    return 1;
}

/* Gets the indentation of the buffer on a given line

Arguments
---------
buffer: ParseBuffer *
    the buffer to read indentation from
indentation: uint8_t *
    pointer at which the result is stored
lineNumber: uint32_t
    the number of the line to check. If zero, the current line is used

Returns
-------
success: uint8_t
    non-zero if the operation succeeded
*/
uint8_t parseBufferLineIndentation(ParseBuffer *buffer, 
        uint8_t *indentation, 
        uint32_t lineNumber) {
    
    if(buffer == NULL || lineNumber > buffer->lineCount)
        return 0;
    
    else if(lineNumber <= 0)
        lineNumber = buffer->position.line;
    
    *indentation = buffer->lineIndentations[lineNumber - 1];
    return 1;
}

/* Gets the text on a given line of the buffer

Arguments
---------
buffer: ParseBuffer *
    the buffer from which to get text
value: char *
    pointer to fill with the result
size: size_t
    size of the value buffer, in bytes
lineNumber: uint32_t
    the number of the line whose text to read. If zero, the current line is 
    presumed

Returns
-------
success: uint8_t
    non-zero if the operation succeeded
*/
uint8_t parseBufferLineText(ParseBuffer *buffer, 
        char *text, 
        size_t size,
        uint32_t lineNumber) {
    
    if(buffer == NULL)
        return 0;
    
    if(lineNumber <= 0)
        lineNumber = buffer->position.line;
    
    if(lineNumber < 1 || lineNumber > buffer->lineCount)
        return 0;
    
    // Work out the start index, using the given line number
    uint32_t startIndex = 0;
    if(lineNumber > 1)
        startIndex = buffer->lineIndentations[lineNumber - 1];
    
    // Move the start index forward until we reach a non-whitespace character
    while(startIndex < buffer->length && 
            (buffer->text[startIndex] <= ' ' 
                || buffer->text[startIndex] > '~'))
        startIndex += 1;
    
    // Work out the end index
    uint32_t endIndex = buffer->length;
    if(lineNumber > buffer->lineCount)
        endIndex = buffer->lineIndices[lineNumber] - 1;
    else if(buffer->length > 0 && buffer->text[buffer->length - 1] == '\n')
        endIndex = buffer->length - 1;
    
    // Move the end index backward until a non-whitespace character is reached
    while(endIndex > startIndex && 
            (buffer->text[startIndex] < ' ' 
                || buffer->text[startIndex] > '~'))
        endIndex -= 1;
    
    // Copy the substring into the text result
    strncpy(text, buffer->text + startIndex, endIndex - startIndex);
    return 1;
}

/* Checks whether a given string matches at the current position in the buffer

Arguments
---------
buffer: ParseBuffer *
    the buffer to check for a match in
text: char *
    the text to check for
match: uint8_t *
    pointer to value where the result is stored (non-zero if a match was 
    present)
consume: uint8_t
    if non-zero, consumes the matched string

Returns
-------
success: uint8_t
    non-zero if the operation succeeded
*/
uint8_t parseBufferMatch(ParseBuffer *buffer, 
        char *text, 
        uint8_t *match, 
        uint8_t consume) {
    
    if(buffer == NULL
            || text == NULL 
            || match == NULL)
        return 0;
    
    // If a match would take us past the end of the buffer, don't bother 
    // checking
    uint8_t textLength = strlen(text);
    if(buffer->position.index + textLength >= buffer->length) {
        *match = 0;
        return 1;
    }

    // In the most common case, we're just tring to match a single character.
    // In that case, we can directly compare the two
    if(textLength == 1)
        *match = buffer->text[buffer->position.index] == text[0];
    
    // If the text is more than one character long, we have to pull out a 
    // substring and compare that instead.
    else {
        char substring[textLength];
        strncpy(substring, 
                buffer->text + buffer->position.index, 
                textLength);
        *match = strncmp(text, substring, textLength) == 0;
    }

    if(*match && consume)
        parseBufferIncrement(buffer, textLength);
    
    return 1;
}

/* Skips past whitespace characters found in the buffer

Arguments
---------
buffer: ParseBuffer *
    the buffer to use
includeNewlines: uint8_t
    if non-zero, considers newlines as whitespace

Returns
-------
success: uint8_t
    non-zero if the operation succeeded
*/
uint8_t parseBufferSkipSpace(ParseBuffer *buffer, uint8_t includeNewlines) {
    if(buffer == NULL)
        return 0;
    
    while(1) {
        if(buffer->position.index == buffer->length)
            break;
        
        char character = buffer->text[buffer->position.index];
        if(character == ' '
                || character == '\t'
                || character == '\r'
                || character == '\v'
                || (includeNewlines && character == '\n'))
            parseBufferIncrement(buffer, 1);
        else
            break;
    }

    return 1;
}

/* Destroy a buffer instance

Arguments
---------
buffer: ParseBuffer
    the buffer to destroy
*/
uint8_t parseBufferDestroy(ParseBuffer *buffer) {
    if(buffer == NULL)
        return 0;
    
    free(buffer->lineIndentations);
    free(buffer->lineIndices);

    free(buffer);
    buffer = NULL;
    return 1;
}