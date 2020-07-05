uses math;
const start = -9;
      endt = 3;
      step = 0.2;
var x, y : real;
begin
        x := start;
        while (x < endt + step) do
        begin
                if (x < -7 - step) then
                        y := tan(x) - x * x * x

                else if ((x >= -7 - step) and (x < 0.0)) then
                        y := -1 / exp(0.1 * (-x) * ln(-x)) / 13

                else if (x = 0.0) then
                        y := 1 / 13

                else if ((x > 0.0) and (x < 1.0)) then
                        y := exp(0.1 * x * ln(x)) / 13

                else if (x >= 1.0) then
                        y := 53 * x - exp(x);

		    writeln('x = ', x:5:1, ' y = ', y:5:3);

                x := x + step;
        end;
end.

