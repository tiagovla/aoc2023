#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 256
#define ARRAY_LENGTH 100

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (b) : (a))

typedef struct {
  long long data[ARRAY_LENGTH];
  int size;
} Vector;

void vector_append(Vector *vec, long number) {
  vec->data[vec->size++] = number;
}

int find_possibilities(long long time, long long dist) {
  long double t1 = ceil(0.5 * (time - sqrt(time * time - 4 * dist)) + 0.0001);
  return floor(time - 2 * t1 + 1);
}

int main() {
  char *filename = "input.txt";
  FILE *fp = fopen(filename, "r");
  char *delimiter, *token;
  Vector time = {0};
  Vector distance = {0};

  char line[LINE_LENGTH];

  fgets(line, sizeof(line), fp);
  delimiter = strchr(line, ':');
  token = strtok(delimiter + 1, " ");
  while (token != NULL) {
    vector_append(&time, atoll(token));
    token = strtok(NULL, " ");
  }

  fgets(line, sizeof(line), fp);
  delimiter = strchr(line, ':');
  token = strtok(delimiter + 1, " ");
  while (token != NULL) {
    vector_append(&distance, atoll(token));
    token = strtok(NULL, " ");
  }

  long a = time.data[0];
  long b = distance.data[0];

  int sum = 1;
  for (int i = 0; i < time.size; i++) {
    int res = find_possibilities(time.data[i], distance.data[i]);
    sum *= res;
    printf("%d\n", res);
  }
  printf("SUM: %d\n", sum);

  fclose(fp);

  return 0;
}
