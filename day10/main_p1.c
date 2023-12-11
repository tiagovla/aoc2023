
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 512

typedef enum { DOWN, RIGHT, UP, LEFT } Direction;

char *direction_to_string(Direction direction) {
  switch (direction) {
  case DOWN:
    return "DOWN";
  case RIGHT:
    return "RIGHT";
  case UP:
    return "UP";
  case LEFT:
    return "LEFT";
  }
}

typedef struct {
  int x;
  int y;
} Point;

Point get_start_position(char *map, int map_size) {
  Point start_position = {-1, -1};
  for (int i = 0; i < map_size; i++) {
    for (int j = 0; j < map_size; j++) {
      if (map[i * map_size + j] == 'S') {
        start_position.x = i;
        start_position.y = j;
      }
    }
  }
  return start_position;
}

Direction get_next_direction(char symbol, Direction current_direction);

int main() {
  FILE *fp = fopen("input.txt", "r");
  char line[LINE_LENGTH];

  fgets(line, sizeof(line), fp);
  *strchr(line, '\n') = '\0';
  int map_size = strlen(line);

  char map[map_size][map_size];
  strncpy(map[0], line, map_size);

  for (int i = 1; fgets(line, sizeof(line), fp); i++) {
    *strchr(line, '\n') = '\0';
    strncpy(map[i], line, map_size);
  }

  Point start_position = get_start_position(&map[0][0], map_size);
  // printf("%d %d\n", start_position.x, start_position.y);

  Direction current_direction = RIGHT;
  Point current_position = start_position;

  for (int i = 0;; i++) {
    printf("CURRENT: %d %d %c %s\n", current_position.x, current_position.y,
           map[current_position.x][current_position.y],
           direction_to_string(current_direction));
    if (current_direction == DOWN) {
      current_position.x = current_position.x + 1;
      current_position.y = current_position.y;
    } else if (current_direction == RIGHT) {
      current_position.x = current_position.x;
      current_position.y = current_position.y + 1;
    } else if (current_direction == UP) {
      current_position.x = current_position.x - 1;
      current_position.y = current_position.y;
    } else if (current_direction == LEFT) {
      current_position.x = current_position.x;
      current_position.y = current_position.y - 1;
    }
    char symbol = map[current_position.x][current_position.y];
    // printf("GET_NEXT_DIRECTION: %c %s\n", symbol,
    // direction_to_string(current_direction));
    current_direction = get_next_direction(symbol, current_direction);
    // printf("NEXT DIRECTION: %s\n", direction_to_string(current_direction));
    if (current_position.x == start_position.x &&
        current_position.y == start_position.y) {
      printf("LOOP: %d\n", (i + 1) / 2);
      break;
    }
  }

  fclose(fp);
  return 0;
}

Direction get_next_direction(char symbol, Direction current_direction) {
  switch (current_direction) {
  case DOWN:
    switch (symbol) {
    case '|':
      return DOWN;
    case 'L':
      return RIGHT;
    case 'J':
      return LEFT;
    }
  case RIGHT:
    switch (symbol) {
    case '-':
      return RIGHT;
    case 'J':
      return UP;
    case '7':
      return DOWN;
    }
  case UP:
    switch (symbol) {
    case '|':
      return UP;
    case '7':
      return LEFT;
    case 'F':
      return RIGHT;
    }
  case LEFT:
    switch (symbol) {
    case '-':
      return LEFT;
    case 'L':
      return UP;
    case 'F':
      return DOWN;
    }
  }
}
