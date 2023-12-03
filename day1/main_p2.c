#include <stdio.h>
#include <string.h>

#define FILENAME "input.txt"
#define MAX_LINE 512

char *map_str(int i) {
  static char *map[] = {"zero", "one", "two",   "three", "four",
                        "five", "six", "seven", "eight", "nine"};
  return map[i];
}
char map_dig(int i) { return i + '0'; }

int first_last(char *str) {
  int first = -1, last = -1;
  do {
    if (*str < '0' || *str > '9')
      continue;
    if (first == -1)
      first = *str - '0';
    else
      last = *str - '0';
  } while (*(str++) != '\0');
  if (last == -1)
    last = first;
  return first * 10 + last;
}

void replace(char *str) {
  char buffer[MAX_LINE];
  char *orig, rep, *p, *q;

  for (int i = 1; i < 10; i++) {
    orig = map_str(i);
    rep = map_dig(i);
    q = str;
    while (1) {
      p = strstr(q, map_str(i));
      if (p == NULL)
        break;
      q = p + 3;
      strncpy(buffer, str, p - str + 1);
      buffer[p - str + 1] = map_dig(i);
      strcpy(&buffer[p - str + 1 + 1], p); // yolo
      strncpy(str, buffer, MAX_LINE);
    }
  }
}

int main() {

  FILE *fp = fopen(FILENAME, "r");
  char line[MAX_LINE];
  int calibration_total = 0;

  while (fgets(line, sizeof(line), fp)) {
    replace(line);
    calibration_total += first_last(line);
  }
  fclose(fp);
  printf("calibration_total = %d\n", calibration_total);

  return 0;
}
