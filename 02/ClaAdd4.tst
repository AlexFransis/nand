// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/02/Add16.tst

load ClaAdd4.hdl,
output-file ClaAdd4.out,
compare-to ClaAdd4.cmp,
output-list a%B1.4.1 b%B1.4.1 cin%B2.1.2 out%B1.4.1 carry%B3.1.3;

set a %B0000,
set b %B0000,
set cin 0,
eval,
output;

set a %B0000,
set b %B1111,
set cin 0,
eval,
output;

set a %B1111,
set b %B1111,
set cin 0,
eval,
output;

set a %B1010,
set b %B0101,
set cin 0,
eval,
output;

set a %B0011,
set b %B0001,
set cin 0,
eval,
output;

set a %B0001,
set b %B1001,
set cin 0,
eval,
output;

set a %B0010,
set b %B0001,
set cin 0,
eval,
output;

set a %B0001,
set b %B1001,
set cin 0,
eval,
output;

set a %B1101,
set b %B1011,
set cin 0,
eval,
output;

set a %B0000,
set b %B1111,
set cin 1,
eval,
output;
