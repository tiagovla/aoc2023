#include <stdio.h>

#define FILENAME "input.txt"
#define MAX_LINE 256

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

int main() {

  FILE *fp = fopen(FILENAME, "r");
  char line[MAX_LINE];
  int calibration_total = 0;

  while (fgets(line, sizeof(line), fp)) {
    calibration_total += first_last(line);
  }
  fclose(fp);
  printf("calibration_total = %d\n", calibration_total);

  return 0;
}
