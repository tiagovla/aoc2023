#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 128
#define ARRAY_LENGTH 16

typedef struct {
  int data[ARRAY_LENGTH];
  int size;
} Array;

void array_append(Array *array, int value) {
  array->data[array->size++] = value;
}

int array_compare(Array *a, Array *b) {
  if (a->size != b->size) {
    return 0;
  }
  for (int i = 0; i < a->size; i++) {
    if (a->data[i] != b->data[i]) {
      return 0;
    }
  }
  return 1;
}

Array count_hashtag_separeted_by_dot(char *str) {
  Array result = {0};
  while (*str != '\0') {
    if (*str == '#') {
      int count = 0;
      while (*str == '#') {
        count++;
        str++;
      }
      array_append(&result, count);
    }
    str++;
  }
  return result;
}

int print_combinations(char *str, Array *groups) {
  int count = 0;
  char *question_mark = strchr(str, '?');

  if (question_mark == NULL) {
    // printf("%s\n", str);
    Array res = count_hashtag_separeted_by_dot(str);
    return array_compare(groups, &res);
  }
  *question_mark = '#';
  count += print_combinations(str, groups);
  *question_mark = '.';
  count += print_combinations(str, groups);
  *question_mark = '?';
  return count;
}

int main() {
  FILE *fp = fopen("input.txt", "r");

  char line[LINE_LENGTH];
  int sum = 0;
  while (fgets(line, sizeof(line), fp)) {
    printf("%s", line);
    char *delimiter = strchr(line, ' ');
    Array groups = {0};
    *delimiter = '\0';
    char *token = strtok(delimiter + 1, ",");
    for (int i = 0; token != NULL; i++) {
      array_append(&groups, atoi(token));
      token = strtok(NULL, ",");
    }
    sum += print_combinations(line, &groups);
  }
  printf("SUM:%d\n", sum);

  fclose(fp);
  return 0;
}
