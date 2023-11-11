#ifndef STACK
#define STACK

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../../my_lib/work_with_file.h"
#include "type_stck_element.h"



#define STACK_DUMP(ptr_stk, ver_code)            \
    _Stack_Dump(ptr_stk, ver_code, __LINE__, __PRETTY_FUNCTION__, __FILE__)


#define CALC_CANARAY(val)     \
    calculate_canaray((void *)val)

#define MAX(x, y)   \
    x > y ? x : y

#define MIN(x, y)   \
    x > y ? y : x


struct Stack
{
    Can_type LEFT_canaray = POISON_VAL_FOR_CANARAY;

    int size_s                = -1;
    int copacity              = -1;//size_t для ядовитого значения
    unsigned int increase_size   = 2;
    size_t size_canar      = MAX(sizeof(Can_type), 8);
    long long int hash_s         = POISON_VAL_FOR_HASH;
    long long int hash_data      = POISON_VAL_FOR_HASH;

    Elen_s *ptr_data = nullptr;

    Can_type RIGHT_canaray = POISON_VAL_FOR_CANARAY;
};

#include "verification_and_check.h"

Can_type calculate_canaray(void *ptr_on_degin_data);

CODE_ERRORS Stack_init(Stack *stk, size_t size_stk, unsigned int increase_size=2);

CODE_ERRORS Stack_Push(Stack *ptr_stk, Elen_s val_to_push);

CODE_ERRORS Realloc_Stack(Stack *stk, size_t new_size);

CODE_ERRORS  Stack_Destructor(Stack *stk);

CODE_ERRORS Stack_Pop(Stack *ptr_stk, Elen_s *ptr_val_pop);

int ERROR_S(Stack *ptr_stk, Verification_code *ver_code);

CODE_ERRORS _Stack_Dump(Stack *ptr_stk, Verification_code ver_code, const int LINE, const char *NAME_FUNC, const char *FILE_NAME);
#endif
