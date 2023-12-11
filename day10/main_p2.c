#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 256

typedef enum { DOWN, RIGHT, UP, LEFT } Direction;

char *direction_to_string(Direction direction) {
  char *directions[] = {"DOWN", "RIGHT", "UP", "LEFT"};
  return directions[direction];
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

Point get_next_position(Point current_position, Direction current_direction) {
  if (current_direction == DOWN) {
    current_position.x++;
  } else if (current_direction == RIGHT) {
    current_position.y++;
  } else if (current_direction == UP) {
    current_position.x--;
  } else if (current_direction == LEFT) {
    current_position.y--;
  }
  return current_position;
}

int main() {
  FILE *fp = fopen("input.txt", "r");
  if (fp == NULL) {
    printf("Could not open file input.txt\n");
    exit(1);
  }
  char line[LINE_LENGTH];

  fgets(line, sizeof(line), fp);
  int map_size = strlen(line) - 1;

  char map[map_size][map_size];
  strncpy(map[0], line, map_size);

  for (int i = 1; fgets(line, sizeof(line), fp); i++) {
    strncpy(map[i], line, map_size);
  }

  Point start_position = get_start_position(&map[0][0], map_size);
  map[start_position.x][start_position.y] = 'L';

  printf("START:%d %d\n", start_position.x, start_position.y);

  Direction current_direction = RIGHT;
  Point current_position = start_position;
  printf("START: %d %d %c %s\n", current_position.x, current_position.y,
         map[current_position.x][current_position.y],
         direction_to_string(current_direction));

  Direction last_up_down = DOWN;

  for (int i = 0;; i++) {
    // printf("CURRENT: %d %d %c %s\n", current_position.x, current_position.y,
    //        map[current_position.x][current_position.y],
    //        direction_to_string(current_direction));
    Point next_position =
        get_next_position(current_position, current_direction);

    char symbol = map[next_position.x][next_position.y];
    map[next_position.x][next_position.y] = 'X';

    Direction next_direction = get_next_direction(symbol, next_direction);

    if (current_direction == UP || current_direction == DOWN)
      last_up_down = current_direction;

    if (next_position.x != current_position.x) {
      if (current_direction == UP)
        map[next_position.x][next_position.y] = 'U';
      else if (current_direction == DOWN)
        map[next_position.x][next_position.y] = 'D';
    } else {
      if (next_direction != last_up_down) {
        if (next_direction == UP)
          map[next_position.x][next_position.y] = 'U';
        else if (next_direction == DOWN)
          map[next_position.x][next_position.y] = 'D';
      }
    }

    current_position = next_position;
    current_direction = next_direction;

    if (current_position.x == start_position.x &&
        current_position.y == start_position.y) {
      break;
    }
  }

  for (int i = 0; i < map_size; i++) {
    for (int j = 0; j < map_size; j++) {
      char c = map[i][j];
      if (c != 'X' && c != 'U' && c != 'D') {
        int x = 0;
        for (int k = j + 1; k < map_size; k++) {
          if (map[i][k] == 'U')
            x++;
          if (map[i][k] == 'D')
            x--;
        }
        if (x != 0) {
          map[i][j] = 'I';
        } else
          map[i][j] = 'O';
        printf("%c", map[i][j]);
      } else
        printf("%c", map[i][j]);
    }
    printf("\n");
  }

  int num = 0;
  for (int i = 0; i < map_size; i++) {
    for (int j = 0; j < map_size; j++) {
      if (map[i][j] == 'I')
        num++;
    }
  }
  printf("NUM: %d\n", num);

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
