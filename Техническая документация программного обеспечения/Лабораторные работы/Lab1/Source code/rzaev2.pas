program rzaev2;
var
   list : array [1..10] of Integer;
   i, min : Integer;
begin
     writeln('Введите 10 чисел');
     for i := 1 to 10 do
         read(list[i]);
     min := list[1];
     for i := 2 to 10 do
         if list[i] < min then
            min := list[i];
     writeln('Минимальное из 10 чисел: ', min);
end.
