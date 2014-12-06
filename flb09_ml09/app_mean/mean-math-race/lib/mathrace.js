/*
License
Copyright (c) 2012 Iván Loire

Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to 
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
of the Software, and to permit persons to whom the Software is furnished to do 
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE.
*/

module.exports = function(io) {
	var lastOpSolution;
	var sockets = io.sockets;
	var HofModel = require("../models/hof");
	var clients = {};
	var sessions = [];
	var newOp = defineOp();
	var scores = {};
	var hallF = HofModel.listNode();
	var hall_of_fame = [];

	sockets.on('connection', function (client) {
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
		});


		client.on('join', function (name) {
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
				sessions.push(client.id);
				clients[client.id] = client;
				client.emit ('hall_of_fame', hall_of_fame); //send hall of fame
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
			hallF = HofModel.listNode();

			if (format_scores(scores).length) {
				NewUpdateHallOfFame(format_scores(scores), timestamp);				
			}
			scores = {};
			game_started = new Date(); //start game again!
			newOp = defineOp();
			broadcast("new-game",newOp);
			broadcast ('scores', format_scores(scores)); //broadcast scores
			broadcast ('hall_of_fame', null); //broadcast "hall of fame"
		} else 
			broadcast("time",remaining);
	},1000);

	function broadcast(type, msg) {
		for (var i=0, l=sessions.length; i < l ; i++) {
				clients[sessions[i]].emit(type, msg);
		};
	}

	function NewUpdateHallOfFame(scores, timestamp){
		var newPlayer = true;
		for(var score_key in scores) { // Atualiza para o maior score realizado
			for (var i=0, l=hallF.length; i < l ; i++) {
		   		if (hallF[i].player == scores[score_key].player) {
		   			if (hallF[i].score < scores[score_key].score) {
		   				hallF[i].score = scores[score_key].score;
		   				// delete hallF[i]._id;
		   				// console.log("hallF["+i+"]._id:"+hallF[i]);
		   				HofModel.update(hallF[i]);
		   			};
					newPlayer = false;
		   		};
			}
			if (newPlayer) {
				scores[score_key].timestamp = timestamp;
				HofModel.create(scores[score_key]);
				hallF[hallF.length] = scores[score_key];
				console.log("hallF[hallF.length]: "+hallF[hallF.length-1].player);
			};
			newPlayer = true;
		}
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