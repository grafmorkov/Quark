#include <stdint.h>
#include <stdbool.h>


int main() {
entry:
    int t0;
    t0 = 1;
    goto while_cond;
while_cond:
    int t1 = t0 < 10000000;
    if (t1) goto while_body; else goto while_end;
while_body:
    t0 = 1;
    goto while_cond;
while_end:
    return 0;
}
