#include <string.h>
#include "stack.h"
#include "hash.h"
#include "verification_and_check.h"
#include <errno.h>

static const size_t DEFAULT_SIZE_STK = 4;

static const size_t POISON_VAL = -1;

static CODE_ERRORS _Put_Down_Defense(Stack *stk);

static CODE_ERRORS _Raise_Up_Defense(Stack *stk);

static CODE_ERRORS Put_Down_Defense(Stack *stk)
{
    Can_type *ptr_left_can_in_data = (Can_type *)((char *)stk->ptr_data - (stk->size_canar));
    *ptr_left_can_in_data = POISON_VAL_FOR_CANARAY;

    #ifdef HASH
        stk->hash_s = POISON_VAL_FOR_HASH;
    #endif

    Can_type *ptr_right_can_in_data = (Can_type *)((char *)(stk->ptr_data) + (sizeof(Elen_s) * stk->copacity));
    *ptr_right_can_in_data = POISON_VAL_FOR_CANARAY;

    stk->ptr_data = (Elen_s *)((char *)stk->ptr_data - (stk->size_canar));

    return ALL_GOOD;

}

static CODE_ERRORS Raise_Up_Defense(Stack *stk)
{
    Can_type *ptr_left_can_in_data = (Can_type *)((char *)stk->ptr_data);
    *ptr_left_can_in_data = CALC_CANARAY((void *)ptr_left_can_in_data);
    stk->ptr_data = (Elen_s *)((char *)stk->ptr_data + (stk->size_canar));


    Can_type *ptr_right_can_in_data = (Can_type *)((char *)(stk->ptr_data) + (sizeof(Elen_s) * stk->copacity));
    *ptr_right_can_in_data = CALC_CANARAY((void *)ptr_right_can_in_data);

    #ifdef HASH
        recalc_hash(stk);
    #endif
    return ALL_GOOD;
}

CODE_ERRORS Realloc_Stack(Stack *stk, size_t new_size)
{
    #ifdef CANARAY
        Put_Down_Defense(stk);
    #endif

    stk->copacity = new_size;

    stk->ptr_data = (Elen_s *)realloc(stk->ptr_data,
                   sizeof(Elen_s *) * (new_size + (stk->size_canar) * 2));

    // stk_res = realloc
    // (stk_res != NUll)
    // stk->ptr_data = stk_res
    // TODO: check non-null

    #ifdef CANARAY
        Raise_Up_Defense(stk);
    #endif

    memset((stk->ptr_data + stk->size_s), POISON_VAL_FOR_STACK, (stk->copacity - stk->size_s) * sizeof(Elen_s));

    return ALL_GOOD;

}


Can_type calculate_canaray(void *ptr_on_degin_data)
{
    const size_t am_char_in_adress = 17;

    char val_1[am_char_in_adress * 3] = {};
    sprintf(val_1, "%p", ptr_on_degin_data);
    char val_2[am_char_in_adress] = "0xdeddedded";
    char *all_str = strcat(val_1, val_2);
    long long int hash_s = HASH_FUNC(all_str, strlen(val_1) + strlen(val_2));

    return hash_s;
}


