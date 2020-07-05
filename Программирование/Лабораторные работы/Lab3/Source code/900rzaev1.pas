uses crt;
procedure readString(var input : String; msg : String);
begin
    writeln(msg);
    readln(input);
end;
function count(str, substr : String) : Integer;
var i, c, len, sublen : Integer;
begin
    c := 0;
    len := Length(str);
    sublen := Length(substr);
    for i := 1 to len - sublen + 1 do
        if (pos(substr, copy(str, i, sublen)) = 1) then
            inc(c);
    count := c;
end;
procedure printResult(str, substr : String; occurs : Integer);
begin
    writeln('The number of occurences of the string:');
    writeln('"', substr, '"');
    writeln('in the string:');
    writeln('"', str, '"');
    writeln('equals: ', occurs);
end;
var str, substr : String;
begin
    clrscr;
    readString(str, 'Enter a string');
    readString(substr, 'Enter a substring');
    printResult(str, substr, count(str, substr));
    readln;
end.

