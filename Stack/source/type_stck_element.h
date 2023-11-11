#ifndef TYPE_ELEMENT
#define TYPE_ELEMENT

#ifdef CHAR
typedef char Elen_s;
static const char POISON_VAL_FOR_STACK = '_';
#define %STK %c

#elifdef DOUBLE
typedef double Elen_s;
static const double POISON_VAL_FOR_STACK = NAN;
#define %STK %lf

#elifdef LONG_INT
typedef long int Elen_s;
static const double POISON_VAL_FOR_STACK = 99999;
#define %STK %ld

#elifdef CHAR_P
typedef (char *) Elen_s;
static const char * POISON_VAL_FOR_STACK = &'9';
#define %STK %s

#else
typedef long int Elen_s;
static const long int  POISON_VAL_FOR_STACK = -1;
#define _T_STK %lf
#endif

#ifdef CAN_DOUBLE
typedef double Can_type;
static const double POISON_VAL_FOR_CANARAY = INF;
#define %CAN %lf

#elifdef CAN_LONG_INT
typedef long int Can_type;
static const double POISON_VAL_FOR_CANARAY = INF;
#define %CAN %ld

#elifdef CAN_INT_P
typedef (int *) Can_type;
static const double POISON_VAL_FOR_CANARAY = nullptr;
#define CAN_PRINTF "%p"

#else
typedef long long int Can_type;
static const long long int POISON_VAL_FOR_CANARAY = -123456;

#endif

const long long int POISON_VAL_FOR_HASH = 0;

#define CANARAY

#define HASH

#endif


