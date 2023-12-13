#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 128
#define ARRAY_LENGTH 56

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef struct {
  int data[ARRAY_LENGTH];
  int size;
} Array;

void array_append(Array *array, int value) {
  array->data[array->size++] = value;
}

int array_compare_partial(Array *a, Array *b) {
  int size = MIN(a->size, b->size);
  for (int i = 0; i < size; i++) {
    if (a->data[i] > b->data[i])
      return 1;
    else if (a->data[i] < b->data[i])
      return -1;
  }
  return 0;
}

void array_print(Array *array) {
  printf("(");
  for (int i = 0; i < array->size; i++) {
    printf("%d,", array->data[i]);
  }
  printf(")");
}

void count_partial_hashtag(char *str, Array *result) {
  result->size = 0;
  char *question_mark = strchr(str, '?');
  while (str != question_mark && *str != '\0') {
    if (*str == '#') {
      int count = 0;
      while (*str == '#') {
        count++;
        str++;
      }
      array_append(result, count);
    } else
      str++;
  }
}

unsigned long int process_combinations(char *str, Array *target_tuple) {
  unsigned long int count = 0;
  static Array partial_tuple = {0};
  count_partial_hashtag(str, &partial_tuple);
  int comp = array_compare_partial(&partial_tuple, target_tuple);
  char *question_mark = strchr(str, '?');
  if (comp == 1) {
    return 0; // (1,4,3,) > (1,3,3,3);
  } else if (comp == 0 && target_tuple->size == partial_tuple.size) {
    return 1; // (1,4,3,) == (1,4,3,)
  } else if (question_mark == NULL) {
    return 0;
  }

  *question_mark = '#';
  count += process_combinations(str, target_tuple);
  if (comp >= 0) { // quit early skip (1,4,3,2,2) if target is (1,4,4)
    *question_mark = '.';
    count += process_combinations(str, target_tuple);
  }
  *question_mark = '?';
  return count;
}

int main() {
  FILE *fp = fopen("input.txt", "r");

  char line[LINE_LENGTH];
  unsigned long int sum = 0;

  while (fgets(line, sizeof(line), fp)) {
    printf("%s", line);

    // parse line/target_tuple
    char *delimiter = strchr(line, ' ');
    Array target_tuple = {0};
    *delimiter = '\0';
    char *token = strtok(delimiter + 1, ",");
    for (int i = 0; token != NULL; i++) {
      array_append(&target_tuple, atoi(token));
      token = strtok(NULL, ",");
    }

    // target_tuple*5 and line*5
    int size = target_tuple.size;
    for (int i = 0; i < 5 - 1; i++)
      for (int j = 0; j < size; j++)
        array_append(&target_tuple, target_tuple.data[j]);

    size = strlen(line);
    int pos = size;
    for (int i = 0; i < 5 - 1; i++) {
      line[pos++] = '?';

      for (int j = 0; j < size; j++)
        line[pos++] = line[j];
    }
    line[pos++] = '\0';

    sum += process_combinations(line, &target_tuple);
  }
  printf("SUM:%lu\n", sum);

  fclose(fp);
  return 0;
}
