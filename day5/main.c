#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 256
#define ARRAY_LENGTH 100
#define NUM_ACTIONS 7
#define NUM_TYPES 3
#define DATA_TYPE long

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (b) : (a))

typedef enum {
  SEED_TO_SOIL = 0,
  SOIL_TO_FERTILIZER,
  FERTILIZER_TO_WATER,
  WATER_TO_LIGHT,
  LIGHT_TO_TEMPERATURE,
  TEMPERATURE_TO_HUMIDITY,
  HUMIDITY_TO_LOCATION,
} Action;

typedef enum { DESTINATION = 0, SOURCE, RANGE } Type;

typedef struct {
  DATA_TYPE data[ARRAY_LENGTH];
  int size;
} Vector;

void vector_append(Vector *vec, long number) {
  vec->data[vec->size++] = number;
}

DATA_TYPE map(Vector *data, DATA_TYPE source, Action action);

DATA_TYPE find_min_location_range(Vector *data, DATA_TYPE initial_seed,
                                  DATA_TYPE range);

typedef struct thread_data {
  Vector *data;
  DATA_TYPE initial_seed;
  DATA_TYPE range;
  DATA_TYPE result;
} Thread_Data;

void *thread_worker(void *arg) {
  Thread_Data *tdata = (Thread_Data *)arg;
  tdata->result =
      find_min_location_range(tdata->data, tdata->initial_seed, tdata->range);
  pthread_exit(NULL);
}

int main() {
  char *filename = "input.txt";
  FILE *fp = fopen(filename, "r");
  char *token;

  char line[LINE_LENGTH];

  Vector data[NUM_ACTIONS][NUM_TYPES] = {0};
  Vector seeds = {0};

  // parse seeds
  while (fgets(line, sizeof(line), fp)) {
    line[strcspn(line, "\n")] = 0;

    if (line[0] == '\0')
      break;
    token = strtok(line, " ");
    token = strtok(NULL, " ");
    while (token != NULL) {
      vector_append(&seeds, atol(token));
      token = strtok(NULL, " ");
    }
  }

  for (int i = 0; i < NUM_ACTIONS; i++) {
    fgets(line, sizeof(line), fp); // skip first line
    while (fgets(line, sizeof(line), fp)) {
      line[strcspn(line, "\n")] = 0;
      if (line[0] == '\0')
        break;
      token = strtok(line, " ");
      for (int j = 0; token != NULL; j++) {
        vector_append(&data[i][j], atoll(token));
        token = strtok(NULL, " ");
      }
    }
  }
  DATA_TYPE min_location = 0;

  for (int i = 0; i < seeds.size; i++) {
    DATA_TYPE result = seeds.data[i];
    for (int j = 0; j < NUM_ACTIONS; j++) {
      result = map(&data[0][0], result, j);
    }
    if (min_location == 0)
      min_location = result;
    else
      min_location = MIN(result, min_location);
  }

  printf("Min location: %ld\n", min_location);

  // min_location = 0xFFFFFFFFFFFFFF;
  // for (int i = 0; i < seeds.size; i += 2) {
  //   printf("Seed Start: %ld Range: %ld\n", seeds.data[i], seeds.data[i + 1]);
  //   DATA_TYPE local_min =
  //       find_min_location_range(&data[0][0], seeds.data[i], seeds.data[i +
  //       1]);
  //   min_location = MIN(local_min, min_location);
  // }
  // printf("Min location (range): %ld\n", min_location);

  int size = seeds.size / 2;
  Thread_Data data_threads[20] = {0};
  pthread_t threads[20];

  for (int i = 0; i < seeds.size; i += 2) {
    data_threads[i / 2].initial_seed = seeds.data[i];
    data_threads[i / 2].range = seeds.data[i + 1];
    data_threads[i / 2].data = &data[0][0];
    pthread_create(&threads[i / 2], NULL, (void *)&thread_worker,
                   (void *)&data_threads[i / 2]);
  }

  for (int i = 0; i < size; i++) {
    pthread_join(threads[i], NULL);
  }

  min_location = 0xFFFFFFFFFFFFFF;
  for (int i = 0; i < size; i++) {
    min_location = MIN(data_threads[i].result, min_location);
  }
  printf("Min location (range): %ld\n", min_location);

  return 0;
}

DATA_TYPE find_min_location_range(Vector *data, DATA_TYPE initial_seed,
                                  DATA_TYPE range) {
  DATA_TYPE min_location = 0XFFFFFFFFFFFFFF;
  for (int i = 0; i < range; i++) {
    DATA_TYPE result = initial_seed + i;
    for (int j = 0; j < NUM_ACTIONS; j++) {
      result = map(data, result, j);
    }
    min_location = MIN(result, min_location);
  }
  return min_location;
}

DATA_TYPE map(Vector *data, DATA_TYPE source, Action action) {
  Vector *src = &data[action * NUM_TYPES + SOURCE];
  Vector *dst = &data[action * NUM_TYPES + DESTINATION];
  Vector *rng = &data[action * NUM_TYPES + RANGE];
  for (int i = 0; i < src->size; i++) {
    if (source >= src->data[i] && source < src->data[i] + rng->data[i])
      return dst->data[i] + source - src->data[i];
  }
  return source;
}
