unit crand;

interface
  procedure srand(s: Longword);
  function rand: Longword;
  
implementation

//pseudo random generator
var
    seed: Longword;

procedure srand(s: Longword);
begin
    seed := s;
end;

function rand: Longword;
var
	next, result: Longword;
begin
	next := seed;

	next := next * 1103515245;
	next := next + 12345;
	result := (next div 65536) mod 2048;

	next := next * 1103515245;
	next := next + 12345;
	result := result shl 10;
	result := result xor ((next div 65536) mod 1024);

	next := next * 1103515245;
	next := next + 12345;
	result := result shl 10;
	result := result xor ((next div 65536) mod 1024);

	seed := next;

  rand := result;
end;
//

end.