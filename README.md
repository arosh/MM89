# How to run test

```
java MazeFixingVis -exec ./mm89 -seed 1 -debug -vis
```

# Memo

* border cellとは"RLUSE"のどれかに隣接する'.'のこと
* 評価関数に「指されているマスの数」を入れてみては？
* 到達可能なマスの数を最大化するような焼きなましをしてからパスを最大化する
* Followが遅い
* 既にF個変えてしまった時の処理
* 到達可能なマスを入れ替えたほうが効率が良い
* "A path can visit any cell only once, but different paths can visit same cell." なので，'S'でループになってもアウト
* せめてあと5倍は早くしよう
* Fを使い切れていない。Greedyに到達マスを増やしてから焼きなまし？
* 正確な評価難しいよね？？？高速化できなくね？？？

# How to profile

```
export CPUPROFILE=pprof.out # ProfilerStartしているなら不要
java MazeFixingVis -exec ./mm89 -vis -debug -seed 2
google-pprof --callgrind mm89 pprof.out > pprof.callgrind 2> /dev/null && kcachegrind pprof.callgrind 2> /dev/null
```

```
java MazeFixingVis -exec "valgrind --tool=callgrind --callgrind-out-file=callgrind.out ./mm89" -vis -debug -seed 2
```

# References

* https://community.topcoder.com/longcontest/?module=ViewProblemStatement&rd=16494&compid=12525
* http://shindannin.hatenadiary.com/entry/20121224/1356364040
* https://github.com/arosh/arosh.github.com/wiki/Debug
