module.exports = function(io) {
	var sockets = io.sockets;
	sockets.on('connection', function (client) {
		client.on('send-server', function (item) {
			var msg = "<b>"+item.name+":</b> "+item.value+"<br>";
			client.emit('send-client', "Server->client:"+msg+" Op:"+defineOp());
			client.broadcast.emit('send-client', "Server->all:"+msg);
		});
	});

	function getRandomNumber(min, max) {
    	return Math.floor(Math.random() * (max - min + 1)) + min;	
	}

	function defineOp() {
		var number1 = getRandomNumber(0,20);
		var number2 = getRandomNumber(0,20);
		var operation_type = getRandomNumber(0,1) ? '+' : '-';
		return number1 + operation_type + number2;
	}
}