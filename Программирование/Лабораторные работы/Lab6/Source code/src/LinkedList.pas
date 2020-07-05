unit LinkedList;

interface

type
    TValue = Integer; 
    
    TNode = record
		value : TValue;
		next : ^TNode;
    end;
    
    TLinkedList = record
		first : ^TNode;
    end;

procedure list_create(var list : TLinkedList);

procedure list_delete(var list : TLinkedList);

function list_empty(list : TLinkedList) : Boolean;

procedure list_push_front(var list : TLinkedList; value : TValue);

procedure list_push_back(var list : TLinkedList; value : TValue);

procedure list_insert(var list : TLinkedList; n : Integer; value : TValue);

procedure list_push_after(var list : TLinkedList; after, value : TValue);

procedure list_remove(var list : TLinkedList; value : TValue);

procedure list_print(list : TLinkedList);

procedure list_help;

implementation

procedure list_create(var list : TLinkedList);
begin
    list.first := nil;
end;

procedure list_delete(var list : TLinkedList);
var
    node : ^TNode;
begin
    while (list.first <> nil) do
    begin
		node := list.first^.next;
		dispose(list.first);
		list.first := node;
    end;
end;

function list_empty(list : TLinkedList) : Boolean;
begin
    if list.first = nil then
		writeln('List is empty')
    else
		writeln('List is not empty');
    list_empty := list.first <> nil;
end;

procedure list_push_front(var list : TLinkedList; value : TValue);
var node : ^TNode;
begin
    new(node);
    node^.value := value;
    node^.next := list.first;
    list.first := node;
end;

procedure list_push_back(var list : TLinkedList; value : TValue);
type
	TPtr = ^TNode;
var
    node : ^TNode;
	head : ^TPtr;
begin
    new(node);
    node^.value := value;
    node^.next := nil;	
	head := @list.first;
	while head^ <> nil do
	begin
		head := @(head^)^.next;
	end;
	head^ := node;
end;

procedure list_insert(var list : TLinkedList; n : Integer; value : TValue);
type
	TPtr = ^TNode;
var
    node : ^TNode;
	head : ^TPtr;
    i : Integer;
begin
    i := 1;
	new(node);
	node^.value := value;
	node^.next := nil;
	head := @list.first;
	while ( (head^ <> nil) and (i < n) ) do
	begin
		head := @(head^)^.next;
		inc(i);
	end;
	node^.next := head^;
	head^ := node;
end;

procedure list_remove(var list : TLinkedList; value : TValue);
type
	TPtr = ^TNode;
var
    node : ^TNode;
	head : ^TPtr;
begin
	head := @list.first;
	while ( (head^ <> nil) and ((head^)^.value <> value) ) do
		head := @(head^)^.next;
	
	if (head^ = nil) then
		writeln('No such value in list')
	else
	begin
		node := (head^)^.next;
		dispose(head^);
		head^ := node;
	end;
end;

procedure list_push_after(var list : TLinkedList; after, value : TValue);
type
	TPtr = ^TNode;
var
    node : ^TNode;
	head : ^TPtr;
begin
	head := @list.first;
	while ( (head^ <> nil) and ((head^)^.value <> after) ) do
		head := @(head^)^.next;
		
	if (head^ <> nil) then
	begin
		new(node);
		node^.value := value;
		node^.next := (head^)^.next;
		(head^)^.next := node;
	end
	else
		writeln('No such value in list');
end;

procedure list_print(list : TLinkedList);
var
    node : ^TNode;
begin
	node := list.first;
	while (node <> nil) do
	begin
		write(node^.value, ' ');
		node := node^.next;
	end;
	writeln;
end;

procedure list_help;
begin
	writeln('help - Show this message');
	writeln('pushfront <value> - Insert element at beginning');
	writeln('pushback <value> - Add element at the end');
	writeln('pushafter <value1> <value2> - Insert element value2 after value1');
	writeln('insert <pos> <value> - Insert element before the element at the pos');
	writeln('empty - Test whether list is empty');
	writeln('print - Print all elements stored in list');
	writeln('remove <value> - Remove element with specific value');
	writeln('clear - Clear content');
	writeln('exit - Exit the pragramm');
end;

end.
