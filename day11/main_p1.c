#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 256
#define MAX_GALAXIES 500
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

typedef struct {
  int x;
  int y;
} Galaxy;

typedef struct {
  Galaxy galaxies[MAX_GALAXIES];
  int size;
} GalaxyList;

void add_galaxy(GalaxyList *galaxy_list, Galaxy galaxy) {
  galaxy_list->galaxies[galaxy_list->size++] = galaxy;
}

int distance(Galaxy galaxy1, Galaxy galaxy2) {
  return abs(galaxy1.x - galaxy2.x) + abs(galaxy1.y - galaxy2.y);
}

int main() {

  FILE *fp = fopen("input.txt", "r");
  char line[LINE_LENGTH];
  GalaxyList galaxy_list = {0};

  // read galaxies and universe size
  int max_x = 0, max_y = 0;
  for (int i = 0; fgets(line, sizeof(line), fp); i++) {
    for (int j = 0; line[j] != '\0'; j++) {
      if (line[j] == '#') {
        Galaxy galaxy = {i, j};
        add_galaxy(&galaxy_list, galaxy);
        max_x = MAX(max_x, i + 1);
        max_y = MAX(max_y, j + 1);
      }
    }
  }
  printf("BEFORE EXPANSION\n");
  printf("MAX: %d, %d\n", max_x, max_y);
  for (int i = 0; i < galaxy_list.size; i++) {
    printf("Galaxy %d: (%d, %d)\n", i + 1, galaxy_list.galaxies[i].x,
           galaxy_list.galaxies[i].y);
  }

  // expand the universe vertically
  for (int i = max_x - 1; i >= 0; i--) {
    int contains_x = 0;
    for (int j = 0; j < galaxy_list.size; j++) {
      if (galaxy_list.galaxies[j].x == i)
        contains_x = 1;
    }
    if (!contains_x) {
      max_x++;
      for (int j = 0; j < galaxy_list.size; j++) {
        if (galaxy_list.galaxies[j].x > i)
          galaxy_list.galaxies[j].x++;
      }
    }
  }
  for (int i = max_y - 1; i >= 0; i--) {
    int contains_y = 0;
    for (int j = 0; j < galaxy_list.size; j++) {
      if (galaxy_list.galaxies[j].y == i)
        contains_y = 1;
    }
    if (!contains_y) {
      max_y++;
      for (int j = 0; j < galaxy_list.size; j++) {
        if (galaxy_list.galaxies[j].y > i)
          galaxy_list.galaxies[j].y++;
      }
    }
  }

  printf("AFTER EXPANSION\n");
  printf("MAX: %d, %d\n", max_x, max_y);
  for (int i = 0; i < galaxy_list.size; i++) {
    printf("Galaxy %d: (%d, %d)\n", i + 1, galaxy_list.galaxies[i].x,
           galaxy_list.galaxies[i].y);
  }

  printf("Universe size: %d, %d\n", max_x, max_y);

  char universe[max_x][max_y];
  for (int i = 0; i < max_x; i++) {
    for (int j = 0; j < max_y; j++) {
      universe[i][j] = '.';
    }
  }
  for (int i = 0; i < galaxy_list.size; i++) {
    universe[galaxy_list.galaxies[i].x][galaxy_list.galaxies[i].y] =
        i + 1 + '0';
  }

  // Plot
  // for (int i = 0; i < max_x; i++) {
  //   for (int j = 0; j < max_y; j++) {
  //     printf("%c ", universe[i][j]);
  //   }
  //   printf("\n");
  // }

  int total_distance = 0;
  for (int i = 0; i < galaxy_list.size; i++) {
    for (int j = i + 1; j < galaxy_list.size; j++) {
      int dist = distance(galaxy_list.galaxies[i], galaxy_list.galaxies[j]);
      // printf("Galaxy %d and %d with distance %d\n", i + 1, j + 1, dist);
      total_distance += dist;
      // printf("Galaxy %d and %d are neighbours\n", i + 1, j + 1);
    }
  }
  printf("Total distance: %d\n", total_distance);
  // int res = distance(galaxy_list.galaxies[0], galaxy_list.galaxies[6]);
  // printf("Distance: %d\n", res);

  fclose(fp);
  return 0;
}
