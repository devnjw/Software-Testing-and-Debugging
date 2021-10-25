#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sanitizer/coverage_interface.h>

int *edge_pair;
int edge_pair_idx, count = 0 ;
uint32_t range ;
void make_log_file() {
   int fd = open("coverage_log", O_WRONLY | O_CREAT | O_TRUNC,0644);

   if ( fd < 0 ) {
       perror("Cannot open the file!\n");
       return;
   }
   if (edge_pair_idx < 2) {
   }else {
                for (int i = 2 ; i < edge_pair_idx; i++) {
                char ep[32];
                int length = sprintf(ep, "%d,%d,%d\n", edge_pair[i - 2], edge_pair[i - 1], edge_pair[i]);

                size_t s = 0 ;
                while( s < length ) {
                           s += write(fd, ep, length - s);
                }
                }
   }
   close(fd);
   free(edge_pair) ;
}

extern void __sanitizer_cov_trace_pc_guard_init(uint32_t *start,
                                                    uint32_t *stop) {
  static uint64_t N;  // Counter for the guards.
  if (start == stop || *start) return;  // Initialize only once.

  for (uint32_t *x = start; x < stop; x++)
    *x = ++N;  // Guards should start from 1.

  range = stop - start + 1;
  edge_pair = (int *)malloc(sizeof(int) * 2 * range) ;

  atexit(make_log_file);
}

extern void __sanitizer_cov_trace_pc_guard(uint32_t *guard) {
  if (!*guard) return;  // Duplicate the guard check.
  char *token;
  const char *delimiter = ":";
  void *PC = __builtin_return_address(0);
  char PcDescr[1024];
  // This function is a part of the sanitizer run-time.
  // To use it, link with AddressSanitizer or other sanitizer.
  __sanitizer_symbolize_pc(PC, "%p %F %L", PcDescr, sizeof(PcDescr));

  token = strtok(PcDescr, delimiter) ;
  token = strtok(NULL, delimiter);
  if ( token != NULL ) {
        int line_number = atoi(token) ;

        edge_pair[edge_pair_idx++] = line_number ;
  }
}

