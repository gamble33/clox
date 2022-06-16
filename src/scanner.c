#include "include/scanner.h"
#include "include/common.h"

#include <stdio.h>
#include <string.h>

typedef struct {
  const char* start;
  const char* current;
  int line;
} Scanner;

Scanner scanner;

void initScanner(const char* source) {
  scanner.start = source;
  scanner.current = scanner.start;
  scanner.line = 1;

  int line = -1;
  for (;;) {
    Token token = scanToken();
    if (token.line != line) {
      printf("%4d ", token.line);
      line = token.line;
    } else {
      printf("   | ");
    }
    printf("%2d `%.*s`\n", token.type, token.length, token.start);

    if (token.type == TOKEN_EOF) break;
  }
}

static bool isAtEnd() {
  return *scanner.current == '\0';
}

static char advance() {
  return *(scanner.current++);
}

static bool match(char expected) {
  if (isAtEnd()) return false;
  if (*scanner.current != expected) return false;
  scanner.current++;
  return true;
}

static Token makeToken(TokenType type) {
  Token token;
  token.type = type;
  token.line = scanner.line;
  token.length = (int)(scanner.current - scanner.start);
  token.start = scanner.start;
  return token;
}

static Token errorToken(const char* message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.line = scanner.line;
  token.start = message;
  token.length = (int)strlen(message);
  return token;
}

Token scanToken() {
  scanner.start = scanner.current;

  if (isAtEnd()) return makeToken(TOKEN_EOF);

  char c = advance();

  switch (c) {

    case '(': return makeToken(TOKEN_LEFT_PAREN);
    case ')': return makeToken(TOKEN_RIGHT_PAREN);
    case '{': return makeToken(TOKEN_LEFT_BRACE);
    case '}': return makeToken(TOKEN_RIGHT_BRACE);
    case ',': return makeToken(TOKEN_COMMA);
    case '.': return makeToken(TOKEN_DOT);
    case '-': return makeToken(TOKEN_MINUS);
    case '+': return makeToken(TOKEN_PLUS);
    case '/': return makeToken(TOKEN_SLASH);
    case '*': return makeToken(TOKEN_STAR);
    case ';': return makeToken(TOKEN_SEMICOLON);

    case '!': return makeToken(
      match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG
    );

    case '=': return makeToken(
      match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL
    );

    case '>': return makeToken(

      match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER
    );

    case '<': return makeToken(
      match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS
    );
  }

  return errorToken("Unexpected character.");
}
