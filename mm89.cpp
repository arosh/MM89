#ifndef LOCAL
#define NDEBUG
#endif
#include <cassert>
#include <climits>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <deque>
#include <array>
#include <sys/time.h>
#ifdef ENABLE_PROFILE
#include <gperftools/profiler.h>
#endif
using namespace std;
#define REP(i, n) for (int i = 0; i < (int)(n); ++i)
#define DEBUG(x) cerr << #x << " = " << x << endl

double gettime() {
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

typedef array<array<char, 80>, 80> MazeCharType;
typedef array<array<bool, 80>, 80> MazeBoolType;

MazeCharType transform(const vector<string> &vs) {
  MazeCharType ary;
  REP(i, vs.size()) {
    REP(j, vs[i].size()) {
      ary[i][j] = vs[i][j];
    }
  }
  return ary;
}

struct Action {
  int y, x, d, sgn;
  Action() {}
  Action(int y_, int x_, int d_, int sgn_) : y(y_), x(x_), d(d_), sgn(sgn_) {}
};

struct Solver {
  const MazeCharType M;
  const int F;
  const int H, W;
  const int dy[4] = { 0, -1, 0, 1 };
  const int dx[4] = { 1, 0, -1, 0 };
  MazeBoolType is_border;
  MazeBoolType visited_one_path;
  MazeBoolType visited_over_all;
  MazeBoolType visited_any_path;
  Solver(vector<string> M_, int F_) : M(transform(M_)), F(F_), H(M_.size()), W(M_[0].size()) {}

  bool IsOutside(int y, int x) {
    if (y < 0 || y >= H || x < 0 || x >= W)
      return true;
    if (M[y][x] == '.')
      return true;
    return false;
  }

  void FillFalse(MazeBoolType &ary) {
    REP(y, H) {
      REP(x, W) {
        ary[y][x] = false;
      }
    }
  }

  void SetUpBorder() {
    FillFalse(is_border);
    REP(y, H) {
      REP(x, W) {
        if (M[y][x] == '.') {
          REP(i, 4) {
            if (IsOutside(y + dy[i], x + dx[i]) == false) {
              is_border[y][x] = true;
            }
          }
        }
      }
    }
  }

  void FillVisitedOverAll(const Action stk[], const int size) {
    REP(i, size) {
      const Action &a = stk[i];
      if (a.sgn == -1) {
        visited_over_all[a.y][a.x] = true;
      }
    }
  }

  void Follow(const MazeCharType &maze, int start_y, int start_x, int start_d) {
    static Action stk[4 * 80 * 80];
    int stk_size = 0;
    stk[stk_size++] = Action(start_y, start_x, start_d, 1);
    while (stk_size) {
      Action cur = stk[--stk_size];
      int y = cur.y;
      int x = cur.x;
      int d = cur.d;
      if (cur.sgn > 0) {
        if (visited_one_path[y][x]) {
          // check loop
          continue;
        }
        if (is_border[y][x]) {
          // goal
          FillVisitedOverAll(stk, stk_size);
        } else {
          stk[stk_size++] = Action(y, x, d, -1);
          visited_one_path[y][x] = true;
          visited_any_path[y][x] = true;
          assert(IsOutside(y, x) == false);
          int nd = -1;
          switch (maze[y][x]) {
          case 'S':
            nd = (d + 0) & 3;
            break;
          case 'L':
            nd = (d + 1) & 3;
            break;
          case 'U':
            nd = (d + 2) & 3;
            break;
          case 'R':
            nd = (d + 3) & 3;
            break;
          }
          if (nd != -1) {
            stk[stk_size++] = Action(y + dy[nd], x + dx[nd], nd, 1);
          } else /* if (maze[y][x] == 'E') */ {
            stk[stk_size + 0] = Action(y + 0, x + 1, 0, 1);
            stk[stk_size + 1] = Action(y - 1, x + 0, 1, 1);
            stk[stk_size + 2] = Action(y + 0, x - 1, 2, 1);
            stk[stk_size + 3] = Action(y + 1, x + 0, 3, 1);
            stk_size += 4;
          } /* else {
            assert(false);
          } */
        }
      } else {
        visited_one_path[y][x] = false;
      }
    }
  }

  void Search(const MazeCharType &maze) {
    FillFalse(visited_one_path);
    FillFalse(visited_over_all);
    FillFalse(visited_any_path);
    REP(y, H) {
      REP(x, W) {
        if (is_border[y][x]) {
          REP(i, 4) {
            if (IsOutside(y + dy[i], x + dx[i]) == false && M[y + dy[i]][x + dx[i]] != '.') {
              Follow(maze, y + dy[i], x + dx[i], i);
            }
          }
        }
      }
    }
  }

  int GetScore() {
    int c = 0;
    REP(y, H) {
      REP(x, W) {
        if (visited_over_all[y][x])
          ++c;
      }
    }
    return c;
  }

  int GetCanMove() {
    int c = 0;
    REP(y, H) {
      REP(x, W) {
        if (visited_any_path[y][x])
          ++c;
      }
    }
    return c;
  }

  int CountFixedCell(const MazeCharType &maze) {
    int fixed = 0;
    REP(y, H) {
      REP(x, W) {
        if (maze[y][x] != M[y][x])
          ++fixed;
      }
    }
    return fixed;
  }

  void CreateCandidatePoint(
      const MazeCharType &status,
      const MazeBoolType &visited,
      const MazeBoolType &pointed,
      vector<int> &ys,
      vector<int> &xs) {
    assert(ys.empty());
    assert(xs.empty());
    REP(y, H) {
      REP(x, W) {
        if (status[y][x] != '.' &&
            status[y][x] != 'E' &&
            visited[y][x]) {
          REP(i, 4) {
            if (IsOutside(y + dy[i], x + dx[i]) == false && pointed[y + dy[i]][x + dx[i]] == false) {
              ys.push_back(y);
              xs.push_back(x);
              break;
            }
          }
        }
      }
    }
  }

  vector<int> Sample(int n, int k) {
    vector<int> seq(n);
    REP(i, n) {
      seq[i] = i;
    }
    for (int i = n - 1; i > 0; --i) {
      swap(seq[i], seq[xrand() % (i + 1)]);
    }
    seq.erase(seq.begin() + k, seq.end());
    assert((int)seq.size() == k);
    return seq;
  }

  vector<string> Optimize() {
    double time_start = gettime();
    double time_use = 9.8;
    double time_end = time_start + time_use;
    double time_current;

    MazeCharType best_status = M;
    Search(best_status);
    int best_score = GetScore();
    MazeCharType current_status = best_status;
    int current_potential = 10000 * best_score + GetCanMove();
    MazeBoolType current_visited = visited_any_path;
    MazeBoolType current_pointed = visited_over_all;
    // int current_fixed = CountFixedCell(current_status);

    while ((time_current = gettime()) < time_end) {
      vector<int> ys, xs;
      CreateCandidatePoint(current_status, current_visited, current_pointed, ys, xs);
      assert(xs.size() == ys.size());
      MazeCharType next_status = current_status;
      const int mul = 1;
      // const int mul = 100;
      // const int mul = min<int>(xs.size(), max<int>(1, 0.005 * (F - current_fixed)));
      // const int mul = min<int>(xs.size(), max<int>(1, 100 * (time_end - time_current) / time_use));
      REP(i, mul) {
        int rand_index = xrand() % xs.size();
        int move_y = ys[rand_index];
        int move_x = xs[rand_index];
        char move_d = "SLUR"[xrand() % 4];
        char tmp = next_status[move_y][move_x];
        next_status[move_y][move_x] = move_d;
        if (CountFixedCell(next_status) > F) {
          next_status[move_y][move_x] = tmp;
          break;
        }
      }
      Search(next_status);
      int next_score = GetScore();
      if (best_score < next_score) {
        best_score = next_score;
        best_status = next_status;
#ifdef LOCAL
        cerr << "[" << time_current - time_start << "] best_score = " << best_score << endl;
#endif
      }
      int next_potential = 10000 * next_score + GetCanMove();
      // bool force_next = xrand() / (1.0 + ULONG_MAX) < (time_end - time_current)/time_use;
      bool force_next = false;
      if (current_potential < next_potential || force_next) {
        current_potential = next_potential;
        current_status = next_status;
        current_visited = visited_any_path;
        current_pointed = visited_over_all;
        // current_fixed = CountFixedCell(next_status);
        // cerr << "[" << time_current - time_start << "] current_potential = " << current_potential << endl;
      }
    }

    vector<string> ret;
    REP(y, H) {
      REP(x, W) {
        if (best_status[y][x] != M[y][x]) {
          ret.push_back(to_string(y) + " " + to_string(x) + " " + best_status[y][x]);
        }
      }
    }
#ifdef LOCAL
    cerr << "R/F = " << ret.size() << "/" << F << endl;
#endif
    assert((int)ret.size() <= F);
    return ret;
  }

  vector<string> Improve() {
    SetUpBorder();
#ifdef ENABLE_PROFILE
    ProfilerStart("./pprof.out");
#endif
    vector<string> ret = Optimize();
#ifdef ENABLE_PROFILE
    ProfilerStop();
#endif
    return ret;
  }
};

struct MazeFixing {
  vector<string> improve(vector<string> maze, int F) {
    return Solver(maze, F).Improve();
  }
};

#ifdef LOCAL
int main() {
  // height: [10, 80]
  int H;
  cin >> H;
  // width: [10, 80]
  vector<string> maze(H);
  REP(i, H) {
    cin >> maze[i];
  }
  // fixes: [N/10, N/3], N is the number of non '.' cells in the maze
  int F;
  cin >> F;
  MazeFixing maze_fixing;
  vector<string> ret = maze_fixing.improve(maze, F);
  cout << ret.size() << endl;
  REP(i, ret.size()) {
    cout << ret[i] << endl;
  }
  cout.flush();
}
#endif
