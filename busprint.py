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





#    (or a b c d e f g h)
#    A: andanotsel012{i}
#    B: andbsel0notsel12{i}
#    C: andcsel1notsel02{i}
#    D: anddsel0sel1notsel2{i}
#    E: andenotselnotsel1sel2{i}
#    F: andfsel0notselsel2{i}
#    G: andgnotsel0sel1sel2{i}
#    H: andhsel0sel1sel2{i}

# 8Way16BitMux
print(f'''
    /* FINAL SOLUTION
        (or
            (and a (not sel0) (not sel1) (not sel2))
            (and b sel0 (not sel1) (not sel2))
            (and c (not sel0) sel1 (not sel2))
            (and d sel0 sel1 (not sel2))
            (and e (not sel0) (not sel1) sel2)
            (and f sel0 (not sel1) sel2)
            (and g (not sel0) sel1 sel2)
            (and h sel0 sel1 sel2))
    */

    Nand(a=sel[0], b=sel[0], out=notsel0);
    Nand(a=sel[1], b=sel[1], out=notsel1);
    Nand(a=sel[2], b=sel[2], out=notsel2); ''')
for i in range(16):
    print(f'''
    /* INPUT A */
    /* (and (not sel0) (not sel1)) */
    Nand(a=notsel0, b=notsel1, out=nandnotsel0notsel1{i});
    Nand(a=nandnotsel0notsel1{i}, b=nandnotsel0notsel1{i}, out=andnotsel01{i});

    /* (and (not sel0) (not sel1) (not sel2)) */
    Nand(a=notsel2, b=andnotsel01{i}, out=nandnotsel012{i});
    Nand(a=nandnotsel012{i}, b=nandnotsel012{i}, out=andnotsel012{i});

    /* (and a (not sel0) (not sel1) (not sel2)) */
    Nand(a=a[{i}], b=andnotsel012{i}, out=nandanotsel012{i});
    Nand(a=nandanotsel012{i}, b=nandanotsel012{i}, out=andanotsel012{i});

    /* INPUT B */
    /* (and (not sel1) (not sel2)) */
    Nand(a=notsel1, b=notsel2, out=nandnotsel1notsel2{i});
    Nand(a=nandnotsel1notsel2{i}, b=nandnotsel1notsel2{i}, out=andnotsel12{i});

    /* (and sel0 (not sel1) (not sel2)) */
    Nand(a=sel[0], b=andnotsel12{i}, out=nandsel0notsel12{i});
    Nand(a=nandsel0notsel12{i}, b=nandsel0notsel12{i}, out=andsel0notsel12{i});

    /* (and b sel0 (not sel1) (not sel2)) */
    Nand(a=b[{i}], b=andsel0notsel12{i}, out=nandbandsel0notsel12{i});
    Nand(a=nandbandsel0notsel12{i}, b=nandbandsel0notsel12{i}, out=andbsel0notsel12{i});

    /* INPUT C */
    /* (and (not sel0) (not sel2)) */
    Nand(a=notsel0, b=notsel2, out=nandnotsel02{i});
    Nand(a=nandnotsel02{i}, b=nandnotsel02{i}, out=andnotsel02{i});

    /* (and sel1 (not sel0) (not sel2)) */
    Nand(a=sel[1], b=andnotsel02{i}, out=nandsel1andnotsel02{i});
    Nand(a=nandsel1andnotsel02{i}, b=nandsel1andnotsel02{i}, out=andsel1notsel02{i});

    /* (and c sel1 (not sel0) (not sel2)) */
    Nand(a=c[{i}], b=andsel1notsel02{i}, out=nandcandsel1notsel02{i});
    Nand(a=nandcandsel1notsel02{i}, b=nandcandsel1notsel02{i}, out=andcsel1notsel02{i});

    /* INPUT D */
    /* (and sel1 (not sel2)) */
    Nand(a=sel[1], b=notsel2, out=nandsel1notsel2{i});
    Nand(a=nandsel1notsel2{i}, b=nandsel1notsel2{i}, out=andsel1notsel2{i});

    /* (and sel0 sel1 (not sel2)) */
    Nand(a=sel[0], b=andsel1notsel2{i}, out=nandsel0sel1notsel2{i});
    Nand(a=nandsel0sel1notsel2{i}, b=nandsel0sel1notsel2{i}, out=andsel0sel1notsel2{i});

    /* (and d sel0 sel1 (not sel2)) */
    Nand(a=d[{i}], b=andsel0sel1notsel2{i}, out=nanddsel0sel1notsel2{i});
    Nand(a=nanddsel0sel1notsel2{i}, b=nanddsel0sel1notsel2{i}, out=anddsel0sel1notsel2{i});

    /* INPUT E */
    /* (and (not sel0) (not sel1)) */
    Nand(a=notsel0, b=notsel1, out=nandnotsel01{i});
    Nand(a=nandnotsel01{i}, b=nandnotsel01{i}, out=andnotsel0notsel1{i});

    /* (and (not sel0) (not sel1) sel2) */
    Nand(a=sel[2], b=andnotsel0notsel1{i}, out=nandsel2notsel01{i});
    Nand(a=nandsel2notsel01{i}, b=nandsel2notsel01{i}, out=andnotsel0notsel1sel2{i});

    /* (and e (not sel0) (not sel1) sel2) */
    Nand(a=e[{i}], b=andnotsel0notsel1sel2{i}, out=nandenotsel01sel2{i});
    Nand(a=nandenotsel01sel2{i}, b=nandenotsel01sel2{i}, out=andenotselnotsel1sel2{i});

    /* INPUT F */
    /* (and sel0 (not sel1)) */
    Nand(a=sel[0], b=notsel1, out=nandsel0notsel1{i});
    Nand(a=nandsel0notsel1{i}, b=nandsel0notsel1{i}, out=andsel0notsel1{i});

    /* (and sel0 (not sel1) sel2) */
    Nand(a=sel[2], b=andsel0notsel1{i}, out=nandsel0notsel1sel2{i});
    Nand(a=nandsel0notsel1sel2{i}, b=nandsel0notsel1sel2{i}, out=andsel0notsel1sel2{i});

    /* (and f sel0 (not sel1) sel2) */
    Nand(a=f[{i}], b=andsel0notsel1sel2{i}, out=nandfsel0notselsel2{i});
    Nand(a=nandfsel0notselsel2{i}, b=nandfsel0notselsel2{i}, out=andfsel0notselsel2{i});

    /* INPUT G */
    /* (and (not sel0) sel1) */
    Nand(a=notsel0, b=sel[1], out=nandnotsel0sel1{i});
    Nand(a=nandnotsel0sel1{i}, b=nandnotsel0sel1{i}, out=andnotsel0sel1{i});

    /* (and (not sel0) sel1 sel2) */
    Nand(a=sel[2], b=andnotsel0sel1{i}, out=nandnotsel0sel1sel2{i});
    Nand(a=nandnotsel0sel1sel2{i}, b=nandnotsel0sel1sel2{i}, out=andnotsel0sel1sel2{i});

    /* (and g (not sel0) sel1 sel2) */
    Nand(a=g[{i}], b=andnotsel0sel1sel2{i}, out=nandgnotsel0sel1sel2{i});
    Nand(a=nandgnotsel0sel1sel2{i}, b=nandgnotsel0sel1sel2{i}, out=andgnotsel0sel1sel2{i});

    /* INPUT H */
    /* (and sel0 sel1) */
    Nand(a=sel[0], b=sel[1], out=nandsel0sel1{i});
    Nand(a=nandsel0sel1{i}, b=nandsel0sel1{i}, out=andsel0sel1{i});

    /* (and sel0 sel1 sel2) */
    Nand(a=andsel0sel1{i}, b=sel[2], out=nandsel0sel1sel2{i});
    Nand(a=nandsel0sel1sel2{i}, b=nandsel0sel1sel2{i}, out=andsel0sel1sel2{i});

    /* (and h sel0 sel1 sel2)) */
    Nand(a=h[{i}], b=andsel0sel1sel2{i}, out=nandhsel0sel1sel2{i});
    Nand(a=nandhsel0sel1sel2{i}, b=nandhsel0sel1sel2{i}, out=andhsel0sel1sel2{i});


    Nand(a=andanotsel012{i}, b=andanotsel012{i}, out=ora{i});
    Nand(a=andbsel0notsel12{i}, b=andbsel0notsel12{i}, out=orb{i});
    Nand(a=ora{i}, b=orb{i}, out=orab{i});

    Nand(a=orab{i}, b=orab{i}, out=nandorab{i});
    Nand(a=andcsel1notsel02{i}, b=andcsel1notsel02{i}, out=orc{i});
    Nand(a=nandorab{i}, b=orc{i}, out=orabc{i});

    Nand(a=orabc{i}, b=orabc{i}, out=nandorabc{i});
    Nand(a=anddsel0sel1notsel2{i}, b=anddsel0sel1notsel2{i}, out=ord{i});
    Nand(a=nandorabc{i}, b=ord{i}, out=orabcd{i});

    Nand(a=orabcd{i}, b=orabcd{i}, out=nandorabcd{i});
    Nand(a=andenotselnotsel1sel2{i}, b=andenotselnotsel1sel2{i}, out=ore{i});
    Nand(a=nandorabcd{i}, b=ore{i}, out=orabcde{i});

    Nand(a=orabcde{i}, b=orabcde{i}, out=nandorabcde{i});
    Nand(a=andfsel0notselsel2{i}, b=andfsel0notselsel2{i}, out=orf{i});
    Nand(a=nandorabcde{i}, b=orf{i}, out=orabcdef{i});

    Nand(a=orabcdef{i}, b=orabcdef{i}, out=nandorabcdef{i});
    Nand(a=andgnotsel0sel1sel2{i}, b=andgnotsel0sel1sel2{i}, out=org{i});
    Nand(a=nandorabcdef{i}, b=org{i}, out=orabcdefg{i});

    Nand(a=orabcdefg{i}, b=orabcdefg{i}, out=nandorabcdefg{i});
    Nand(a=andhsel0sel1sel2{i}, b=andhsel0sel1sel2{i}, out=orh{i});
    Nand(a=nandorabcdefg{i}, b=orh{i}, out=out[{i}]);''')
