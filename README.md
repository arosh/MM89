# How to run test

```
java MazeFixingVis -exec ./mm89 -seed 1 -debug -vis
```

# Memo

* border cellとは"RLUSE"のどれかに隣接する'.'のこと
* 評価関数に「指されているマスの数」を入れてみては？

# How to profile

```
export CPUPROFILE=pprof.out
java MazeFixingVis -exec ./mm89 -vis -debug -seed 2
google-pprof --callgrind mm89 pprof.out > pprof.callgrind 2> /dev/null && kcachegrind pprof.callgrind 2> /dev/null
```

# References

* http://shindannin.hatenadiary.com/entry/20121224/1356364040
* https://github.com/arosh/arosh.github.com/wiki/Debug
