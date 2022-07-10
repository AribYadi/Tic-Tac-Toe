#include <stdio.h>
#include <stdbool.h>
#include <conio.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define TABLE_SIZE 3

const char table_chars[3] = ".XO";
static int table[TABLE_SIZE * TABLE_SIZE];

static int cur_x = 0, cur_y = 0;
static int player = 0;

void go_back(void) {
  fputc('\r', stdout);
  printf("\x1b[%dA", TABLE_SIZE + 1);
}

void print_table(void) {
  for (int y = 0; y < TABLE_SIZE; ++y) {
    for (int x = 0; x < TABLE_SIZE; ++x) {
      if (cur_x == x && cur_y == y) printf("[%c]", table_chars[table[y * TABLE_SIZE + x]]);
      else printf(" %c ", table_chars[table[y * TABLE_SIZE + x]]);
    }
    fputc('\n', stdout);
  }
}

void confirm_selection(void) {
  int *state = &table[cur_y * TABLE_SIZE + cur_x];
  if (*state != 0) return;
  *state = player + 1;
  player = 1 - player;
}

bool prompt(void) {
  printf("> ");
  char inp = getch();
  fputc('\n', stdout);

  switch (inp) {
    case 'l':
    case 'd':
      cur_x += 1;
      break;
    case 'h':
    case 'a':
      cur_x -= 1;
      break;
    case 'k':
    case 'w':
      cur_y -= 1;
      break;
    case 'j':
    case 's':
      cur_y += 1;
      break;

    case 'q':
    case 'c':
    case 27:
    case 3:
      return false;

    case ' ':
    case 13:
      confirm_selection();
      break;

    default:
      break;
  };

  cur_x = MAX(0, MIN(TABLE_SIZE - 1, cur_x));
  cur_y = MAX(0, MIN(TABLE_SIZE - 1, cur_y));

  return true;
}

bool check_winner(void) {
  int prev_player = 1 - player;

  // Check rows and cols
  for (int i = 0; i < TABLE_SIZE; ++i) {
    bool won = true;

    // Check `i` as y
    for (int x = 0; x < TABLE_SIZE; ++x) {
      if (table[i * TABLE_SIZE + x] - 1 != prev_player) {
        won = false;
        break;
      }
    }
    if (won) return true;
    // Check `i` as x
    for (int y = 0; y < TABLE_SIZE; ++y) {
      if (table[y * TABLE_SIZE + i] - 1 != prev_player) {
        won = false;
        break;
      }
    }
    if (won) return true;
  }

  // Check diagonals
  bool won = true;
  for (int i = 0; i < TABLE_SIZE; ++i) {
    if (table[i * TABLE_SIZE + i] - 1 != prev_player) {
      won = false;
      break;
    }
  }
  if (won) return true;
  for (int i = 0; i < TABLE_SIZE; ++i) {
    if (table[i * TABLE_SIZE + (TABLE_SIZE - i - 1)] - 1 != prev_player) {
      won = false;
      break;
    }
  }
  if (won) return true;

  return false;
}

bool is_full(void) {
  for (int y = 0; y < TABLE_SIZE; ++y) {
    for (int x = 0; x < TABLE_SIZE; ++x) {
      if (table[y * TABLE_SIZE + x] == 0) return false;
    }
  }
  return true;
}

int main(void) {
  bool play = true;
  while (play) {
    // Initialize variables
    for (int i = 0; i < TABLE_SIZE * TABLE_SIZE; ++i) table[i] = 0;
    cur_x = 0, cur_y = 0;
    player = 0;

    print_table();
    while (!check_winner() && !is_full()) {
      if (!prompt()) {
        play = false;
        break;
      }
      go_back();
      print_table();
    }

    if (check_winner()) {
      print_table();
      printf("=> Player %d has won!\n", 1 - player + 1);
      printf("=> Play Again? (y/N)\n");
      printf("> ");
      char ch = getch();
      fputc('\n', stdout);

      if (ch == 'Y' || ch == 'y') continue;
      else break;
    } else if (is_full()) {
      print_table();
      printf("=> Tie!\n");
      printf("=> Play Again? (y/N)\n");
      printf("> ");
      char ch = getch();
      fputc('\n', stdout);

      if (ch == 'Y' || ch == 'y') continue;
      else break;
    }
  }

  return 0;
}
