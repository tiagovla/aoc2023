#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 256
#define ARRAY_LENGTH 1000

typedef union {
  struct {
    short unsigned int bet;
    char cards[5];
    char type;
  };
  unsigned long long data;
} Hand;

typedef struct {
  Hand data[ARRAY_LENGTH];
  int size;
} Vector;

char card_to_id(char card) {
  switch (card) {
  case 'T':
    return 10;
  case 'J':
    return 1;
  case 'Q':
    return 12;
  case 'K':
    return 13;
  case 'A':
    return 14;
  default:
    return card - '0';
  }
}

typedef enum {
  HIGH_CARD = 0,
  ONE_PAIR = 1,
  TWO_PAIR = 2,
  THREE_OF_A_KIND = 3,
  FULL_HOUSE = 5,
  FOUR_OF_A_KIND = 14,
  FIVE_OF_A_KIND = 15
} Cards;

Hand get_new_rule_value(Hand hand) {
  char jokers = 0;

  for (int i = 0; i < 5; i++)
    if (hand.cards[i] == 1)
      jokers++;

  if (jokers == 0)
    return hand;

  switch (hand.type) {
  case FOUR_OF_A_KIND:
    hand.type = FIVE_OF_A_KIND;
    break;
  case THREE_OF_A_KIND:
    hand.type = FOUR_OF_A_KIND;
    break;
  case FULL_HOUSE:
    hand.type = FIVE_OF_A_KIND;
    break;
  case TWO_PAIR:
    if (jokers == 1)
      hand.type = FULL_HOUSE;
    if (jokers == 2)
      hand.type = FOUR_OF_A_KIND;
    break;
  case ONE_PAIR:
    hand.type = THREE_OF_A_KIND;
    break;
  case HIGH_CARD:
    hand.type = ONE_PAIR;
    break;
  }
  return hand;
}

Hand get_hand_value(char *str) {
  Hand hand = {0};
  char count[15] = {0};
  char score = 0;

  for (int i = 0; i < 5; i++) {
    hand.cards[i] = card_to_id(str[i]);
  }

  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 5; j++) {
      if (hand.cards[j] == i) {
        count[i]++;
      }
    }
  }

  for (int i = 0; i < 15; i++) {
    switch (count[i]) {
    case 5:
      score = FIVE_OF_A_KIND;
      break;
    case 4:
      score = FOUR_OF_A_KIND;
      break;
    case 3:
      score += THREE_OF_A_KIND;
      break;
    case 2:
      score += ONE_PAIR;
      break;
    }
  }

  if (score == 4)
    hand.type = FULL_HOUSE;
  else
    hand.type = score;

  return hand;
}

int compare_hands(Hand a, Hand b) {
  if (a.type > b.type)
    return 1;
  if (a.type < b.type)
    return -1;
  for (int i = 0; i < 5; i++) {
    if (a.cards[i] > b.cards[i])
      return 1;
    if (a.cards[i] < b.cards[i])
      return -1;
  }
  return 0;
}

void insert_sort(Vector *vec, Hand hand) {
  int i = vec->size - 1;
  while (i >= 0 && compare_hands(vec->data[i], hand) > 0) {
    vec->data[i + 1] = vec->data[i];
    i--;
  }
  vec->data[i + 1] = hand;
  vec->size++;
}

int main() {
  char *filename = "input.txt";
  FILE *fp = fopen(filename, "r");
  char *token;
  char line[LINE_LENGTH];
  Vector hands = {0};

  while (fgets(line, sizeof(line), fp)) {
    char *delimiter = strchr(line, ' ');
    *(delimiter++) = '\0';
    Hand hand = {0};
    hand.bet = atoi(delimiter);
    hand.data |= get_hand_value(line).data;
    hand.data = get_new_rule_value(hand).data;
    insert_sort(&hands, hand);
  }

  int score = 0;
  for (int rank = 1; rank <= hands.size; rank++)
    score += rank * hands.data[rank - 1].bet;
  printf("SCORE: %d\n", score);

  fclose(fp);

  return 0;
}
