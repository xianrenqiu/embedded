# embedded project

## 运行环境

### Linux

```t
$ mkdir -p build && cd build
$ cmake -DSAMPLE=hello ..
$ make
$ ./gdb.sh
```

### Windows

```t
$ cmake -G "MinGW Makefiles" -DSAMPLE=hello ..
$ mingw32-make.exe
```
