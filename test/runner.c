/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "include/common.h"
#include "tests.h"

#include <stdlib.h>
#include <time.h>
#include <string.h>

int
main(int argc, const char * argv[]) {
  test_entry_t *entry;
  test_status_t st;
  int32_t       i, count, passed, failed, maxlen;
  double        start, end, elapsed, total;

  passed = failed = maxlen  = 0;
  total  = 0.0;
  count  = sizeof(tests) / sizeof(tests[0]);

  fprintf(stderr, CYAN "\nWelcome to cglm tests\n\n" RESET);

  for (i = 0; i < count; i++) {
    int32_t len;

    entry = tests + i;
    len   = (int32_t)strlen(entry->name);

    maxlen = GLM_MAX(maxlen, len);
  }

  maxlen += 5;

  fprintf(stderr,
          BOLDWHITE  "  %-*s    %-*s\n",
          maxlen, "Test Name", maxlen, "Elapsed Time");

  for (i = 0; i < count; i++) {
    entry   = tests + i;
    start   = clock();
    st      = entry->entry();
    end     = clock();
    elapsed = (end - start) / CLOCKS_PER_SEC;
    total  += elapsed;

    if (!st.status) {
      fprintf(stderr,
              BOLDRED  "  " FAIL_TEXT BOLDWHITE " %s " RESET, entry->name);
      if (st.msg) {
        fprintf(stderr,
                YELLOW "- %s" RESET,
                st.msg);
      }

      fprintf(stderr, "\n");

      failed++;
    } else {
      fprintf(stderr, GREEN  "  " OK_TEXT RESET " %-*s  ", maxlen, entry->name);

      if (elapsed > 0.01)
        fprintf(stderr, YELLOW "%.2fs", elapsed);
      else
        fprintf(stderr, "0");

      fprintf(stderr, "\n" RESET);
      passed++;
    }

    struct timespec t;
    t.tv_nsec = 100000000 * 0.5;
//    nanosleep(&t, NULL);
  }

  if (failed == 0) {
    fprintf(stderr,
            BOLDGREEN "\n  All tests are passed " FINAL_TEXT "\n" RESET);
  }

  fprintf(stderr,
          CYAN "\ncglm test results (%0.2fs):\n" RESET
          "--------------------------\n"

          MAGENTA "%d" RESET " tests are runned, "
          GREEN   "%d" RESET " %s passed, "
          RED     "%d" RESET " %s failed\n\n" RESET,
          total,
          count,
          passed,
          passed > 1 ? "are" : "is",
          failed,
          failed > 1 ? "are" : "is");

  return failed;
}
