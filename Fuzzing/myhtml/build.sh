#!/bin/bash

set -x

export CFLAGS="-O2 -fno-omit-frame-pointer -gline-tables-only -fsanitize=address,fuzzer-no-link -fsanitize-address-use-after-scope"
export CC="clang"

build_lib(){
	rm -rf myhtml
	git clone https://github.com/lexborisov/myhtml.git
	(cd myhtml && make static)
}
build_fuzzer(){
	clang -g -Imyhtml/include ${CFLAGS} -fsanitize=fuzzer target.cc myhtml/lib/libmyhtml_static.a -o fuzzer_myhtml
}

build_lib
build_fuzzer
