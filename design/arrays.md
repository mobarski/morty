# Arrays

## Sandbox - top

### brackets as separator

```
# [ and ] acts only as visual separator
def swap-arr ( a x y -- ) :y :x :a
    a[x] .get (tmp)
    a[y] .get a[x] .set
    (tmp) a[y] .set
end
```

### new-word th not reversed
```
def swap-arr ( a x y -- ) :y :x :a
    a x th (tmp)
    a y th a x th-set
    (tmp) a y th-set
end
```

### method
```
def swap-arr ( a x y -- ) :y :x :a
    x a.get (tmp)
    y a.get x a.set
    (tmp) y a.set
end
```



## Sandbox - other



### add get (OLD)
```
def swap-arr ( a x y -- ) :y :x :a
    a x add get (tmp)
    a y add get a x add set
    (tmp) a y add set
end
```

### square brackets
```
# = suggests that any operation can be made inside brackets -> should be allowed 
# + most common array notation
# - conflict with lambda -> arrays are more important
def swap-arr ( a x y -- ) :y :x :a
    a[x] get (tmp)
    a[y] get a[x] set
    (tmp) a[y] set
end
```

### dot
```
def swap-arr ( a x y -- ) :y :x :a
    a.x get (tmp)
    a.y get a.x set
    (tmp) a.y set
end
```

### new-word [] not reversed
```
def swap-arr ( a x y -- ) :y :x :a
    a x []get (tmp)
    a y []get a x []set
    (tmp) a y []set
end
```

### new-word [] reversed
```
def swap-arr ( a x y -- ) :y :x :a
    x a []get (tmp)
    y a []get x a []set
    (tmp) y a []set
end
```

### new-word . not reversed
```
def swap-arr ( a x y -- ) :y :x :a
    a x .get (tmp)
    a y .get a x .set
    (tmp) a y .set
end
```

### new-word . reversed
```
def swap-arr ( a x y -- ) :y :x :a
    x a .get (tmp)
    y a .get x a .set
    (tmp) y a .set
end
```

### new-word COMBO . not reversed
```
def swap-arr ( a x y -- ) :y :x :a
    a x.get (tmp)
    a y.get a x.set
    (tmp) a y.set
end
```

### other
```
def swap-arr ( a x y -- ) :y :x :a
    a x []@ (tmp)
    a y []@ a x []!
    (tmp) a y []!
end


# colon
def swap-arr ( a x y -- ) :y :x :a
    a:x get (tmp)
    a:y get a:x set
    (tmp) a:y set
end

# comma
def swap-arr ( a x y -- ) :y :x :a
    a,x get (tmp)
    a,y get a,x set
    (tmp) a,y set
end

# plus
def swap-arr ( a x y -- ) :y :x :a
    a+x get (tmp)
    a+y get a+x set
    (tmp) a+y set
end

```
