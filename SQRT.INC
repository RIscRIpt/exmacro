;Taken from http://www.codecodex.com/wiki/Calculate_an_integer_square_root 
macro fasm_sqrt n {
    local root, rem, place
    root = 0
    rem = n
    place = 0x4000000000000000
    while place > rem
        place = place shr 2
    end while
    while place
        if rem >= root + place
            rem = rem - root - place
            root = root + place shl 1
        end if
        root = root shr 1
        place = place shr 2
    end while
    fasm_sqrt_result = root
}

