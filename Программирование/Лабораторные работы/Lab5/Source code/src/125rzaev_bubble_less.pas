program bubble_less;
type cmp_t = function(a, b : Longint):Integer;
     array_t = array [0..1000100] of Longint;

function cmp(a, b : Longint):Integer;
begin
    if a = b then
        cmp := 0
    else if a < b then
        cmp := -1
    else if a > b then
        cmp := 1;
end;

procedure swap(var a, b : Longint);
var t : Longint;
begin
    t := a;
    a := b;
    b := t;
end;

procedure bubble_sort(var arr : array_t; sz : Longint; cmp : cmp_t);
var ch : Boolean;
    i, j : Longint;
begin
    j := sz - 1;
    ch := True;
    while ch or (j > 0) do
    begin
        ch := False;
        for i := 1 to j do
            if cmp(arr[i - 1], arr[i]) = 1 then
            begin
                swap(arr[i - 1], arr[i]);
                ch := True;
            end;
        dec(j);
    end;
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
    bubble_sort(s, n, @cmp);
    for i := 0 to n - 1 do
        write(out, s[i], ' ');
    close(inp);
    close(out);
end.

