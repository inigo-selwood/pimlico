#ifndef _PARSE_BUFFER_H
#define _PARSE_BUFFER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	uint32_t index;
	int32_t line;
	int16_t column;
} Position;

typedef struct {
	const char *text;
	const char *commentSequence;

	uint32_t length;

	Position position;

	uint32_t  lineCount; 
	uint32_t *lineIndices;
	uint8_t  *lineIndentations;
} ParseBuffer;

Position positionCreate();

uint8_t positionPrint(Position *position, char *buffer, size_t size);

ParseBuffer *parseBufferCreate(const char *text, const char *commentSequence);

uint8_t parseBufferGetCharacter(ParseBuffer *buffer, char *value);

uint8_t parseBufferFinished(ParseBuffer *buffer, uint8_t *finished);

uint8_t parseBufferIncrement(ParseBuffer *buffer, uint16_t steps);

uint8_t parseBufferRead(ParseBuffer *buffer, char *value, uint8_t consume);

uint8_t parseBufferLineIndentation(ParseBuffer *buffer, 
	uint8_t *indentation, 
	uint32_t lineNumber);

uint8_t parseBufferLineText(ParseBuffer *buffer, 
	char *text, 
	size_t size,
	uint32_t lineNumber);

uint8_t parseBufferMatch(ParseBuffer *buffer, 
	const char *text, 
	uint8_t *match, 
	uint8_t consume);

uint8_t parseBufferSeek(ParseBuffer *buffer, 
	const char *text,
	uint8_t *result, 
	uint8_t consume, 
	uint8_t limit);

uint8_t parseBufferSkipLine(ParseBuffer *buffer);

uint8_t parseBufferSkipSpace(ParseBuffer *buffer, uint8_t includeNewlines);

uint8_t parseBufferDestroy(ParseBuffer *buffer);

#endif // _PARSE_BUFFER_H