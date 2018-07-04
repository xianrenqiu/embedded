# embedded project

## Linux

```t
$ mkdir -p build && cd build
$ cmake -DSAMPLE=hello ..
$ make
$ ./gdb.sh
```

## Windows

```t
$ cmake -G "MinGW Makefiles" -DSAMPLE=hello ..
$ mingw32-make.exe
```

## board
![image](https://github.com/xianrenqiu/embedded/blob/master/doc/board/A.jpg)