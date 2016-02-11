#include "postgres.h"
#include "fmgr.h"
#include "libpq/pqformat.h"
#include "utils/builtins.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

typedef uint32 base64_4b_t;

static char base64_chars[64] = "+-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static char char_to_num_b64[] = {
    127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,
    127,127,127,127,127,127,127,127,127,127,127,0,127,1,127,127,2,3,4,5,6,7,8,9,10,11,127,127,127,127,127,127,127,12,13,14,15,16,17,
    18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,127,127,127,127,127,127,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
    52,53,54,55,56,57,58,59,60,61,62,63,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,
    127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,
    127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,
    127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,
    127,127,127,127,127,127,127,127,127,127,127,127,127,127};

#define base644BGetDatum(x)	 UInt32GetDatum(x)
#define DatumGetbase644B(x)	 DatumGetUInt32(x)
#define PG_GETARG_BASE644B(n) DatumGetbase644B(PG_GETARG_DATUM(n))
#define PG_RETURN_BASE644B(x) return base644BGetDatum(x)

Datum base64_4b_in(PG_FUNCTION_ARGS);
Datum base64_4b_out(PG_FUNCTION_ARGS);
Datum base64_4b_to_text(PG_FUNCTION_ARGS);
Datum base64_4b_from_text(PG_FUNCTION_ARGS);
Datum base64_4b_send(PG_FUNCTION_ARGS);
Datum base64_4b_recv(PG_FUNCTION_ARGS);
Datum base64_4b_lt(PG_FUNCTION_ARGS);
Datum base64_4b_le(PG_FUNCTION_ARGS);
Datum base64_4b_eq(PG_FUNCTION_ARGS);
Datum base64_4b_ne(PG_FUNCTION_ARGS);
Datum base64_4b_ge(PG_FUNCTION_ARGS);
Datum base64_4b_gt(PG_FUNCTION_ARGS);
Datum base64_4b_cmp(PG_FUNCTION_ARGS);

static base64_4b_t cstring_to_base64_4b(char *base64_4b_string);
static char *base64_4b_to_cstring(base64_4b_t base64_4b);



/* generic input/output functions */
PG_FUNCTION_INFO_V1(base64_4b_in);
Datum
base64_4b_in(PG_FUNCTION_ARGS)
{
	base64_4b_t	result;

	char   *base64_4b_str = PG_GETARG_CSTRING(0);
	result = cstring_to_base64_4b(base64_4b_str);

	PG_RETURN_BASE644B(result);
}

PG_FUNCTION_INFO_V1(base64_4b_out);
Datum
base64_4b_out(PG_FUNCTION_ARGS)
{
	base64_4b_t	packed_base64_4b;
	char   *base64_4b_string;

	packed_base64_4b = PG_GETARG_BASE644B(0);
	base64_4b_string = base64_4b_to_cstring(packed_base64_4b);

	PG_RETURN_CSTRING(base64_4b_string);
}

/* type to/from text conversion routines */
PG_FUNCTION_INFO_V1(base64_4b_to_text);
Datum
base64_4b_to_text(PG_FUNCTION_ARGS)
{
	char	*base64_4b_string;
	text	*base64_4b_text;

	base64_4b_t	packed_base64_4b =  PG_GETARG_BASE644B(0);

	base64_4b_string = base64_4b_to_cstring(packed_base64_4b);
	base64_4b_text = DatumGetTextP(DirectFunctionCall1(textin, CStringGetDatum(base64_4b_string)));

	PG_RETURN_TEXT_P(base64_4b_text);
}

PG_FUNCTION_INFO_V1(base64_4b_from_text);
Datum
base64_4b_from_text(PG_FUNCTION_ARGS)
{
	text  *base64_4b_text = PG_GETARG_TEXT_P(0);
	char  *base64_4b_str = DatumGetCString(DirectFunctionCall1(textout, PointerGetDatum(base64_4b_text)));
	base64_4b_t base64_4b = cstring_to_base64_4b(base64_4b_str);

	PG_RETURN_BASE644B(base64_4b);
}

/* Functions to convert to/from bytea */
PG_FUNCTION_INFO_V1(base64_4b_send);
Datum
base64_4b_send(PG_FUNCTION_ARGS)
{
	StringInfoData buffer;
	base64_4b_t base64_4b = PG_GETARG_BASE644B(0);

	pq_begintypsend(&buffer);
	pq_sendint(&buffer, (uint32)base64_4b, 4);
	PG_RETURN_BYTEA_P(pq_endtypsend(&buffer));
}

PG_FUNCTION_INFO_V1(base64_4b_recv);
Datum base64_4b_recv(PG_FUNCTION_ARGS)
{
	base64_4b_t	base64_4b;
	StringInfo	buffer = (StringInfo) PG_GETARG_POINTER(0);

	base64_4b = pq_getmsgint(buffer, 4);
	PG_RETURN_BASE644B(base64_4b);
}

/* functions to support btree opclass */
PG_FUNCTION_INFO_V1(base64_4b_lt);
Datum
base64_4b_lt(PG_FUNCTION_ARGS)
{
	base64_4b_t a = PG_GETARG_BASE644B(0);
	base64_4b_t b = PG_GETARG_BASE644B(1);
	PG_RETURN_BOOL(a < b);
}

