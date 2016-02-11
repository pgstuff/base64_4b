/*
 * Author: The maintainer's name
 * Created at: Wed Oct 14 23:12:59 -0400 2015
 *
 */

SET client_min_messages = warning;

-- SQL definitions for base64_4b type
CREATE TYPE base64_4b;

-- basic i/o functions
CREATE OR REPLACE FUNCTION base64_4b_in(cstring) RETURNS base64_4b AS '$libdir/base64_4b'
LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION base64_4b_out(base64_4b) RETURNS cstring AS '$libdir/base64_4b'
LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION base64_4b_send(base64_4b) RETURNS bytea AS '$libdir/base64_4b'
LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION base64_4b_recv(internal) RETURNS base64_4b AS '$libdir/base64_4b'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE base64_4b (
	input = base64_4b_in,
	output = base64_4b_out,
	send = base64_4b_send,
	receive = base64_4b_recv,
	internallength = 4,
	passedbyvalue
);

-- functions to support btree opclass
CREATE OR REPLACE FUNCTION base64_4b_lt(base64_4b, base64_4b) RETURNS bool AS '$libdir/base64_4b'
LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION base64_4b_le(base64_4b, base64_4b) RETURNS bool AS '$libdir/base64_4b'
LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION base64_4b_eq(base64_4b, base64_4b) RETURNS bool AS '$libdir/base64_4b'
LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION base64_4b_ne(base64_4b, base64_4b) RETURNS bool AS '$libdir/base64_4b'
LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION base64_4b_ge(base64_4b, base64_4b) RETURNS bool AS '$libdir/base64_4b'
LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION base64_4b_gt(base64_4b, base64_4b) RETURNS bool AS '$libdir/base64_4b'
LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION base64_4b_cmp(base64_4b, base64_4b) RETURNS int4 AS '$libdir/base64_4b'
LANGUAGE C IMMUTABLE STRICT;

-- to/from text conversion
CREATE OR REPLACE FUNCTION base64_4b_to_text(base64_4b) RETURNS text AS '$libdir/base64_4b'
LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION base64_4b_from_text(text) RETURNS base64_4b AS '$libdir/base64_4b'
LANGUAGE C IMMUTABLE STRICT;

-- operators
CREATE OPERATOR < (
	leftarg = base64_4b, rightarg = base64_4b, procedure = base64_4b_lt,
	commutator = >, negator = >=,
	restrict = scalarltsel, join = scalarltjoinsel
);
CREATE OPERATOR <= (
	leftarg = base64_4b, rightarg = base64_4b, procedure = base64_4b_le,
	commutator = >=, negator = >,
	restrict = scalarltsel, join = scalarltjoinsel
);
CREATE OPERATOR = (
	leftarg = base64_4b, rightarg = base64_4b, procedure = base64_4b_eq,
	commutator = =, negator = <>,
	restrict = eqsel, join = eqjoinsel,
	merges
);
CREATE OPERATOR <> (
	leftarg = base64_4b, rightarg = base64_4b, procedure = base64_4b_ne,
	commutator = <>, negator = =,
	restrict = neqsel, join = neqjoinsel
);
CREATE OPERATOR > (
	leftarg = base64_4b, rightarg = base64_4b, procedure = base64_4b_gt,
	commutator = <, negator = <=,
	restrict = scalargtsel, join = scalargtjoinsel
);
CREATE OPERATOR >= (
	leftarg = base64_4b, rightarg = base64_4b, procedure = base64_4b_ge,
	commutator = <=, negator = <,
	restrict = scalargtsel, join = scalargtjoinsel
);

-- aggregates
CREATE OR REPLACE FUNCTION base64_4b_smaller(base64_4b, base64_4b)
RETURNS base64_4b
AS '$libdir/base64_4b'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION base64_4b_larger(base64_4b, base64_4b)
RETURNS base64_4b
AS '$libdir/base64_4b'
    LANGUAGE C IMMUTABLE STRICT;

CREATE AGGREGATE min(base64_4b)  (
    SFUNC = base64_4b_smaller,
    STYPE = base64_4b,
    SORTOP = <
);

CREATE AGGREGATE max(base64_4b)  (
    SFUNC = base64_4b_larger,
    STYPE = base64_4b,
    SORTOP = >
);

-- btree operator class
CREATE OPERATOR CLASS base64_4b_ops DEFAULT FOR TYPE base64_4b USING btree AS
	OPERATOR 1 <,
	OPERATOR 2 <=,
	OPERATOR 3 =,
	OPERATOR 4 >=,
	OPERATOR 5 >,
	FUNCTION 1 base64_4b_cmp(base64_4b, base64_4b);
-- cast from/to text
CREATE CAST (base64_4b AS text) WITH FUNCTION base64_4b_to_text(base64_4b) AS ASSIGNMENT;
CREATE CAST (text AS base64_4b) WITH FUNCTION base64_4b_from_text(text) AS ASSIGNMENT;
