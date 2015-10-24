; Generates primes up to n, using Sieve of Eratosthenes
macro generate_primes n {
    local x, y, i, j, p, slimit, sbase, virtual_prime_bits
    virtual at 0
        virtual_prime_bits::
        ; 0, 1 - not primes
        ; 2, .. - probrably primes
        db 1111'1100b
        db 1 + (n - 1) / 8 dup 1111'1111b
        repeat (n + 1) / 8 + 1
            i = % - 1
            load x byte from i
            repeat 8
                j = % - 1
                p = i * 8 + j
                if p > n
                    break
                end if
                if x and (1 shl j)
                    sbase = p * p
                    slimit = 1 + (n - sbase) / p
                    if slimit > 0
                        repeat slimit
                            j = sbase + p * (% - 1)
                            load y byte from j / 8
                            y = y and (not (1 shl (j and 7)))
                            store byte y at j / 8
                        end repeat
                    end if
                end if
            end repeat
        end repeat
    end virtual
    repeat (n + 1) / 8 + 1
        i = % - 1
        load x byte from virtual_prime_bits:i
        repeat 8
            j = % - 1
            if x and (1 shl j)
                p = i * 8 + j
                if p > n
                    break
                end if
                dq p
                display d=p, 13, 10
            end if
        end repeat
    end repeat
}

