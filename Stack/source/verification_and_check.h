#ifndef VERFICATION
#define VERFICATION


#include <stdlib.h>
  // � ���, � ���, ������ ���� �� ����, �����������!!!
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "type_stck_element.h"



struct Verification_code
{
    unsigned int stk_null             : 1;
    unsigned int ptr_data_null        : 1;
    unsigned int size_s_bad           : 1;
    unsigned int cache_bad            : 1;
    unsigned int left_can_bad         : 1;
    unsigned int right_can_bad        : 1;
    unsigned int right_data_can_bad   : 1;
    unsigned int left_data_can_bad    : 1;
    unsigned int hash_data            : 1;
    unsigned int poison_name_el_s     : 1;
    unsigned int not_poison_name_el_s : 1;
    bool error = false;


};

Verification_code verificator_out(Stack *stk);

int early_verificator(Stack *stk);

#endif
