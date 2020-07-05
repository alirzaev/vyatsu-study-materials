program qsort_greater;
type cmp_t = function(a, b : Longint):Integer;
     array_t = array [0..1000100] of Longint;

function cmp(a, b : Longint):Integer;
begin
    if a = b then
        cmp := 0
    else if a < b then
        cmp := 1
    else if a > b then
        cmp := -1;
end;

procedure swap(var a, b : Longint);
var t : Longint;
begin
    t := a;
    a := b;
    b := t;
end;

procedure qsort(var arr : array_t; l, r: Longint; cmp : cmp_t);
var    i, j : Longint;
       m : Longint;
begin
    m := arr[l + (r - l) div 2];
    i := l;
    j := r;
    while i <= j do
    begin
        while cmp(arr[i], m) = -1 do inc(i);
        while cmp(arr[j], m) = 1 do dec(j);
        if i <= j then
        begin
            swap(arr[i], arr[j]);
            inc(i);
            dec(j);
        end;
    end;

    if i < r then
        qsort(arr, i, r, cmp);
    if j > l then
        qsort(arr, l, j, cmp);
end;

var inp, out : TextFile;
    s : array_t;
    n, i : Longint;
begin
    assign(inp, 'input.txt');
    assign(out, 'output.txt');
    reset(inp);
    rewrite(out);
    readln(inp, n);
    for i := 0 to n - 1 do
        read(inp, s[i]);
    qsort(s, 0, n - 1, @cmp);
    for i := 0 to n - 1 do
        write(out, s[i], ' ');
    close(inp);
    close(out);
end.

