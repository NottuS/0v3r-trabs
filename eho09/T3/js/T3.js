function createObj(){
    canvas = document.getElementById('drawCanvas');
    input = document.getElementById('nPoly');
    add = document.getElementById('add');
    clear = document.getElementById('clear');
    input.defaultValue="3";
    ctx = canvas.getContext("2d");
    with(canvas){
        style.border = "1px solid black";
        width = 700;
        height = 500;

    };
    clickArea = 7;
    move = function(event){};

    points = new Array();
    points.push({x: 10, y: 10});
    points.push({x: 100, y: 100});
    objects = new Array();
    objects.push(points);

    base = points[0];
    end = points[1];

    oldX = 0;
    oldY = 0;

    draw = function(){
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        for (var k = 0; k < objects.length; k++) {
            var points = objects[k];
            for (var i = 0; i < points.length - 1; i++) {
                ctx.beginPath();
                ctx.arc(points[i].x, points[i].y, 5, 0, 2 * Math.PI, false);
                ctx.fill();

                ctx.beginPath();
                ctx.arc(points[i+1].x, points[i+1].y, 5, 0, 2 * Math.PI, false);
                ctx.fill();

                ctx.beginPath();
                ctx.moveTo(points[i].x, points[i].y);
                ctx.lineTo(points[i+1].x, points[i+1].y);
                ctx.lineWidth = 5;
                ctx.stroke();
            }
        };
    }

    add.onclick = function(event){
        var vertices = parseInt(input.value);
        
        if ( vertices < 3 || vertices > 8) {
            alert("Min: 3\nMax: 8");
            return;
        };

        var points = new Array();
        var angle = 0.0;
        var step = 2 * Math.PI / vertices;
        var radius = vertices * 10;

        for ( var i = 0; i < vertices; i++ ) {
            var x = radius * Math.cos(angle) + canvas.width/2;
            var y = radius * Math.sin(angle) + canvas.height/2;
            angle += step;
            points.push({x:x, y:y});
        }
        points.push(points[0]);
        objects.push(points);
        
        draw();
    }

    clear.onclick = function(event) {
        objects = new Array();
        draw();
    }


    canvas.onmousemove = function(event){
        document.getElementById('canvasXY').innerHTML = "X: " + (event.pageX - canvas.offsetLeft) + " " +
                                                       "Y: " + (event.pageY - canvas.offsetTop);
        move(event);
        draw();  
    }
    
    canvas.onmouseup = function(event) {
        move = function(event){};
    }

    canvas.onmousedown = function(event) {
        physics(event);
    }

    updateBase = function(event) {
        base.x = event.pageX - canvas.offsetLeft;
        base.y = event.pageY - canvas.offsetTop;
    }

    updateEnd = function(event) {
        end.x = event.pageX - canvas.offsetLeft;
        end.y = event.pageY - canvas.offsetTop;
    }

    updateAll = function(event) {
        var diffX = event.pageX - canvas.offsetLeft - oldX;
        oldX = event.pageX - canvas.offsetLeft;
        var diffY = event.pageY - canvas.offsetTop - oldY;
        oldY = event.pageY - canvas.offsetTop;
        
        end.x += diffX;
        end.y += diffY;
        base.x += diffX;
        base.y += diffY;
    }

    physics = function(event) {
        var x = event.pageX - canvas.offsetLeft;
        var y = event.pageY - canvas.offsetTop;
        for (var k = 0; k < objects.length; k++) {
            var points = objects[k];
            for (var i = 0; i < points.length - 1; i++) {
                fx = (points[i].y - points[i+1].y) * x + (points[i+1].x - points[i].x) * y + (points[i].x*points[i+1].y - points[i+1].x*points[i].y);
                if (x  < points[i].x + clickArea &&  x > points[i].x - clickArea && 
                    y < points[i].y + clickArea && y > points[i].y - clickArea ) {
                    base = points[i];
                    move = updateBase;
                    return;
                } else if (x < points[i+1].x + clickArea &&  x > points[i+1].x - clickArea &&
                            y < points[i+1].y + clickArea && y > points[i+1].y - clickArea ) {
                    end = points[i+1];
                    move = updateEnd;
                    return;
                } else if (fx > -2250 && fx < 2250  && 
                                x >= Math.min(points[i].x, points[i+1].x) - clickArea && 
                                x <= Math.max(points[i].x, points[i+1].x) + clickArea && 
                                y >= Math.min(points[i].y, points[i+1].y) - clickArea && 
                                y <= Math.max(points[i].y, points[i+1].y) + clickArea) {
                    if( event.button == 2 ){
                        points.splice(i+1,0,{x: x, y: y});
                        draw();
                    } else {
                        oldX = x;
                        oldY = y;
                        base = points[i];
                        end = points[i+1];
                        move = updateAll;
                    }
                    return;
                }
            }
        }
    }

    draw();
    //setInterval(draw, 0);

    return this;
}