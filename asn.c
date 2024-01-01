
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
   
const int SEC = 1E6, LEVEL = 8; // 0 < LEVEL <= SEC
const int n = 20;
const int sx = 1, sy = 2;
const int dx[] = {0, 1, 0, -1};
const int dy[] = {1, 0, -1, 0};
const int keys[] = {KEY_RIGHT, KEY_DOWN, KEY_LEFT, KEY_UP};

int fx, fy, dir, score;

void draw_square(WINDOW* win) {
  for (int i = 0; i < n; i++) {
    mvwaddstr(win, i * sx, 0 * sy, "#");
  }
  for (int i = 0; i < n; i++) {
    mvwaddstr(win, i * sx, (n - 1) * sy, "#");
  }
  for (int j = 1; j < n - 1; j++) {
    mvwaddstr(win, 0 * sx, j * sy, "#");
  }
  for (int j = 1; j < n - 1; j++) {
    mvwaddstr(win, (n - 1) * sx, j * sy, "#");
  }
}

#define L 255
int bx[L + 1], by[L + 1];
int head, length;

void draw_buffer(WINDOW* win) {
  mvwaddstr(win, fx * sx, fy * sy, score % (10 * LEVEL) == (5 * LEVEL) ? "$" : "*");
  int x = bx[head] * sx;
  int y = by[head] * sy;
  mvwaddstr(win, x, y, "@");
  for (int j = 1; j < length; j++) {
    mvwaddstr(win, bx[(head + j) & L] * sx, by[(head + j) & L] * sy, "O");
  }
}

bool inside(int i, int j) {
  if (i <= 0) return 0;
  if (i >= n - 1) return 0;
  if (j <= 0) return 0;
  if (j >= n - 1) return 0;
  return 1;
}

#define R 24
char result[R] = "Score:";

void update_score() {
  int x = score;
  if (x == 0) {
    result[R - 4] = '0';
  }
  for (int i = R - 4; x > 0; i--) {
    result[i] = (char) (48 + x % 10);
    x /= 10;
  }
}

void update_food() {
  int bad; 
  do {
    fx = rand() % (n - 2) + 1;
    fy = rand() % (n - 2) + 1;
    bad = 0;
    for (int j = 0; j < length; j++) {
      int i = (head + j) & L;
      bad |= bx[i] == fx && by[i] == fy;
    }
  } while (bad);
}

bool update_snake() {
  int h = head;
  if (--head < 0) {
    head = L;
  }
  bx[head] = bx[h] + dx[dir];
  by[head] = by[h] + dy[dir];
  for (int j = 1; j < length; j++) {
    int i = (head + j) & L;
    if (bx[i] == bx[head] && by[i] == by[head]) {
      return 0;
    }
  }
  int tail = (head + length) & L;
  return inside(bx[head], by[head]);
}

int main() {
  srand(97);
  WINDOW* win = initscr();
  keypad(win, true);
  nodelay(win, true);  
  while (true) {
    for (int i = 6; i < R - 1; i++) {
      result[i] = ' ';
    }
    result[R - 1] = 0;
    for (int i = 0; i <= L; i++) {
      bx[i] = -1;
      by[i] = -1;
    }
    length = 3;
    head = 0;
    for (int i = 0; i < length; i++) {
      bx[i] = n / 2;
      by[i] = length - i - 1;
    }
    dir = 0;
    score = 0;
    update_food();
    update_score();
    while (true) {
      int key = wgetch(win);
      if (key == keys[(dir + 1) & 3]) {
        dir = (dir + 1) & 3;
      }
      if (key == keys[(dir + 3) & 3]) {
        dir = (dir + 3) & 3;
      }
      if (!update_snake()) {
        break;
      }
      erase();
      draw_square(win);
      if (bx[head] == fx && by[head] == fy) {
        update_food();
        if (score % (10 * LEVEL) == (5 * LEVEL)) {
          score += 5 * LEVEL;
        } else {
          score += LEVEL;
          length += 1;
        }
        update_score();
      }
      draw_buffer(win);
      mvwaddstr(win, n * sx, 0, result);
      usleep(SEC / LEVEL); 
    }
    mvwaddstr(win, n * sx / 2 - 2, 5, "  GAME OVER!   ");
    mvwaddstr(win, n * sx / 2 - 1, 5, "  ");
    mvwaddstr(win, n * sx / 2 - 1, 7, result);
    mvwaddstr(win, n * sx / 2 + 1, 3, " Press RIGHT_ARROW to play again!");
    mvwaddstr(win, n * sx / 2 + 2, 3, " Press LEFT_ARROW to quit game!");
    mvwaddstr(win, n * sx, R, "");
    while (wgetch(win) == ERR) {}
    int que;
    do {
      que = wgetch(win);
      if (que == KEY_LEFT || que == KEY_RIGHT) {
        break;
      }
    } while (true);
    if (que == KEY_LEFT) {
      break;
    }
  }
  endwin();
  return 0;
}

