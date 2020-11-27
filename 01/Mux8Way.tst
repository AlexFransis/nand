load Mux8Way.hdl,
output-file Mux8Way.out,
compare-to Mux8Way.cmp,
output-list a%B1.1.1 b%B1.1.1 c%B1.1.1 d%B1.1.1 e%B1.1.1 f%B1.1.1 g%B1.1.1 h%B1.1.1 sel%B1.3.1 out%B2.1.2;

set a 0,
set b 0,
set c 0,
set d 0,
set e 0,
set f 0,
set g 0,
set h 0,
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

set sel 4,
eval,
output;

set sel 5,
eval,
output;

set sel 6,
eval,
output;

set sel 7,
eval,
output;

set a 1,
set b 0,
set c 0,
set d 0,
set e 0,
set f 0,
set g 0,
set h 0,
set sel 0,
eval,
output;

set a 0,
set b 1,
set c 0,
set d 0,
set e 0,
set f 0,
set g 0,
set h 0,
set sel 1,
eval,
output;

set a 0,
set b 0,
set c 1,
set d 0,
set e 0,
set f 0,
set g 0,
set h 0,
set sel 2,
eval,
output;

set a 0,
set b 0,
set c 0,
set d 1,
set e 0,
set f 0,
set g 0,
set h 0,
set sel 3,
eval,
output;

set a 0,
set b 0,
set c 0,
set d 0,
set e 1,
set f 0,
set g 0,
set h 0,
set sel 4,
eval,
output;

set a 0,
set b 0,
set c 0,
set d 0,
set e 0,
set f 1,
set g 0,
set h 0,
set sel 5,
eval,
output;

set a 0,
set b 0,
set c 0,
set d 0,
set e 0,
set f 0,
set g 1,
set h 0,
set sel 6,
eval,
output;

set a 0,
set b 0,
set c 0,
set d 0,
set e 0,
set f 0,
set g 0,
set h 1,
set sel 7,
eval,
output;

set a 1,
set b 1,
set c 0,
set d 0,
set e 1,
set f 0,
set g 0,
set h 0,
set sel 6,
eval,
output;

set a 0,
set b 0,
set c 1,
set d 0,
set e 0,
set f 1,
set g 1,
set h 0,
set sel 7,
eval,
output;
