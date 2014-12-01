module.exports = function(io) {
	var lastOpSolution;
	var sockets = io.sockets;
	var lClient;
	var clients = {};
	var sessions = [];
	var newOp = defineOp();
	var scores = {}
	var hall_of_fame = [];

	sockets.on('connection', function (client) {
		lClient = client;
		client.on('send-server-result', function (item) {
				var msg;
				if (lastOpSolution == item.value) {
					msg = "Acertou";
					newOp = defineOp();
					client.emit('send-client-result', 1);
					client.broadcast.emit('send-client-result', 2);
					client.emit('send-client', newOp);
					client.broadcast.emit('send-client', newOp);
					scores[item.name] = (scores[item.name] || 0) + 1;
					broadcast ('scores', format_scores(scores)); //broacast scores
				} else {
					msg = "Errou";
					client.emit('send-client-result', 0);
				}	
				client.broadcast.emit('send-client', newOp);
				//newOp = defineOp();
				// msg += "<b>"+item.name+":</b> "+item.value+"<br>";
				// client.emit('send-client', "Server->client:"+msg+" Op:"+newOp);
				// client.broadcast.emit('send-client', "Server->all:"+msg+" Op:"+newOp);
		});

		client.on('join', function (data) {
			client.emit ('new_operation', newOp); //send challenge to new client
			client.emit ('scores', format_scores(scores)); //send scores to new client
			client.emit ('hall_of_fame', hall_of_fame); //send hall of fame
			var newClient = true;
			for (var i=0; i < sessions.length; i++) {
				if (sessions[i] === client.id) {
					newClient = false;
				}
			}
			if (newClient) {
				console.log("newClient: "+client.id);
				sessions.push(client.id);
				clients[client.id] = client;
				// client.emit('send-client', newOp);
				// client.emit ('scores', format_scores(scores)); //send scores to new client
				// client.emit ('history', history); //send history
				broadcast ('hall_of_fame', hall_of_fame); //send hall of fame
		
			}
		});
	});

	var game_duration = 60 * 1000;
	var game_started = new Date();

	setInterval(function broadcastTime() {
		var elapsed = new Date().getTime() - game_started.getTime();
		var remaining = Math.floor((game_duration - elapsed) / 1000);
		if (remaining<0){
			var timestamp = game_started.getDate() + '/' + (game_started.getMonth() + 1) + '/' + game_started.getFullYear() + ' ' +  game_started.getHours() + ":" + (game_started.getMinutes() > 9 ? game_started.getMinutes() : '0' + game_started.getMinutes());
			if (format_scores(scores).length)
				hall_of_fame = UpdateHallOfFame(format_scores(scores), timestamp);
			scores = {};
			game_started = new Date(); //start game again!
			newOp = defineOp();
			broadcast("new-game",newOp);
			broadcast ('scores', format_scores(scores)); //broadcast scores
			broadcast ('hall_of_fame', hall_of_fame); //broadcast "hall of fame"
		} else 
			broadcast("time",remaining);
	},1000);

	function broadcast(type, msg) {
		for (var i=0, l=sessions.length; i < l ; i++) {
				clients[sessions[i]].emit(type, msg);
		};
	}

	function UpdateHallOfFame(scores, timestamp){
		var newPlayer = true;
		// if(hall_of_fame.length)
		   	for(var score_key in scores) { // Atualiza para o maior score realizado
				for(var hof_key in hall_of_fame) {
					if (hall_of_fame[hof_key].player == scores[score_key].player) {
						if (hall_of_fame[hof_key].score < scores[score_key].score) {
							hall_of_fame[hof_key].score = scores[score_key].score;
						};
						newPlayer = false;
					};
				}
				if (newPlayer) {
					scores[score_key].timestamp = timestamp;
					hall_of_fame.push(scores[score_key]); //add all scores
				};
				newPlayer = true;
			}

		// for (var i = 0, l = scores.length; i < l ;  i++) {
		// 	var score = scores[i];
		// 	score.timestamp = timestamp;
		// 	hall_of_fame.push(score); //add all scores
		// };

		//sort by score
		hall_of_fame.sort(function(a,b) {
      		return (b["score"] - a["score"]);
		});
		// util.sort(hall_of_fame, 'score', true);

		//and slice array!
		return hall_of_fame.slice(0,20);
	}


	function format_scores (scores){
	   var arr = [];
	   for(var key in scores){
	      arr.push({player: key, score : scores[key]});
	   }
	   return arr;
	}

	function getRandomNumber(min, max) {
    	return Math.floor(Math.random() * (max - min + 1)) + min;	
	}

	function defineOp() {
		var number1 = getRandomNumber(0,20);
		var number2 = getRandomNumber(0,20);
		var operation_type = getRandomNumber(0,1) ? '+' : '-';
		lastOpSolution = eval(number1 + operation_type + number2);
		return number1 + operation_type + number2;
	}
}