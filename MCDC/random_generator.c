#include <stdio.h>
#include <stdlib.h>
int main () {
    FILE * fp = fopen("test_input", "w");
    for(int n = 0 ; n < 100 ; n++){
        for(int i = 0 ; i < 12 ; i++){
            fprintf(fp, "%d ", rand() % 1000);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}
