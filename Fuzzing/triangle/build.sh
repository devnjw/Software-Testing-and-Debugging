set -x

clang -g -O1 -fsanitize=fuzzer,signed-integer-overflow triangle.c fuzz_target.c -o test-target
