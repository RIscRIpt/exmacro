macro ipv4 a,b,c,d,p {
    db d, c, b, a
    if ~ p eq
        dd p
    end if
}
