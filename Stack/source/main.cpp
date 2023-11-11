#include <string.h>
#include "stack.h"
#include "../../my_lib/work_with_file.h"
#include "hash.h"
#include "verification_and_check.h"
#include <errno.h>

int main() {
    printf("AAAAAAAAAAA\n");
    Stack stk1 = {0};
    printf("cap = %d\n", stk1.copacity);

    Stack *stk = &stk1;
    Elen_s c = 9;

    Elen_s j = 9;

    size_t d = 3;
    Stack_init(stk, d, 2);


    for (int i =0; i < 5; i++)
    {
        j += 1;
        LOG(1, stderr, "\n sdfsfd %d    ", i);
        int i1 = Stack_Push(stk, j);
        LOG(1, stderr, "sdfsfd %d %d\n", i, i1);
    }

    Stack_Pop(stk, &c);
    Stack_Pop(stk, &j);

    LOG(1, stderr, "\n stack %d, %d\n", j, c);

    Verification_code ver_code = verificator_out(stk);
    if (ver_code.error) STACK_DUMP(stk, ver_code);
}
