# Local variables

```

def ack (mn--a) :n :m
    m 0 eq then n 1 add ret do
    n 0 eq then m 1 sub 1 ack ret do
    m 1 sub
        m n 1 sub ack
    ack
end

def ack | m n -- a |
    m 0 eq then n 1 add ret do
    n 0 eq then m 1 sub 1 ack ret do
    m 1 sub
        m n 1 sub ack
    ack
end

def ack ( m n -- a ) | m n |
    m 0 eq then n 1 add ret do
    n 0 eq then m 1 sub 1 ack ret do
    m 1 sub
        m n 1 sub ack
    ack
end

def ack | m n | (--a)
    m 0 eq then n 1 add ret do
    n 0 eq then m 1 sub 1 ack ret do
    m 1 sub
        m n 1 sub ack
    ack
end

def ack (mn--a) :m,n
    m 0 eq then n 1 add ret do
    n 0 eq then m 1 sub 1 ack ret do
    m 1 sub
        m n 1 sub ack
    ack
end

```

