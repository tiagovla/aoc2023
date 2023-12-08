#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 512
#define ARRAY_LENGTH 1000

typedef struct Node Node;
typedef char Tag[4];

int compare_tags(Tag tag1, Tag tag2) { return strncmp(tag1, tag2, 3); }

struct Node {
  Tag name;
  Node *parent;
  Node *right;
  Node *left;
};

typedef struct {
  Node *data;
  unsigned int size;
  unsigned int capacity;
} Vector;

void insert_new(Vector *vector, char *name) {
  // change capacity if needed
  if (vector->size == vector->capacity) {
    if (vector->capacity == 0) {
      vector->capacity = 1;
      vector->data = malloc(vector->capacity * sizeof(Node));
    } else {
      vector->capacity *= 2;
      vector->data = realloc(vector->data, vector->capacity * sizeof(Node));
      // trusting this won't fail, yolo
    }
  }
  // move nodes to the right and find correct index
  int i = vector->size - 1;
  while (i >= 0 && strncmp(vector->data[i].name, name, 3) > 0) {
    vector->data[i + 1] = vector->data[i];
    i--;
  }
  // insert node
  Node *current = &vector->data[i + 1];
  strncpy(current->name, name, 3);
  current->parent = NULL;
  current->left = current;
  current->right = current;
  vector->size++;
}

Node *find_node(Vector *vector, char *name) {
  int left = 0;
  int right = vector->size - 1;
  int middle;
  int comparison;
  while (left <= right) {
    middle = (left + right) / 2;
    comparison = strncmp(vector->data[middle].name, name, 3);
    if (comparison == 0)
      return &vector->data[middle];
    else if (comparison < 0)
      left = middle + 1;
    else
      right = middle - 1;
  }
  return NULL;
}

int main() {

  char *filename = "input.txt";
  FILE *fp = fopen(filename, "r");
  char *token;
  char directions[LINE_LENGTH];
  char line[LINE_LENGTH];

  Vector nodes = {0};
  fgets(directions, sizeof(directions), fp);

  fgets(line, sizeof(line), fp); // skip line

  unsigned long position = ftell(fp);
  fflush(fp);

  while (fgets(line, sizeof(line), fp)) {
    *strchr(line, ' ') = '\0';
    insert_new(&nodes, line);
  }

  fseek(fp, position, SEEK_SET);

  while (fgets(line, sizeof(line), fp)) {
    Tag parent = {0}, left = {0}, right = {0};
    sscanf(line, "%3s = (%3s, %3s)", parent, left, right);
    Node *parent_node = find_node(&nodes, parent);
    parent_node->left = find_node(&nodes, left);
    parent_node->right = find_node(&nodes, right);
  }

  int n_directions = strlen(directions);
  unsigned long steps = 0;
  Node *first = &nodes.data[0];
  printf("First node: %3s\n", first->name);
  Node *last = &nodes.data[nodes.size - 1];
  printf("Last node: %3s\n", last->name);
  Node *current = first;

  while (1) {
    unsigned long index = steps % (n_directions - 1);
    printf("Node: %3s (%3s, %3s), Direction: %c \n", current->name,
           current->left->name, current->right->name, directions[index]);
    if (directions[index] == 'L')
      current = current->left;
    else
      current = current->right;
    steps++;
    if (steps % 10000000 == 0)
      printf("%ld\n", steps);

    if (current == last)
      break;
  }

  printf("STEPS: %ld\n", steps);

  free(nodes.data);

  fclose(fp);
  return 0;
}
