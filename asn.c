
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
   
const int n = 20, SEC = 1E6;
int x, y, sx = 1, sy = 2, fx, fy, dx, dy, score;

void draw_square(WINDOW* win) {
  for (int i = 0; i < n; i++) mvwaddstr(win, i * sx, 0 * sy, "#");
  for (int i = 0; i < n; i++) mvwaddstr(win, i * sx, (n - 1) * sy, "#");
  for (int j = 1; j < n - 1; j++) mvwaddstr(win, 0 * sx, j * sy, "#");
  for (int j = 1; j < n - 1; j++) mvwaddstr(win, (n - 1) * sx, j * sy, "#");
}

bool inside(int i, int j) {
  if (i <= 0) return 0;
  if (i >= n - 1) return 0;
  if (j <= 0) return 0;
  if (j >= n - 1) return 0;
  return 1;
}

char result[12] = "Score:";
void update_score() {
  if (score % 10 == 5) {
    score += 5;
  } else {
    score += 1;
  }
  int x = score;
  if (x == 0) {
    result[10] = '0';
  }
  for (int i = 10; x > 0; i--) {
    result[i] = (char) (48 + x % 10);
    x /= 10;
  }
}

void update_food() {
  fx = rand() % (n - 2) + 1;
  fy = rand() % (n - 2) + 1;
}

int main() {
  srand(97);
  WINDOW* win = initscr();
  keypad(win, true);
  nodelay(win, true);  
  x = n / 2;
  y = 0;
  dx = 0;
  dy = 1;
  update_food();
  for (int i = 6; i < 12; i++) {
    result[i] = ' ';
  }
  result[11] = 0;
  score = -1;
  update_score();
  for (bool gameOn = 1; gameOn; ) {
    int key = wgetch(win);
    switch (key) {
      case KEY_RIGHT: 
        dx = 0;
        dy = 1;
        break;
      case KEY_DOWN:
        dx = 1;
        dy = 0;
        break;
      case KEY_LEFT:
        dx = 0;
        dy = -1;
        break;
      case KEY_UP:
        dx = -1;
        dy = 0;
        break;
    }
    x += dx;
    y += dy;
    gameOn &= inside(x, y);
    if (x == fx && y == fy) {
      update_food();
      update_score();
    }
    erase();
    draw_square(win);
    mvwaddstr(win, x * sx, y * sy, "o");
    mvwaddstr(win, fx * sx, fy * sy, score % 10 == 5 ? "$" : "*");
    mvwaddstr(win, n * sx, 0, result);
    usleep(SEC / 8); 
  }
  mvwaddstr(win, n * sx / 2, 5, "GAME OVER!");
  mvwaddstr(win, n * sx / 2 + 1, 5, result);
  for (int i = 0; i < 2; i++) {
    while (wgetch(win) == ERR) {}
  }
  endwin();
  return 0;
}

