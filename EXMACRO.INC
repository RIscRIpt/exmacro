macro align a, v {
    local b
    if a and (a - 1) ;Check if a is power of two
        display 'align value is not power of two!', 13, 10
        err
    end if
    b = ((-$) and (a - 1))
    if b
        if ~ v eq
            db b dup v
        else
            db b dup 0x90
        end if
    end if
}

macro diu x& {
    local matched
    matched = 0
    match l t, x \{
        matched = 1
        if used l
            l t
        else
            display 'Unused variable `', \`l, 13, 10
        end if
    \}
    if ~ matched
        display 'Unsupported usage of `dil`', 13, 10
        err
    end if
}

macro ifdef v {
    if defined v
}

macro diub l {
    if used l
        l:
    macro endb \{
        purge endb
        end if
    \}
}


macro class name {
    macro class_end \{
        purge class_end
        virtual at 0
            name name
        end virtual
    \}
    struc name \{
        .:
}

skip_to fix rb .-$+
endc fix } class_end

macro define_if_used name, type, [data] {
    common
    if type eq db
        label name byte
    else if type eq dw
        label name word
    else if type eq dd
        label name dword
    else if type eq df
        label name fword
    else if type eq dq
        label name qword
    else if type eq dt
        label name tword
    else
        label name
    end if
    if used name
        forward
            type data
        common
    end if
}

macro bytes name, [data] {
    common
    label name
    if used name
        forward
            db data
        common
    end if
    sizeof.#name = $ - name
}

macro string name, [data] {
    common
    label name
    if used name
        forward
            db data
        common
    end if
    db 0
    sizeof.#name = $ - name
}

macro wstring name, [data] {
    common
    label name
    if used name
        forward
            du data
        common
    end if
    du 0
    sizeof.#name = $ - name
}

macro mov_dbl_const dst, val {
    local hi, lo
    virtual at 0
        dq val
        load hi dword from 0
        load lo dword from 4
    end virtual
    mov dword[dst], hi
    mov dword[dst + 4], lo
}

macro jmpcall dest, [arg] { ; push dword arguments and jump to destination
    common
    if ~ arg eq
        reverse
        pushd dword arg
    common
    end if
    jmp dest
}

