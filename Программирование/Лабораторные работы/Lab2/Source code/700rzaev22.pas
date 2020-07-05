program lab2_2;
//variant 700
const LEN =  999;
var n, m, i, j : integer;
    t 		   : real;
    matr	   : array [0..LEN, 0..LEN] of real;
begin
	writeln('Enter the dimensions of matrix');
	n := 0; m := 0;
	while ((n < 1) or (n > LEN + 1) or
		   (m < 1) or (m > LEN + 1) ) do
	begin
		writeln('Enter the two numbers between 1 and ', LEN + 1);
		read(n, m);
	end;

for i := 0 to n - 1 do
	for j := 0 to m - 1 do
	begin
		read(t);
		matr[i, j] := sqr(t);
	end;

writeln;

for i := 0 to n - 1 do
begin
	for j := 0 to m - 1 do
	begin
		t := matr[i, j];
		write(t:3:2, ' ');
	end;
	writeln;
end;

readln;
end.
		 