CODE_ERRORS Stack_init(Stack *stk, size_t size_stk, unsigned int increase_size)
{
    LOG(1, stderr, "Stack_init begin");
    if (early_verificator(stk) == 0) fprintf(stderr, "ERROR");
    LOG(1, stderr, "Stack %d", stk == nullptr);

    stk->size_canar      = MAX(sizeof(Can_type), 8);
    stk->size_s = 0;
    stk->increase_size = increase_size;//TODO rename to max_size
    size_t real_size_stk = MIN(size_stk, DEFAULT_SIZE_STK);
    stk->copacity = real_size_stk;

    #ifdef CANARAY
        size_t size_stack = (sizeof(Elen_s) * stk->copacity) + stk->size_canar * 2;
        stk->LEFT_canaray = CALC_CANARAY(&(stk->LEFT_canaray));
    #else
        size_t size_stack = (sizeof(Elen_s) * stk->copacity);
        stk->size_canar = 0;
    #endif

    stk->ptr_data = (Elen_s *)calloc(size_stack, sizeof(char));
    // TODO: check NULL

    LOG(1, stderr, "\ncalloc\n %zu  %p\n", size_stack, stk->ptr_data);

    #ifdef CANARAY
        *(Can_type *)(stk->ptr_data) = (Can_type)CALC_CANARAY(stk->ptr_data);

        LOG(1, stderr, "\n ptr_LEFT_can_in_data, %p can %lld\n", stk->ptr_data, CALC_CANARAY(stk->ptr_data));

        stk->ptr_data = (Elen_s *)((char *  )stk->ptr_data + (size_t)(stk->size_canar));
        memset((void *)stk->ptr_data , POISON_VAL_FOR_STACK, stk->copacity * sizeof(Elen_s));

        Can_type *ptr_right_can_in_data = (Can_type *)((char *)(stk->ptr_data) + (sizeof(Elen_s) * stk->copacity));
        *ptr_right_can_in_data = (Can_type)CALC_CANARAY(ptr_right_can_in_data);

        LOG(1, stderr, "\n ptr_right_can_in_data, %pcan %lld\n", ptr_right_can_in_data, CALC_CANARAY(ptr_right_can_in_data));

        stk->RIGHT_canaray = CALC_CANARAY(&stk->RIGHT_canaray);
    #endif

    #ifdef HASH
        stk->hash_s = POISON_VAL_FOR_HASH;
        long long int hash_s_1 = 0;
        hash_s_1 = HASH_FUNC(stk, sizeof(Stack));

        stk->hash_s = hash_s_1;
        LOG(1, stderr, "hashs 1 %lld", hash_s_1);
        LOG(1, stderr, "hashs 1 %lld", 10);

        recalc_hash(stk);

    #endif

    Verification_code ver_code = verificator_out(stk);
    if (ver_code.error) STACK_DUMP(stk, ver_code);

    LOG(1, stderr, "Stack_init end");

    return ALL_GOOD;
}

CODE_ERRORS  Stack_Destructor(Stack *stk)
{
    LOG(1, stderr, "\n Stack_Destructor\n  %p\n", stk->ptr_data);

    Verification_code ver_code = verificator_out(stk);

    #ifdef CANARAY
        stk->LEFT_canaray  = POISON_VAL_FOR_CANARAY;
        stk->RIGHT_canaray = POISON_VAL_FOR_CANARAY;
    #endif

    #ifdef HASH
        if (!ver_code.ptr_data_null and !ver_code.cache_bad)
    #else
        if (!ver_code.ptr_data_null)
    #endif
    {
        for(size_t i = 0; i < (stk->copacity); i++)
        {
            *(stk->ptr_data + i) = POISON_VAL_FOR_STACK;
        }
        stk->ptr_data = (Elen_s *)((char *)stk->ptr_data - (sizeof(char) * stk->size_canar));

        free((void *)stk->ptr_data);
    }
    LOG(1, stderr, "\n free\n  %p\n", stk->ptr_data);
    LOG(1, stderr, " free sucsesful");

    stk->copacity = POISON_VAL;
    stk->size_s = POISON_VAL;

    stk = nullptr;
    free((void *)stk);
    return ALL_GOOD;
}

CODE_ERRORS Stack_Push(Stack *ptr_stk, Elen_s val_to_push)
{
    Verification_code ver_code = verificator_out(ptr_stk);
    if (ver_code.error) return STACK_DUMP(ptr_stk, ver_code);

    if (ptr_stk->copacity > ptr_stk->size_s)
    {
        *(ptr_stk->ptr_data + ptr_stk->size_s) = val_to_push;
        ptr_stk->size_s += 1;
        #ifdef HASH
            recalc_hash(ptr_stk);
        #endif
        return ALL_GOOD;
    }
    else
    {
        int code_ret = Realloc_Stack(ptr_stk, ptr_stk->copacity * sizeof(Elen_s) + 2 * ptr_stk->size_canar * sizeof(Can_type));
        *(ptr_stk->ptr_data + ptr_stk->size_s) = val_to_push;

        ptr_stk->size_s += 1;

        #ifdef HASH
            recalc_hash(ptr_stk);
        #endif
        return WAS_REALLOC;
    }
    return UNDEF_ERROR;
}

