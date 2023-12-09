#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 256
#define ARRAY_LENGTH 256

typedef struct {
  int data[ARRAY_LENGTH];
  unsigned int size;
} Vector;

void vector_append(Vector *vector, int value) {
  vector->data[vector->size++] = value;
}

int next_number(Vector *vec) {
  // create diff
  Vector diff = {};
  for (int i = 0; i < vec->size - 1; i++)
    vector_append(&diff, vec->data[i + 1] - vec->data[i]);
  // check if all zeros
  int all_zeros = 1;
  for (int j = 0; j < diff.size; j++) {
    if (diff.data[j] != 0)
      all_zeros = 0;
  }
  // get last digit
  return all_zeros == 1 ? vec->data[vec->size - 1]
                        : vec->data[vec->size - 1] + next_number(&diff);
}

int prev_number(Vector *vec) {
  // create diff
  Vector diff = {};
  for (int i = 0; i < vec->size - 1; i++)
    vector_append(&diff, vec->data[i + 1] - vec->data[i]);
  // check if all zeros
  int all_zeros = 1;
  for (int j = 0; j < diff.size; j++) {
    if (diff.data[j] != 0)
      all_zeros = 0;
  }
  // get first digit
  return all_zeros == 1 ? vec->data[0] : vec->data[0] - prev_number(&diff);
}

int main() {
  char *filename = "input.txt";
  FILE *fp = fopen(filename, "r");
  char *token;
  char line[LINE_LENGTH];

  int sum_next = 0, sum_prev = 0;
  while (fgets(line, sizeof(line), fp)) {
    Vector vec = {0};
    token = strtok(line, " ");
    while (token != NULL) {
      vector_append(&vec, atoi(token));
      token = strtok(NULL, " ");
    }
    int res_next = next_number(&vec);
    int res_prev = prev_number(&vec);
    sum_next += res_next;
    sum_prev += res_prev;
  }
  printf("Sum next: %d\n", sum_next);
  printf("Sum prev: %d\n", sum_prev);

  fclose(fp);
}
