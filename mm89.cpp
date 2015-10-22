#include <bits/stdc++.h>
using namespace std;
#define REP(i, n) for (int i = 0; i < (int)(n); ++i)

vector<string> improve(vector<string> maze, int F) {
  return vector<string>();
}

int main() {
  // height: [10, 80]
  int H;
  cin >> H;
  // width: [10, 80]
  vector<string> maze(H);
  REP(i, H) getline(cin, maze[i]);
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
