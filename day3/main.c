#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef struct {
  char *data;
  unsigned int cols;
  unsigned int rows;
} Data;

Data read_file(char *filename) {
  FILE *fp = fopen(filename, "r");
  unsigned int cols = 0, rows;

  fseek(fp, 0, SEEK_END);
  long f_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char *arr = malloc(f_size + 1);
  fread(arr, f_size, 1, fp);

  fclose(fp);

  for (int i = 0; arr[i] != '\n'; i++) {
    cols++;
  }
  rows = f_size / (cols + 1);

  Data data = {arr, cols, rows};
  return data;
}

char get_char(Data *data, int row, int col) {
  return data->data[row * (data->cols + 1) + col];
}

int check_if_digit(char current_char) {
  if (current_char >= '0' && current_char <= '9') {
    return 1;
  }
  return 0;
}

int check_if_symbol(char current_char) {
  if (check_if_digit(current_char) || current_char == '.' ||
      current_char == '\n' || current_char == '\0') {
    return 0;
  }
  if (current_char == '*') {
    return 2;
  }
  return 1;
}

int check_surroundings(Data *data, int row, int col_start, int col_end) {
  int start_row = MAX(row - 1, 0);
  int end_row = MIN(row + 1, data->rows);
  int start_col = MAX(col_start - 1, 0);
  int end_col = MIN(col_end + 1, data->cols);

  int ret = 0;
  for (int i = start_row; i <= end_row; i++) {
    for (int j = start_col; j < end_col; j++) {
      char ch = get_char(data, i, j);
      ret = MAX(ret, check_if_symbol(ch));
      if (ret == 2)
        return i * 1000 + j; // this works as an id
    }
  }
  return ret;
}

int main() {
  int arr_key[1000] = {0};
  int arr_values[1000] = {0};
  int arr_index = 0;

  int sum = 0;
  char *filename = "input.txt";
  Data data = read_file(filename);
  char current_char;
  int j_first, k1_end;
  int i, j, k;
  for (i = 0; i < data.rows; i++) {
    for (j = 0; j < data.cols; j++) {
      current_char = get_char(&data, i, j);
      int res = check_if_digit(current_char);
      if (!res)
        continue;
      for (k = j + 1;; k++) {
        current_char = get_char(&data, i, k);
        int res = check_if_digit(current_char);
        if (!res)
          break;
      }
      int ress = check_surroundings(&data, i, j, k);
      char buffer[50];
      memset(buffer, 0, sizeof(buffer));
      int ii = 0;
      for (int jj = j; jj < k; jj++, ii++)
        buffer[ii] = get_char(&data, i, jj);
      int num = atoi(buffer);
      if (ress) {
        sum += num;
      }

      int vall = check_surroundings(&data, i, j, k);
      if (vall > 100) {
        arr_key[arr_index] = vall;
        arr_values[arr_index] = num;
        arr_index++;
      }

      j = k;
    }
  }

  for (int i = 0; i < arr_index; i++) {
    int n = 0;
    for (int j = 0; j < arr_index; j++) {
      int key1 = arr_key[i];
      int key2 = arr_key[j];
      if (key1 == key2) {
        n++;
      }
      if (n > 2) {
        for (int k = 0; k < arr_index; k++) {
          if (arr_key[k] == key1) {
            arr_values[k] = 0;
          }
        }
      }
    }
  }

  int part_sum = 0;
  for (int i = 0; i < arr_index; i++) {
    for (int j = 0; j < arr_index; j++) {
      if (i == j)
        continue;
      int key1 = arr_key[i];
      int key2 = arr_key[j];
      if (key1 == key2) {
        part_sum += arr_values[j] * arr_values[i];
      }
    }
  }

  printf("SUM: %d\n", sum);
  printf("GEAR SUM: %d\n", part_sum / 2);

  free(data.data);

  return 0;
}
