#ifndef HASH_D
#define HASH_D
#include <stdlib.h>  // � ���, � ���, ������ ���� �� ����, �����������!!!
#include <stdio.h>
#include <math.h>

#define HASH_FUNC(ptr_on_beg_calc, am_byte) \
    hash_my((char *)ptr_on_beg_calc, am_byte)

CODE_ERRORS recalc_hash(Stack *stk);

unsigned long long int hash_my(char *ptr_on_beg_calc, size_t am_byte);
#endif
