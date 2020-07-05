uses graph, wincrt;

const
    max_depth = 10;
    def_size = 150.0;
    step = 10.0;
    delta = 20.0;
    scale = 1.2;

type 
    Point = record
        x, y : Real;
    end;
    
    Square = record
        a, b, c, d : Point;
    end;

var size, old_size : Real;
    sh_x, sh_y : Real;
    old_sh_x, old_sh_y : Real;
    tpl, btr : Point;
    root : Square;

procedure update_root;
begin
    old_size := size;
    old_sh_x := sh_x;
    old_sh_y := sh_y;
end;

procedure outdate_root;
begin
    size := old_size;
    sh_x := old_sh_x;
    sh_y := old_sh_y;
end;

procedure print_help;
begin
    writeln('w - zoom in');
    writeln('s - zoom out');
    writeln('c - reset');
    writeln('ESC - exit');
end;

procedure zoom(m : Real);
begin
    size := size * m;
    sh_x := sh_x * m;
    sh_y := sh_y * m;
end;

procedure set_rect_points;
var
    w, h : Real;
begin
    w := 93 * size / 16;
    h := 125 * size / 32;
        
    tpl.x := - w / 2 + delta; // левый верхний
    tpl.y := - size / 2 + delta;
    
    btr.x := getmaxx + w / 2 - delta; // правый нижний
    btr.y := getmaxy + h - size / 2 - delta;
end;

function check_edges : Boolean;
var
    x_mid, y_mid : Real;
begin
    x_mid := getmaxx / 2 + sh_x;
    y_mid := getmaxy / 2 + sh_y;
    
    set_rect_points;
    
    if ( (x_mid >= tpl.x) and (x_mid <= btr.x) and
         (y_mid >= tpl.y) and (y_mid <= btr.y) ) then
        check_edges := True
    else
        check_edges := False;
end;

procedure set_root_points;
var
    x_mid, y_mid : Real;
begin
    x_mid := getmaxx / 2 + sh_x;
    y_mid := getmaxy / 2 + sh_y;
    
    root.a.x := x_mid - size / 2;
    root.a.y := y_mid + size / 2;
    
    root.b.x := x_mid + size / 2;
    root.b.y := y_mid + size / 2;
    
    root.c.x := x_mid + size / 2;
    root.c.y := y_mid - size / 2;
    
    root.d.x := x_mid - size / 2;
    root.d.y := y_mid - size / 2;
end;

procedure paint_square(sq : Square);
begin
    line( round(sq.a.x), round(sq.a.y), 
          round(sq.b.x), round(sq.b.y) );
    line( round(sq.b.x), round(sq.b.y), 
          round(sq.c.x), round(sq.c.y) );
    line( round(sq.c.x), round(sq.c.y), 
          round(sq.d.x), round(sq.d.y) );
    line( round(sq.d.x), round(sq.d.y), 
          round(sq.a.x), round(sq.a.y) );
end;
    
procedure paint_fractal(pr : Square; depth : Integer);
var
    x, y : Real;
    n1, n2, n3, n4 : Real;
    left, right : Square;
    e, f, g, h, i : Point;
begin
    if keypressed then
        exit;

    x := (pr.a.x + pr.b.x + pr.c.x + pr.d.x) / 4;
    y := (pr.a.y + pr.b.y + pr.c.y + pr.d.y) / 4;

    n1 := pr.d.x - pr.a.x;
    n4 := pr.d.y - pr.a.y;
    n2 := pr.d.x - pr.c.x;
    n3 := pr.d.y - pr.c.y;
    
    e.x := x + n1;          e.y := y + n4;
    f.x := pr.d.x + n1;     f.y := pr.d.y + n4;
    g.x := x + n1 + n2;     g.y := y + n4 + n3;
    h.x := pr.c.x + n1;     h.y := pr.c.y + n4;
    i.x := x + n1 - n2;     i.y := y + n4 - n3;
    
    left.a := pr.d; left.b := e;
    left.c := f;        left.d := g;
    
    right.a := e;   right.b := pr.c;
    right.c := i;   right.d := h;
    
    //setcolor(depth mod 16);
    paint_square(pr);
    
    if depth <= max_depth then
    begin
        paint_fractal(left, depth + 1);
        paint_fractal(right, depth + 1);
    end;
end;

procedure reset_fractal;
begin
    size := def_size; 
    old_size := def_size;
    sh_x := 0;   old_sh_x := 0;
    sh_y := 100; old_sh_y := 100;
end;

procedure repl;
var cmd : Char;
    switched : Boolean;
    gpath : String;
    gdriver, gmode : SmallInt;
begin
    gpath := '';
    detectgraph(gdriver, gmode);
    initgraph(gdriver, gmode, gpath);
    reset_fractal;
    switched := True;
    repeat
        if switched then
        begin
            if check_edges then
            begin
                set_root_points;
                cleardevice;
                paint_fractal(root, 1);
                update_root;
            end
            else
            begin
                outdate_root;
            end;
        end;
        cmd := readkey;
        switched := True;
        if ord(cmd) = 0 then
        begin
            case ord(readkey) of
            72 : // вверх
                sh_y := sh_y + step;
            80 : // вниз
                sh_y := sh_y - step;
            75 : // влево
                sh_x := sh_x + step;
            77 : // вправо
                sh_x := sh_x - step;
            else
                switched := False;
            end;
        end
        else 
            case ord(cmd) of
            119 : // w - увеличить
                zoom(scale);
            115 : // s - уменьшить
                zoom(1 / scale);
            99 : // c - сброс
                reset_fractal;
            27 : // ESC - выйти
                ;
            else
                switched := False;
            end;
    until ord(cmd) = 27;
    closegraph;
end;

begin
    print_help;
    repl;
end.
