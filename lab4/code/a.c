#include <stdio.h>
#include "time.h"

void main() {
    time_t timer, timerc;
    int count = 1;
    time(&timer);
    while (1) {
        time(&timerc);
        if ((timerc - timer) >= 5) {
          break;
        }
    }
}
