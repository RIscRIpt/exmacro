include 'PRINT.INC'

macro display_line token* {
    match pattern, token \{
        irps value, pattern \\{
            if \\`value eq \\value
                display 39, \\`value, 39
            else
                display \\`value\\#" "
            end if
        \\}
    \}
}

macro align a, v {
    local b
    if a and (a - 1) ;Check if a is power of two
        display 'align value ', d=v, ' is not power of two!', 13, 10
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

macro assert_align a {
    local b
    if a and (a - 1) ;Check if a is power of two
        display 'align value ', d=v, ' is not power of two!', 13, 10
        err
    end if
    b = ((-$) and (a - 1))
    if b
        display ' : assertion of align by ', d=a, ' failed! (', h=$, ')', 13, 10
        err
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
            ;display 'Unused variable `', \`l, 13, 10
        end if
    \}
    if ~ matched
        ;display 'Unsupported usage of `dil`', 13, 10
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
        else
            ;display 'Unused block `', \`l, 13, 10
        end if
        purge endb
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

; Refactored macros `interface` of Abyx : http://board.flatassembler.net/topic.php?p=107946
macro vtable name_parent, [methods] {
    common
        local has_parent
        has_parent equ
        match name:parent, name_parent \{
            _vtable name, parent, methods
            has_parent equ 1
        \}
        match , has_parent \{
            _vtable name_parent,, methods
            restore has_parent
        \}
        restore has_parent
}

macro _vtable name, parent, [methods] {
    common
        macro _#name#_vtable base \{
            match any, parent \\{
                _#parent#_vtable base
            \\}
    forward
            base\#.#methods dd ?
    common
        \}

        virtual at 0
            _#name#_vtable name
        end virtual
}

macro array type, name, [data] {
    common
    label name
    if used name
        forward
            type data
        common
        sizeof.#name = $ - name
    else
        ;display 'Unused array of ', `type, ' `', `name, '`', 13, 10
    end if
}

macro sized_array size_type, type, name, [data] {
    common
    label name
    if used name
        virtual at 0
            forward
                type data
            common
            sizeof.#name = $
        end virtual
        size_type sizeof.#name
        forward
            type data
        common
    else
        ;display 'Unused array of ', `type, ' `', `name, '`', 13, 10
    end if
}

macro bytes name, data& {
    array db, name, data
}

macro string name, data& {
    array db, name, data, 0
}

macro pstring name, data& {
    sized_array db, db, name, data
}

macro wstring name, data& {
    array du, name, data, 0
}

macro pwstring name, data& {
    sized_array dw, du, name, data
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


macro adwrap i, a {
    if __adwrap_init eq i & __adwrap_align = a
    macro endaw \{ end if \}
}

macro adinclude filename, init, [nalign] {
    forward
        __adwrap_init equ init
        __adwrap_align = nalign
        align nalign, ?
        include filename
        assert_align nalign
        restore __adwrap_init
}


macro make_list_definer name, pre_defines, pre_define, post_define, post_defines {
    local list, define_list_item
    list equ
    macro name item& \{
        local _
        macro _ \\{ item \\}
        match x, list \\{ list equ list,_ \\}
        match  , list \\{ list equ _ \\}
    \}
    macro name#\_ \{
        match x, list \\{ list equ list,\_ \\}
        match  , list \\{ list equ \_ \\}
    \}
    macro define_#name#s \{ match _, list \\{ define_list_item _ \\} \}
    macro define_list_item [item] \{
        common
            if ~ pre_defines eq
                pre_defines
            end if
        forward
            if ~ pre_define eq
                pre_define item
            end if
            item
            if ~ post_define eq
                post_define item
            end if
            purge item
        common
            if ~ post_defines eq
                post_defines
            end if
    \}
}

;wrap_ fix macro _ {
;_wrap fix }
;
;macro make_adata_definer name, _align {
;    macro __pre_define_#name#s \{ align _align, ? \}
;    macro __post_define_#name item \{
;        assert_align _align, item
;    \}
;    make_list_definer name, __pre_define_#name#s,, __post_define_#name
;}
;
;macro define_adata_definers prefix, max_align {
;    if max_align and (max_align - 1) ;Check if a is power of two
;        display 'align value ', d=max_align, ' is not power of two!', 13, 10
;        err
;    end if
;    rept max_align cur_align:1 \{
;        if ~ cur_align and (cur_align - 1)
;            make_adata_definer prefix\#\cur_align, cur_align
;        end if
;    \}
;}
;
;macro ninclude filename {
;    local ..
;    ..:
;        include filename
;    if $ - .. > 0
;        display 'ninclude: "', filename, '" tried to define some data!', 13, 10
;        ;err
;    end if
;}
;
