load Mux4Way.hdl,
output-file Mux4Way.out,
compare-to Mux4Way.cmp,
output-list a%B3.1.3 b%B3.1.3 c%B3.1.3 d%B3.1.3 sel%B2.2.2 out%B3.1.3;

set a 0,
set b 0,
set c 0,
set d 0,
set sel 0,
eval,
output;

set sel 1,
eval,
output;

set sel 2,
eval,
output;

set sel 3,
eval,
output;

set a %B1,
set b %B0,
set c %B0,
set d %B0,
set sel 0,
eval,
output;

set a %B0,
set b %B1,
set c %B0,
set d %B0,
set sel 1,
eval,
output;

set a %B0,
set b %B0,
set c %B1,
set d %B0,
set sel 2,
eval,
output;

set a %B0,
set b %B0,
set c %B0,
set d %B1,
set sel 3,
eval,
output;

set a %B0,
set b %B1,
set c %B1,
set d %B1,
set sel 0,
eval,
output;

set a %B1,
set b %B1,
set c %B1,
set d %B0,
set sel 3,
eval,
output;
