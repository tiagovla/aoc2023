#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 256
#define ARRAY_LENGTH 256

typedef struct {
  int numbers[ARRAY_LENGTH];
  int size;
} Card;

void add_number(Card *card, int number) {
  card->numbers[card->size++] = number;
}

int calculate_points(Card *winning_numbers, Card *card) {
  int points = 0;
  for (int i = 0; i < card->size; i++) {
    for (int j = 0; j < winning_numbers->size; j++) {
      if (card->numbers[i] == winning_numbers->numbers[j]) {
        if (points == 0)
          points = 1;
        else
          points <<= 1;
        break;
      }
    }
  }
  return points;
}

int main() {
  char *filename = "input.txt";
  FILE *fp = fopen(filename, "r");
  char line[LINE_LENGTH];
  char *token;
  int score_sum = 0;

  while (fgets(line, sizeof(line), fp)) {
    Card winning_numbers = {0};
    Card card = {0};

    line[strcspn(line, "\n")] = 0;
    printf("%s\n", line);

    char *start_pos = strchr(line, ':');
    char *delimiter = strchr(line, '|');
    *delimiter = '\0';

    token = strtok(start_pos + 1, " ");

    while (token != NULL) {
      add_number(&winning_numbers, atoi(token));
      token = strtok(NULL, " ");
    }

    token = strtok(delimiter + 1, " ");
    while (token != NULL) {
      add_number(&card, atoi(token));
      token = strtok(NULL, " ");
    }
    int result = calculate_points(&winning_numbers, &card);
    score_sum += result;
  }
  printf("SUM: %d\n", score_sum);

  fclose(fp);

  return 0;
}
