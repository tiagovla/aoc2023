#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 256
#define ARRAY_LENGTH 256

typedef struct {
  int numbers[ARRAY_LENGTH];
  int size;
} Vector;

void vector_append(Vector *card, int number) {
  card->numbers[card->size++] = number;
}

int get_extra_cards(Vector *winning_numbers, Vector *card) {
  int points = 0;
  for (int i = 0; i < card->size; i++) {
    for (int j = 0; j < winning_numbers->size; j++) {
      if (card->numbers[i] == winning_numbers->numbers[j]) {
        points++;
        break;
      }
    }
  }
  return points;
}

int recursive_search(Vector *results, int index);

int main() {
  char *filename = "input.txt";
  FILE *fp = fopen(filename, "r");
  char line[LINE_LENGTH];
  char *token;
  Vector results = {0}, accumulator = {};

  while (fgets(line, sizeof(line), fp)) {
    Vector winning_numbers = {0};
    Vector card = {0};

    line[strcspn(line, "\n")] = 0;
    printf("%s\n", line);

    char *start_pos = strchr(line, ':');
    char *delimiter = strchr(line, '|');
    *delimiter = '\0';

    token = strtok(start_pos + 1, " ");
    while (token != NULL) {
      vector_append(&winning_numbers, atoi(token));
      token = strtok(NULL, " ");
    }

    token = strtok(delimiter + 1, " ");
    while (token != NULL) {
      vector_append(&card, atoi(token));
      token = strtok(NULL, " ");
    }

    int result = get_extra_cards(&winning_numbers, &card);
    vector_append(&results, result);
  }

  int sum = 0;
  for (int i = 0; i < results.size; i++) {
    sum += recursive_search(&results, i);
  }
  printf("Sum: %d\n", sum);

  fclose(fp);

  return 0;
}

// could cache this and interate in the reverse order
int recursive_search(Vector *results, int index) {
  int sum = 1;
  if (index >= results->size)
    return sum;
  for (int i = 1; i <= results->numbers[index]; i++)
    sum += recursive_search(results, index + i);
  return sum;
}
