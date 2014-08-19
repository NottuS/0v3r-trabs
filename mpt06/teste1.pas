program exemplo75 (input, output);
var m,n :  boolean;
var g,h :  boolean;
a,b,c,x,y:integer;
function p(lol,j:integer;a:boolean; var k,w:integer):integer;
    label a100;
    var z,zz,zzz:integer;
    function f:integer;
        function ed:integer;
            begin
                f:=1;
            end;
    begin
        f:=2;
        goto a100;
    end;
    begin
        a100: x:=1;
        f();
        z:=f()+x;
        x:=x-1;
        if (z>1) then
            x:=1
        else
            y:=1;
        k:=lol+w+z;
    end;

function kkk(lol,j:integer;a:boolean; var k,w:integer):integer;  
    var z:integer;
    function f:integer;
    begin
    end;
    begin
        f();
        z:=f()+x;
        x:=x-1;
        if (z>1) then
            x:=1
        else
            y:=1;
        k:=lol+w+z
    end;
begin
    x:= p(p(a,b,m,kkk(a,b,n,c,x),a),a,m,b,c) + p(a,b,m,c,y) / a;
    read(b, c, a);
    for a:=1 to b+10 do
        for b:=1 to 10 do
            c:= 1;
    for c:=1 to 10 do
    begin
        a:= 1;
        b:= 1;
    end;
    if (a>b) then
        if (a<b) then
            c:=c+1;
    if (a>b) then
        if (a<b) then
            c:=c+1
        else
            c:=c-1;
    if (a>b) then
        if (a<b) then
            c:=c+1
        else
            c:=c-1
    else
        c:=0;
end.

