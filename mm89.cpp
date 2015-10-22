#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cassert>
#include <sys/time.h>
using namespace std;
#define REP(i, n) for (int i = 0; i < (int)(n); ++i)
#define DEBUG(x) cerr << #x << " = " << x << endl

double GetTime() {
  timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

unsigned long xrand() {
  static unsigned long x = 123456789, y = 362436069, z = 521288629, w = 88675123;
  unsigned long t;
  t = (x ^ (x << 11));
  x = y;
  y = z;
  z = w;
  return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

struct MazeFixing {
  const vector<string> M;
  const int F;
  const int H, W;
  const int dy[4] = { 0, -1, 0, 1 };
  const int dx[4] = { 1, 0, -1, 0 };
  vector<vector<bool>> is_border;
  vector<vector<bool>> visited_one_path;
  vector<vector<bool>> visited_over_all;
  MazeFixing(vector<string> M_, int F_) : M(M_), F(F_), H(M_.size()), W(M_[0].size()) {}

  bool IsOutside(int y, int x) {
    if (y < 0 || y >= H || x < 0 || x >= W)
      return true;
    if (M[y][x] == '.')
      return true;
    return false;
  }

  void Initialize() {
    is_border.assign(H, vector<bool>(W, false));
    REP(y, H) REP(x, W) {
      if (M[y][x] == '.') {
        REP(i, 4) {
          if (IsOutside(y + dy[i], x + dx[i]) == false) {
            is_border[y][x] = true;
          }
        }
      }
    }
  }

  void FillVisitedOverAll() {
    REP(y, H) REP(x, W) {
      visited_over_all[y][x] = visited_over_all[y][x] || visited_one_path[y][x];
    }
  }

  void Follow(const vector<string> &maze, int y, int x, int d) {
    if (visited_one_path[y][x]) {
      // check loop
      return;
    }
    if (IsOutside(y, x) == false) {
      visited_one_path[y][x] = true;
    }
    if (is_border[y][x]) {
      // goal
      FillVisitedOverAll();
    } else {
      if (maze[y][x] == 'S') {
        int nd = (d + 0) % 4;
        Follow(maze, y + dy[nd], x + dx[nd], nd);
      } else if (maze[y][x] == 'L') {
        int nd = (d + 1) % 4;
        Follow(maze, y + dy[nd], x + dx[nd], nd);
      } else if (maze[y][x] == 'U') {
        int nd = (d + 2) % 4;
        Follow(maze, y + dy[nd], x + dx[nd], nd);
      } else if (maze[y][x] == 'R') {
        int nd = (d + 3) % 4;
        Follow(maze, y + dy[nd], x + dx[nd], nd);
      } else if (maze[y][x] == 'E') {
        REP(nd, 4) {
          Follow(maze, y + dy[nd], x + dx[nd], nd);
        }
      } else {
        assert(false);
      }
    }
    visited_one_path[y][x] = false;
  }

  int GetScore(const vector<string> &maze) {
    visited_one_path.assign(H, vector<bool>(W, false));
    visited_over_all.assign(H, vector<bool>(W, false));
    REP(y, H) REP(x, W) {
      if (is_border[y][x]) {
        REP(i, 4) {
          if (IsOutside(y + dy[i], x + dx[i]) == false && M[y + dy[i]][x + dx[i]] != '.') {
            Follow(maze, y + dy[i], x + dx[i], i);
          }
        }
      }
    }
    int c = 0;
    REP(y, H) REP(x, W) {
      if (visited_over_all[y][x])
        ++c;
    }
    return c;
  }

  vector<string> Optimize() {
    double time_start = GetTime();
    double time_elapsed = GetTime() - time_start;
  }

  vector<string> Improve() {
    Initialize();
    return Optimize();
  }
};

vector<string> improve(vector<string> maze, int F) {
  return MazeFixing(maze, F).Improve();
}

int main() {
  // height: [10, 80]
  int H;
  cin >> H;
  // width: [10, 80]
  vector<string> maze(H);
  REP(i, H) cin >> maze[i];
  // fixes: [N/10, N/3], N is the number of non '.' cells in the maze
  int F;
  cin >> F;
  vector<string> ret = improve(maze, F);
  cout << ret.size() << endl;
  REP(i, ret.size()) {
    cout << ret[i] << endl;
  }
  cout.flush();
}
