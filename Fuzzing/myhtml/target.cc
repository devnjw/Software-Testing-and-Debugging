#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "myhtml/api.h"

struct res_html {
	char *html;
	size_t size;
};

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* Data, size_t size){

        char * data = reinterpret_cast<char*>(const_cast<uint8_t*>(Data));
        if(size < 6){
                return 0;
        }

	struct res_html res = {data, (size_t)size};

        myhtml_t * myhtml = myhtml_create();
	myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);
	
	myhtml_tree_t* tree = myhtml_tree_create();
	myhtml_tree_init(tree, myhtml);

	myhtml_parse(tree, MyENCODING_UTF_8, res.html, res.size);

	mycore_string_raw_t str_raw;
	mycore_string_raw_clean_all(&str_raw);

	if(myhtml_serialization_tree_buffer(myhtml_tree_get_document(tree), &str_raw)) {
		fprintf(stderr, "Could not serailization for the tree\n");
		exit(EXIT_FAILURE);
	}

	mycore_string_raw_destroy(&str_raw, false);
	myhtml_tree_destroy(tree);
	myhtml_destroy(myhtml);

        return 0;
}
