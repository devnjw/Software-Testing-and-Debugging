set -x

export ASAN_OPTIONS=halt_on_error=1
export UBSAN_OPTIONS=halt_on_error=1

./test-target corpus seed -max_len=100
