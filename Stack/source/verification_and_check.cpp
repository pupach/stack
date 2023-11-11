#include "stack.h"
#include "verification_and_check.h"
#include "hash.h"
#include "../../my_lib/work_with_file.h"



static void check_data_canaray(Stack *stk, Verification_code *ver_code);

// TODO: remove until needed
int early_verificator(Stack *stk)
{
    return 1;
}


Verification_code verificator_out(Stack *stk)
{
    Verification_code ver_code = {};

    if (stk == nullptr)
    {
        ver_code.stk_null = 1;
        ver_code.error = true;
        ERROR_S(stk, &ver_code);
        return ver_code;
    }
    if (stk->ptr_data == nullptr)
    {
        ver_code.ptr_data_null = 1;
        ver_code.error = true;
    }
    #ifdef CANARAY
        if (stk->LEFT_canaray != CALC_CANARAY(&(stk->LEFT_canaray)))
        {
            ver_code.left_can_bad = 1;
            ver_code.error = true;
        }
        if (stk->RIGHT_canaray != CALC_CANARAY(&(stk->RIGHT_canaray)))
        {
            ver_code.right_can_bad = 1;
            ver_code.error = true;
        }
    #endif
    #ifdef HASH
        long long int hash_s = stk->hash_s;
        long long int hash_data = stk->hash_data;
        stk->hash_s = POISON_VAL_FOR_HASH;
        stk->hash_data = POISON_VAL_FOR_HASH;
        if (hash_s != HASH_FUNC(stk, sizeof(Stack)))
        {
            ver_code.cache_bad = 1;
            ver_code.error = true;
            LOG(1, stderr, "\nhash_s %lld HASH %lld\n", hash_s, HASH(stk, sizeof(Stack)));
        }
        if(!ver_code.ptr_data_null and !ver_code.cache_bad)
            {
            if (hash_data != HASH_FUNC(stk->ptr_data, sizeof(Elen_s) * stk->copacity))
            {
                ver_code.hash_data = 1;
                ver_code.error = true;
            }
        }
    stk->hash_s = hash_s;
    stk->hash_data = hash_data;
    #endif
    #ifdef CANARAY
    if (!ver_code.ptr_data_null and !ver_code.cache_bad and !ver_code.hash_data)
    {
        check_data_canaray(stk, &ver_code);
    }
    #endif
    if (ver_code.error)
    {
        ERROR_S(stk, &ver_code);
        return ver_code;
    }

    return ver_code;
}


static void check_data_canaray(Stack *stk, Verification_code *ver_code)
{

    Can_type *ptr_right_can_in_data = (Can_type *)((char *)(stk->ptr_data) + (sizeof(Elen_s) * stk->copacity));
    Can_type ptr_right_right_can = CALC_CANARAY(ptr_right_can_in_data);

///    fprintf(stderr, "\n_check_data_canaray right ptr %p \n", ptr_right_can_in_data);

    Can_type *ptr_left_can_in_data = (Can_type *)((char *)(stk->ptr_data) -  stk->size_canar);
    Can_type ptr_right_left_can = CALC_CANARAY(ptr_left_can_in_data);

//    fprintf(stderr, "\n_check_data_canaray left ptr %p \n", ptr_left_can_in_data);

    if (*ptr_right_can_in_data != ptr_right_right_can)
    {
        ver_code->right_data_can_bad = 1;
        ver_code->error = true;
        fprintf(stderr, "ptr_right_right_can %lld *ptr_right_can_in_data %lld \n", ptr_right_right_can, *ptr_right_can_in_data);
    }
    if (*ptr_left_can_in_data != ptr_right_left_can)
    {
        ver_code->left_data_can_bad = 1;
        ver_code->error = true;
       fprintf(stderr, "ptr_right_left_can %lld *ptr_left_can_in_data %lld \n", ptr_right_left_can, *ptr_left_can_in_data);
    }


    for (size_t i = 0; i < stk->size_s; i++)
    {
        if (*(stk->ptr_data + i) == POISON_VAL_FOR_STACK)
        {
            ver_code->poison_name_el_s = 1;
            ver_code->error = true;
        }
    }

    for (size_t i = stk->size_s; i < stk->copacity; i++)
    {
        if (*(stk->ptr_data + i) != POISON_VAL_FOR_STACK)
        {
            ver_code->not_poison_name_el_s = 1;
            ver_code->error = true;
        }
    }

}


int ERROR_S(Stack *ptr_stk, Verification_code *ver_code)
{

    return 1;
}

