# clang -fsanitize=address -c tmp.c
clang -g -fsanitize=address -fsanitize-coverage=trace-pc-guard -o triangle triangle.c tmp.o
export ASAN_OPTIONS=''
./triangle
