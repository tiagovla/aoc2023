#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 256
#define MAX_GALAXIES 500
#define OFFSET 1000000-1
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

typedef struct {
   long long int x;
   long long int y;
} Galaxy;

 long long int abs_diff( long long int a,
                                 long long int b) {
  return a > b ? a - b : b - a;
}

typedef struct {
  Galaxy galaxies[MAX_GALAXIES];
  long long int size;
} GalaxyList;

void add_galaxy(GalaxyList *galaxy_list, Galaxy galaxy) {
  galaxy_list->galaxies[galaxy_list->size++] = galaxy;
}

 long long int distance(Galaxy galaxy1, Galaxy galaxy2) {
  return abs_diff(galaxy1.x, galaxy2.x) + abs_diff(galaxy1.y, galaxy2.y);
  return llabs(galaxy1.x-galaxy2.x) + llabs(galaxy1.y- galaxy2.y);
}

int main() {

  FILE *fp = fopen("input.txt", "r");
  char line[LINE_LENGTH];
  GalaxyList galaxy_list = {0};

  // read galaxies and universe size
  long long int max_x = 0, max_y = 0;
  for (long long int i = 0; fgets(line, sizeof(line), fp); i++) {
    for (long long int j = 0; line[j] != '\0'; j++) {
      if (line[j] == '#') {
        Galaxy galaxy = {i, j};
        add_galaxy(&galaxy_list, galaxy);
        max_x = MAX(max_x, i + 1);
        max_y = MAX(max_y, j + 1);
      }
    }
  }

  // expand the universe vertically
  for (int i = max_x - 1; i >= 0; i--) {
    char contains_x = 0;
    for (int j = 0; j < galaxy_list.size; j++) {
      if (galaxy_list.galaxies[j].x == i)
        contains_x = 1;
    }
    if (!contains_x) {
      max_x += OFFSET;
      for (int j = 0; j < galaxy_list.size; j++) {
        if (galaxy_list.galaxies[j].x > i)
          galaxy_list.galaxies[j].x += OFFSET;
      }
    }
  }
  for (int i = max_y - 1; i >= 0; i--) {
    char contains_y = 0;
    for (int j = 0; j < galaxy_list.size; j++) {
      if (galaxy_list.galaxies[j].y == i)
        contains_y = 1;
    }
    if (!contains_y) {
      max_y += OFFSET;
      for (long long int j = 0; j < galaxy_list.size; j++) {
        if (galaxy_list.galaxies[j].y > i)
          galaxy_list.galaxies[j].y += OFFSET;
      }
    }
  }

  long long int total_distance = 0;
  for (int i = 0; i < galaxy_list.size; i++) {
    for (int j = i + 1; j < galaxy_list.size; j++) {
      long long int dist =
          distance(galaxy_list.galaxies[i], galaxy_list.galaxies[j]);
      total_distance += dist;
    }
  }
  printf("Total distance: %lld\n", total_distance);

  fclose(fp);
  return 0;
}
