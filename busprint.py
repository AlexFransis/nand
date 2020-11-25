# 4Way16BitMux
#for i in range(16):
#    print(f'''
#    Nand(a=sel[0], b=sel[0], out=notsel0a{i});
#    Nand(a=sel[0], b=sel[0], out=notsel0c{i});
#    Nand(a=sel[1], b=sel[1], out=notsel1a{i});
#    Nand(a=sel[1], b=sel[1], out=notsel1b{i});
#
#    Nand(a=notsel0a{i}, b=notsel1a{i}, out=nandnotsel0notsel1{i});
#    Nand(a=nandnotsel0notsel1{i}, b=nandnotsel0notsel1{i}, out=andnotsel0notsel1{i});
#    Nand(a=a[{i}], b=andnotsel0notsel1{i}, out=nandanotsel0notsel1{i});
#    Nand(a=nandanotsel0notsel1{i}, b=nandanotsel0notsel1{i}, out=andanotsel0notsel1{i});
#
#    Nand(a=notsel1b{i}, b=sel[0], out=nandnotsel0sel1{i});
#    Nand(a=nandnotsel0sel1{i}, b=nandnotsel0sel1{i}, out=andnotsel0sel1{i});
#    Nand(a=b[{i}], b=andnotsel0sel1{i}, out=nandbnotsel0sel1{i});
#    Nand(a=nandbnotsel0sel1{i}, b=nandbnotsel0sel1{i}, out=andbnotsel0sel1{i});
#
#    Nand(a=sel[1], b=notsel0c{i}, out=nandsel0notsel1{i});
#    Nand(a=nandsel0notsel1{i}, b=nandsel0notsel1{i}, out=andsel0notsel1{i});
#    Nand(a=c[{i}], b=andsel0notsel1{i}, out=nandcsel0notsel1{i});
#    Nand(a=nandcsel0notsel1{i}, b=nandcsel0notsel1{i}, out=andcsel0notsel1{i});
#
#    Nand(a=sel[0], b=sel[1], out=nandsel0sel1{i});
#    Nand(a=nandsel0sel1{i}, b=nandsel0sel1{i}, out=andsel0sel1{i});
#    Nand(a=d[{i}], b=andsel0sel1{i}, out=nanddsel0sel1{i});
#    Nand(a=nanddsel0sel1{i}, b=nanddsel0sel1{i}, out=anddsel0sel1{i});
#
#    Nand(a=andanotsel0notsel1{i}, b=andanotsel0notsel1{i}, out=or1{i});
#    Nand(a=andbnotsel0sel1{i}, b=andbnotsel0sel1{i}, out=or2{i});
#    Nand(a=or1{i}, b=or2{i}, out=orab{i});
#
#    Nand(a=orab{i}, b=orab{i}, out=or3{i});
#    Nand(a=andcsel0notsel1{i}, b=andcsel0notsel1{i}, out=or4{i});
#    Nand(a=or3{i}, b=or4{i}, out=orabc{i});
#
#    Nand(a=orabc{i}, b=orabc{i}, out=or5{i});
#    Nand(a=anddsel0sel1{i}, b=anddsel0sel1{i}, out=or6{i});
#    Nand(a=or5{i}, b=or6{i}, out=out[{i}]);''')



# 8Way16BitMux
