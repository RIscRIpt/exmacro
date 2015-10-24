;Binary search of value with specific type in a specific region [left; right) - right bound not included
;Searches for the left most value, which is equal to `value`
;Returns (sets bisearch_result) address of first value, which is greater or equal to `value`
macro bisearch value, type, left, right {
    local x, l, m, r, base
    if type eq byte
        size = 1
    else if type eq word
        size = 2
    else if type eq dword
        size = 4
    else if type eq qword
        size = 8
    else
        display 'Invalid type: `', `type, 13, 10
        err
    end if
    l = 0
    r = (right - left - 1) / size
    while l <= r
        m = (l + r) shr 1
        load x type from left + m * size
        if value <= x
            r = m - 1
        else
            l = m + 1
        end if
    end while
    bisearch_result = left + l * size
}