PG_FUNCTION_INFO_V1(base64_4b_le);
Datum
base64_4b_le(PG_FUNCTION_ARGS)
{
	base64_4b_t a = PG_GETARG_BASE644B(0);
	base64_4b_t b = PG_GETARG_BASE644B(1);
	PG_RETURN_BOOL (a <= b);
}

PG_FUNCTION_INFO_V1(base64_4b_eq);
Datum
base64_4b_eq(PG_FUNCTION_ARGS)
{
	base64_4b_t a = PG_GETARG_BASE644B(0);
	base64_4b_t b = PG_GETARG_BASE644B(1);
	PG_RETURN_BOOL(a == b);
}

PG_FUNCTION_INFO_V1(base64_4b_ne);
Datum
base64_4b_ne(PG_FUNCTION_ARGS)
{
	base64_4b_t a = PG_GETARG_BASE644B(0);
	base64_4b_t b = PG_GETARG_BASE644B(1);
	PG_RETURN_BOOL(a != b);
}

PG_FUNCTION_INFO_V1(base64_4b_ge);
Datum
base64_4b_ge(PG_FUNCTION_ARGS)
{
	base64_4b_t a = PG_GETARG_BASE644B(0);
	base64_4b_t b = PG_GETARG_BASE644B(1);
	PG_RETURN_BOOL(a >= b);
}

PG_FUNCTION_INFO_V1(base64_4b_gt);
Datum
base64_4b_gt(PG_FUNCTION_ARGS)
{
	base64_4b_t a = PG_GETARG_BASE644B(0);
	base64_4b_t b = PG_GETARG_BASE644B(1);
	PG_RETURN_BOOL(a > b);
}

static int32
base64_4b_cmp_internal(base64_4b_t a, base64_4b_t b)
{
    // This signed operation works because only 30 of 32 bits are used.
    return a - b;

    /*if (a < b)
        return -1;
    else if (a > b)
        return 1;

    return 0;*/
}

PG_FUNCTION_INFO_V1(base64_4b_cmp);
Datum
base64_4b_cmp(PG_FUNCTION_ARGS)
{
	base64_4b_t a = PG_GETARG_BASE644B(0);
	base64_4b_t b = PG_GETARG_BASE644B(1);

	PG_RETURN_INT32(base64_4b_cmp_internal(a, b));
}

/*****************************************************************************
 * Aggregate functions
 *****************************************************************************/

PG_FUNCTION_INFO_V1(base64_4b_smaller);

Datum
base64_4b_smaller(PG_FUNCTION_ARGS)
{
   base64_4b_t left  = PG_GETARG_BASE644B(0);
   base64_4b_t right = PG_GETARG_BASE644B(1);
   int32 result;

   result = base64_4b_cmp_internal(left, right) < 0 ? left : right;
   PG_RETURN_BASE644B(result);
}

PG_FUNCTION_INFO_V1(base64_4b_larger);

Datum
base64_4b_larger(PG_FUNCTION_ARGS)
{
   base64_4b_t left  = PG_GETARG_BASE644B(0);
   base64_4b_t right = PG_GETARG_BASE644B(1);
   int32 result;

   result = base64_4b_cmp_internal(left, right) > 0 ? left : right;
   PG_RETURN_BASE644B(result);
}


/*
 * Convert a cstring to a base64_4b.
 */
static base64_4b_t
cstring_to_base64_4b(char *base64_4b_str)
{
    char                *ptr;
    unsigned long int   total = 0;
    unsigned int        digit_value;

    ptr = base64_4b_str;

    if (*ptr == 0)
        ereport(ERROR,
            (errmsg("base64_4b number \"%s\" must be 1 digits or larger.", base64_4b_str)));

    for (; ; ptr += 1) {
        if (*ptr == 0)
            PG_RETURN_BASE644B(total);

        digit_value = char_to_num_b64[(unsigned)*ptr];

        if (digit_value == 127)
            ereport(ERROR,
                (errmsg("base64_4b number \"%s\" must only contain +, -, 0 to 9, A to Z, and a to z.", base64_4b_str)));

        //total *= 64;
        //total += digit_value;
        total = (total << 6) | digit_value;

        if (total > 1073741823)
            ereport(ERROR,
                (errmsg("base64_4b number \"%s\" must be 5 digits or smaller.", base64_4b_str)));
    }

    PG_RETURN_BASE644B(total);
}

/* Convert the internal representation to output string */
static char *
base64_4b_to_cstring(base64_4b_t base64_4b)
{
    char buffer[9];
    unsigned int offset = sizeof(buffer);
    long unsigned int remainder = base64_4b;
    char   *base64_4b_str;

    buffer[--offset] = '\0';
    do {
        buffer[--offset] = base64_chars[remainder & 63];
        remainder = remainder >> 6;
    } while (remainder > 0);

    base64_4b_str = palloc(sizeof(buffer) - offset);
    memcpy(base64_4b_str, &buffer[offset], sizeof(buffer) - offset);
    return base64_4b_str;
}