CODE_ERRORS Stack_Pop(Stack *ptr_stk, Elen_s *ptr_val_pop)
{
    Verification_code ver_code = verificator_out(ptr_stk);
    if (ver_code.error) return STACK_DUMP(ptr_stk, ver_code);

    if (ptr_stk->size_s > 0)
    {
        ptr_stk->size_s -= 1;
        *ptr_val_pop = *(ptr_stk->ptr_data + ptr_stk->size_s);
        *(ptr_stk->ptr_data + ptr_stk->size_s) = POISON_VAL_FOR_STACK;

        #ifdef HASH
            recalc_hash(ptr_stk);
        #endif

        if (((ptr_stk->size_s * ptr_stk->increase_size) < ptr_stk->copacity) and (ptr_stk->size_s != 0))
        {
           Realloc_Stack(ptr_stk, (int)(ptr_stk->copacity / 2));

           return WAS_REALLOC;
        }
        else if (ptr_stk->size_s != 0)
        {
            return STACK_SOON_EMPTY;
        }
        return ALL_GOOD;
    }
    else
    {
        *ptr_val_pop = POISON_VAL_FOR_STACK;
        return STACK_EMPTY;
    }
}


CODE_ERRORS _Stack_Dump(Stack *ptr_stk, Verification_code ver_code, const int LINE, const char *NAME_FUNC, const char *FILE_NAME)
{
    FILE *stream_out = open_file("log.txt", "w");

    fprintf(stderr, "begin _Stack_Dump \n ");

    #ifdef HASH
        recalc_hash(ptr_stk);
    #endif

    if (ver_code.stk_null)
    {
        fprintf(stderr, "stk_null - nullptr");

        return PTR_NULL;
    }
    else
    {
        fprintf(stream_out, "size_s %zu, copacity %zu, increase_size %d, size_canar %zu \n",
            ptr_stk->size_s, ptr_stk->copacity, ptr_stk->increase_size, ptr_stk->size_canar);

        fprintf(stream_out, "hash_s %lld, hash_data %lld, LEFT_canaray %lld RIGHT_canaray %lld",
            ptr_stk->hash_s, ptr_stk->hash_data, ptr_stk->LEFT_canaray, ptr_stk->RIGHT_canaray);

        fprintf(stream_out, "LINE %d, NAME_FUNC %s, FILE_NAME %s",
            LINE, NAME_FUNC, FILE_NAME);

        if (ver_code.ptr_data_null)
        {
            fprintf(stderr, "ptr_data_null - nullptr");

            return PTR_NULL;
        }
        else
        {
            if (ver_code.size_s_bad)                    fprintf(stderr, "size_s_bad");

            if (ver_code.left_can_bad)                  fprintf(stderr, "left_can_bad");

            if (ver_code.right_can_bad)                 fprintf(stderr, "right_can_bad");

            if (ver_code.right_data_can_bad)            fprintf(stderr, "right_data_can_bad");

            if (ver_code.left_data_can_bad)             fprintf(stderr, "left_data_can_bad");

            if (ver_code.hash_data)                     fprintf(stderr, "hash_data");

            if (ver_code.poison_name_el_s)              fprintf(stderr, "poison_name_el_s");

            if (ver_code.not_poison_name_el_s)          fprintf(stderr, "not_poison_name_el_s");

            if (ver_code.hash_data)                     fprintf(stderr, "hash_data_bad");

            if (ver_code.cache_bad){ptr_stk = nullptr;  fprintf(stderr, "cache_bad");}

            fprintf(stderr, "\n %lld ", *(Can_type *)((char *)ptr_stk->ptr_data - ptr_stk->size_canar));
            fprintf(stderr, "\n ptr %p ", (Can_type *)((char *)ptr_stk->ptr_data - ptr_stk->size_canar));

            for (size_t i = 0; i < ptr_stk->copacity; i++)
            {
                fprintf(stderr, " %d ", *(ptr_stk->ptr_data));
                ptr_stk->ptr_data += 1;
            }
            fprintf(stderr, "\n ptr %p ", (Can_type *)(ptr_stk->ptr_data));
            fprintf(stderr, " %lld \n", *(Can_type *)(ptr_stk->ptr_data));
        }
        Stack_Destructor(ptr_stk);

        LOG(1, stderr, "Stack_Destructor sucsesfull\n  %p\n", ptr_stk);

        close_file_for_debug(stream_out);
    }
    fprintf(stderr, "end _Stack_Dump \n");
    exit(1);
    return ALL_GOOD;

}
