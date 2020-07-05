program lab2_1;
//variant 700
var N          : integer;
	var1, var2 : real;
	i, c1, c2  : integer;
begin
	writeln('Enter the number of elements');
	N := 0;
	while (N < 1) do  begin
		writeln('Enter the number more then zero');
		readln(N);
	end;
	   
	c1 := 0; c2 := 0;
	read(var1);
	for i := 1 to N - 1 do
	begin
		read(var2);
		if (var2 >= var1) then
			inc(c1);
		if (var2 <= var1) then
			inc(c2);
		var1 := var2;
	end;
	if (c1 = N - 1) or (c2 = N - 1) then
		writeln('Monotonous')
	else
		writeln('Nonmonotonic');
	readln;
end.
