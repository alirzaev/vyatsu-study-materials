uses crt;

const max_size = 1000;

type value_type = Integer;

type seq = array[1..max_size] of value_type;

type stack = record
 	size : Integer;
 	container : seq;
end;

procedure stack_init(var stack_ : stack);
begin
	stack_.size := 0;
end;

procedure stack_push(var stack_ : stack; value_ : value_type);
begin
	inc(stack_.size);
	stack_.container[stack_.size] := value_;
end;

procedure stack_pop(var stack_ : stack);
begin
	dec(stack_.size);
end;

function stack_peek(stack_ : stack): value_type;
begin
	stack_peek := stack_.container[stack_.size];
end;

function stack_empty(stack_ : stack): Boolean;
begin
	if (stack_.size = 0) then
		stack_empty := True
	else
		stack_empty := False;
end;

function stack_size(stack_ : stack): Integer;
begin
	stack_size := stack_.size;
end;

function get_cmd(): Integer;
var code : Integer;
begin
	writeln('What do you want to do:', chr(10), chr(13),
			'1. Push element in the stack', chr(10), chr(13),
			'2. Pop element from the stack', chr(10), chr(13),
			'3. Get top element of the stack', chr(10), chr(13),
			'4. Check the stack for empty', chr(10), chr(13),
			'5. Exit');
	readln(code);
	get_cmd := code;
end;

var astack : stack;
	cmd : Integer;
	value : value_type;
begin
	stack_init(astack);
	cmd := 0;
	while (cmd <> 5) do
	begin
		clrscr;
		cmd := get_cmd();
		case cmd of
			1 : 
			begin
				writeln('Enter a value');
				readln(value);
				if (stack_size(astack) = max_size) then
					writeln('Stack is completely filled')
				else
				begin
					stack_push(astack, value);
					writeln('Value is put');
				end;
			end;
			2 : 
			begin
				if (stack_empty(astack)) then
					writeln('Stack is already empty')
				else
				begin
					stack_pop(astack);
					writeln('Element poped');
				end;
			end;
			3 :
			begin
				if (stack_empty(astack)) then
					writeln('Stack is already empty')
				else
				begin
					writeln('The value of the peek of the stack is: ');
					value := stack_peek(astack);
					writeln(value);
				end;
			end;
			4 : 
			begin
				if (stack_empty(astack)) then
					writeln('Stack is empty')
				else
					writeln('Stack is not empty');
			end;
			5 : 
				writeln('Programm will be closed');
			else
				writeln('Unknown command!');
		end;
		writeln('Press Enter to continue');
		readln;
	end;
end.
