#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define MAX_LINE 512
#define ALLOWED_RED 12
#define ALLOWED_GREEN 13
#define ALLOWED_BLUE 14
#define MAX_HANDS 50

char *possible_hands(int i) {
  switch (i) {
  case 0:
    return "red";
  case 1:
    return "green";
  case 2:
    return "blue";
  }
}

typedef union {
  struct {
    int red;
    int green;
    int blue;
  };
  int colors[3];
} Hand;

typedef struct {
  int id;
  Hand hands[MAX_HANDS];
} Game;

int parse_id(char *str) {
  char *p_col, buffer[10];

  p_col = strchr(str, ':');
  strncpy(buffer, &str[5], p_col - str - 5);
  buffer[p_col - str - 5] = '\0';

  return atoi(buffer);
}

int parse_color(char *str, char *color) {
  char buffer_color[MAX_LINE], *color_position;

  color_position = strstr(str, color);
  if (color_position == NULL)
    return 0;

  strncpy(buffer_color, str, color_position - str);
  buffer_color[color_position - str] = '\0';

  return atoi(buffer_color);
}

Hand parse_hand(char *str) {
  Hand hand = {0};
  char buffer[3][MAX_LINE];
  char *p_col, *p_col2;

  memset(buffer, 0, sizeof(buffer));
  p_col = strchr(str, ',');
  if (p_col == NULL)
    strcpy(buffer[0], str);
  else {
    strncpy(buffer[0], str, p_col - str);
    p_col2 = strchr(p_col + 1, ',');
    if (p_col2 == NULL)
      strcpy(buffer[1], p_col + 1);
    else {
      strncpy(buffer[1], p_col + 1, p_col2 - p_col - 1);
      strcpy(buffer[2], p_col2 + 1);
    }
  }
  for (int j = 0; j < MAX_HANDS; j++) {
    for (int i = 0; i < 3; i++) {
      int color = parse_color(buffer[j], possible_hands(i));
      hand.colors[i] += color;
    }
  }
  return hand;
}

Game parse(char *str) {
  Game game = {0};
  game.id = parse_id(str);
  char game_str[MAX_HANDS][MAX_LINE];
  memset(game_str, 0, sizeof(game_str));
  char *p_id = strchr(str, ':');
  char *p_c1;
  char *p = str;
  for (int i = 0; i < MAX_HANDS; i++) {
    p_c1 = strchr(p, ';');
    if (p_c1 == NULL) {
      strcpy(game_str[i], p_id + 1);
      break;
    } else {
      strncpy(game_str[i], p_id + 1, p_c1 - p_id - 1);
      p_id = p_c1;
      p = p_c1 + 1;
    }
  }
  for (int i = 0; i < MAX_HANDS; i++) {
    game.hands[i] = parse_hand(game_str[i]);
  }
  return game;
}

int is_valid(Game game) {
  for (int i = 0; i < MAX_HANDS; i++) {
    if (game.hands[i].blue > ALLOWED_BLUE || game.hands[i].red > ALLOWED_RED ||
        game.hands[i].green > ALLOWED_GREEN)
      return 0;
  }
  return 1;
}

int power_value(Game game) {
  int values[3] = {0};
  for (int i = 0; i < MAX_HANDS; i++) {
    for (int j = 0; j < 3; j++) {
      values[j] = MAX(game.hands[i].colors[j], values[j]);
    }
  }
  return values[0] * values[1] * values[2];
}

int main() {

  char *filename = "input.txt";
  FILE *fp = fopen(filename, "r");

  char line[MAX_LINE];
  int sum_ids = 0, sum_power = 0;
  int valid;

  while (fgets(line, sizeof(line), fp)) {
    Game game = parse(line);
    valid = is_valid(game);
    if (valid) {
      sum_ids += game.id;
    }
    sum_power += power_value(game);
  }

  fclose(fp);

  printf("sum_ids = %d\n", sum_ids);
  printf("sum_power = %d\n", sum_power);

  return 0;
}
