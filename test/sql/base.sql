\set ECHO none
\i sql/base64_4b.sql
\set ECHO all

CREATE TABLE base64_4bs(id serial primary key, base64_4b base64_4b unique);
INSERT INTO base64_4bs(base64_4b) VALUES('+-012'); -- The + is 0, and leading 0's are truncated.
INSERT INTO base64_4bs(base64_4b) VALUES('34567');
INSERT INTO base64_4bs(base64_4b) VALUES('89ABC');
INSERT INTO base64_4bs(base64_4b) VALUES('DEFGH');
INSERT INTO base64_4bs(base64_4b) VALUES('IJKLM');
INSERT INTO base64_4bs(base64_4b) VALUES('NOPQR');
INSERT INTO base64_4bs(base64_4b) VALUES('STUVW');
INSERT INTO base64_4bs(base64_4b) VALUES('XYZab');
INSERT INTO base64_4bs(base64_4b) VALUES('cdefg');
INSERT INTO base64_4bs(base64_4b) VALUES('hijkl');
INSERT INTO base64_4bs(base64_4b) VALUES('mnopq');
INSERT INTO base64_4bs(base64_4b) VALUES('rstuv');
INSERT INTO base64_4bs(base64_4b) VALUES('wxyz');
INSERT INTO base64_4bs(base64_4b) VALUES('20004');
INSERT INTO base64_4bs(base64_4b) VALUES('40002');
INSERT INTO base64_4bs(base64_4b) VALUES('30003');
INSERT INTO base64_4bs(base64_4b) VALUES('+'); -- test limits
INSERT INTO base64_4bs(base64_4b) VALUES('zzzzz');

SELECT * FROM base64_4bs ORDER BY base64_4b;

SELECT MIN(base64_4b) AS min FROM base64_4bs;
SELECT MAX(base64_4b) AS max FROM base64_4bs;

-- index scan
TRUNCATE base64_4bs;
INSERT INTO base64_4bs(base64_4b) SELECT '4'||id FROM generate_series(5678, 8000) id;
SELECT id,base64_4b::text FROM base64_4bs WHERE base64_4b = '48000';

SET enable_seqscan = false;
SELECT id,base64_4b::text FROM base64_4bs WHERE base64_4b = '46000';
SELECT id,base64_4b FROM base64_4bs WHERE base64_4b >= '47000' LIMIT 5;
SELECT count(id) FROM base64_4bs;
SELECT count(id) FROM base64_4bs WHERE base64_4b <> ('46500'::text)::base64_4b;
RESET enable_seqscan;

-- operators and conversions
SELECT '0'::base64_4b < '0'::base64_4b;
SELECT '0'::base64_4b > '0'::base64_4b;
SELECT '0'::base64_4b < '1'::base64_4b;
SELECT '0'::base64_4b > '1'::base64_4b;
SELECT '0'::base64_4b <= '0'::base64_4b;
SELECT '0'::base64_4b >= '0'::base64_4b;
SELECT '0'::base64_4b <= '1'::base64_4b;
SELECT '0'::base64_4b >= '1'::base64_4b;
SELECT '0'::base64_4b <> '0'::base64_4b;
SELECT '0'::base64_4b <> '1'::base64_4b;
SELECT '0'::base64_4b = '0'::base64_4b;
SELECT '0'::base64_4b = '1'::base64_4b;

-- COPY FROM/TO
TRUNCATE base64_4bs;
COPY base64_4bs(base64_4b) FROM STDIN;
+
zzzzz
\.
COPY base64_4bs TO STDOUT;

-- clean up --
DROP TABLE base64_4bs;

-- errors
SELECT ''::base64_4b;
SELECT 'zzzzzz'::base64_4b;
SELECT '!'::base64_4b;
