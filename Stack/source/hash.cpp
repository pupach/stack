#include "stack.h"
#include "verification_and_check.h"
#include "hash.h"




CODE_ERRORS recalc_hash(Stack *stk)
{
    stk->hash_s = POISON_VAL_FOR_HASH;
    stk->hash_data = POISON_VAL_FOR_HASH;

    stk->hash_s = HASH_FUNC(stk, sizeof(Stack));
    stk->hash_data = HASH_FUNC(stk->ptr_data , sizeof(Elen_s) * stk->copacity);


    return ALL_GOOD;
}

unsigned long long int hash_my(char *ptr_on_beg_calc, size_t am_byte)
{
    unsigned long long int res_hash = 0;
    for (size_t i = 0; i < am_byte; i++)
    {
        res_hash += (unsigned long long int)ptr_on_beg_calc[i];
    }

    return res_hash;
}
