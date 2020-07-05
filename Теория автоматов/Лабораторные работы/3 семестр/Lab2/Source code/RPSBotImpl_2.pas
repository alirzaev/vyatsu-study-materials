unit RPSBotImpl;

{$mode objfpc}{$H+}

interface

const

    UNKNOWN = 0;

    ROCK = 1;

    SCISSORS = 2;

    PAPER = 3;

    procedure setParams(setCount, winsPerSet: Integer);
    function choice(previousRivalChoice: Integer): Integer;
    procedure onCurrentGameEnd;
    
    // helper functions
    function produceChoice(rivalP: Integer): Integer;
    procedure srand(s: Longword);
    function rand: Integer;

implementation

var
    P: array [1..3, 1..3] of Longword;
    prevPrevRival, rnd: Integer;

//pseudorandom generator
const RAND_MAX = 32767;
var
    seed: Longword;

procedure srand(s: Longword);
var
    tmp: ^Longword;
begin
    New(tmp);
    seed := tmp^ xor s;
    Dispose(tmp);
end;

function rand: Integer;
var
    next, res: Longword;
begin
    next := seed;

    next := next * 1103515245;
    next := next + 12345;
    res := (next div 65536) mod 2048;

    next := next * 1103515245;
    next := next + 12345;
    res := res shl 10;
    res := res xor ((next div 65536) mod 1024);

    next := next * 1103515245;
    next := next + 12345;
    res := res shl 10;
    res := res xor ((next div 65536) mod 1024);

    seed := next;

    rand := res and RAND_MAX;
end;
//
    
function maxP(choice: Integer): Integer;
var
    i, m : Integer;
begin
    m := 1;
    for i := 2 to 3 do
        if P[choice][m] < P[choice][i] then
            m := i;
    maxP := m;
end;

function produceChoice(rivalP: Integer): Integer;
begin
    if rivalP = ROCK then
        produceChoice := PAPER
    else if rivalP = SCISSORS then
        produceChoice := ROCK
    else
        produceChoice := SCISSORS;
end;

procedure setParams(setCount, winsPerSet: Integer);
var
    i, j: Integer;
begin
    srand(9931);
    
    prevPrevRival := rand mod 3 + 1;

    for i := 1 to 3 do
        for j := 1 to 3 do
            P[i][j] := (rand mod 10 + 1) xor rnd;
end;

function choice(previousRivalChoice: Integer): Integer;
begin
    if previousRivalChoice = UNKNOWN then
        previousRivalChoice := rand mod 3 + 1;

    Inc(P[prevPrevRival][previousRivalChoice]);
    
    prevPrevRival := previousRivalChoice;
    choice := produceChoice(maxP(previousRivalChoice));
    
    Inc(rnd);
end;

procedure onCurrentGameEnd;
begin
end;

end.