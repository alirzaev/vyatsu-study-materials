program app2;
uses graph, wincrt;
const
  colours : array [1..5] of Word = (
    Red,
    Yellow,
    Blue,
    Green,
    White
  );

  types : array [1..5, 1..2] of Word = (
    (Solidln, NormWidth),
    (Dottedln, NormWidth),
    (Dashedln, NormWidth),
    (Centerln, NormWidth),
    (Solidln, ThickWidth)
  );

  line_length = 100;

function get_type : Integer;
var
  n : Integer;
begin
  writeln('Выберите тип линии:');
  writeln('1. Сплошная');
  writeln('2. Точками');
  writeln('3. Штрих');
  writeln('4. Штрих-пунктир');
  writeln('5. Двойной толщины');
  readln(n);
  get_type := n;
end;

function get_colour : Integer;
var
  n : Integer;
begin
  writeln('Выберите цвет линии:');
  writeln('1. Красный');
  writeln('2. Желтый');
  writeln('3. Синий');
  writeln('4. Зеленый');
  writeln('5. Белый');
  readln(n);
  get_colour := n;
end;

function get_arrows : Integer;
var
  a, b : Integer;
begin
  writeln('Наличие стрелок:');
  writeln('1 - стрелка есть, 0 - нет');
  writeln('Введите два числа, для задания наличия стрелок на концах линии');
  readln(a, b);
  get_arrows := a + b * 10;
end;

procedure get_points(var x1, y1, x2, y2 : Word);
begin
  x1 := (GetMaxX - line_length) div 2;
  x2 := (GetMaxX + line_length) div 2;
  y1 := GetMaxy div 2;
  y2 := GetMaxy div 2;
end;

procedure paint_line(t, c, a : Integer);
var
  n, x1, x2, y1, y2 : Word;
  sarrow, earrow : array [1..3] of PointType;
begin
  SetLineStyle(types[t][1], n, types[t][2]);
  SetColor(colours[c]);
  get_points(x1, y1, x2, y2);
  Line(x1, y1, x2, y2);
end;

procedure paint_arrows(t, c, a : Integer);
var
  n, x1, x2, y1, y2 : Word;
  sarrow, earrow : array [1..3] of PointType;
begin
  get_points(x1, y1, x2, y2);
  SetLineStyle(types[1][1], n, types[1][2]);
  SetFillStyle(SolidFill, colours[c]);

  sarrow[1].x := x1; sarrow[1].y := y1;
  sarrow[2].x := x1 + 10; sarrow[2].y := y1 - 10;
  sarrow[3].x := x1 + 10; sarrow[3].y := y1 + 10;

  earrow[1].x := x2; earrow[1].y := y2;
  earrow[2].x := x2 - 10; earrow[2].y := y2 - 10;
  earrow[3].x := x2 - 10; earrow[3].y := y2 + 10;

  if (a mod 10 = 1) then
    FillPoly(3, sarrow);
  if (a div 10 mod 10 = 1) then
    FillPoly(3, earrow);
end;

procedure paint;
var
  tl, cl, al : Integer;
  gpath : String;
  gdriver, gmode : SmallInt;
begin
  tl := get_type;
  cl := get_colour;
  al := get_arrows;
  gpath := '';
  detectgraph(gdriver, gmode);
  initgraph(gdriver, gmode, gpath);
  paint_line(tl, cl,al);
  paint_arrows(tl, cl, al);
  readkey;
end;

begin
  paint;
end.

