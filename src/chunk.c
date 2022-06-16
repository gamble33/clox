#include "include/chunk.h"
#include "include/memory.h"

#include <stdlib.h>
#include <stdio.h>

void initChunk(Chunk* chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;

  chunk->currentLine.lineNumber = 0;
  chunk->currentLine.instructionIndex = 0;
  chunk->lineCount = 0;
  chunk->lineCapacity = 0;
  chunk->lines = NULL;

  initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->capacity);
  freeValueArray(&chunk->constants);
  initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {

  if (line != chunk->currentLine.lineNumber) {
    if (chunk->lineCapacity < chunk->lineCount + 1) {
      int oldCapacity = chunk->lineCapacity;
      chunk->lineCapacity = GROW_CAPACITY(oldCapacity);
      chunk->lines = GROW_ARRAY(
        Line,
        chunk->lines,
        oldCapacity,
        chunk->lineCapacity
      );
    }
    Line newLine = {line, chunk->capacity};
    chunk->lines[chunk->lineCount] = newLine;
    chunk->lineCount++;
  }

  if (chunk->capacity < chunk->count + 1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code = GROW_ARRAY(
      uint8_t,
      chunk->code,
      oldCapacity,
      chunk->capacity
    );
  }

  chunk->code[chunk->count] = byte;
  chunk->count++;
}

int addConstant(Chunk* chunk, Value value) {
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}

int getLine(Chunk* chunk, int instructionIndex) {
  for (int i = 0; i < chunk->lineCount; i++) {
    if (chunk->lines[i].instructionIndex > instructionIndex)
      return chunk->lines[i - 1].lineNumber;
  }
  return chunk->lines[chunk->lineCount - 1].lineNumber;
}
