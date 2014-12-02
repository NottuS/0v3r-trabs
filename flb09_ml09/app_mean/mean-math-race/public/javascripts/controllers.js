angular.module('sonApp.controllers', ['sonApp.services'])
	.controller('UsersCtrl',
		['$scope', 'UsersSrv', '$routeParams', '$location',
			function($scope, UsersSrv, $routeParams, $location) {
				$scope.nome = "|Var do angular - controller.js|";
			 	$scope.valid = false;
			 	var socket = io.connect('http://localhost:3000');

				socket.on('connect',function() {
					var connSocketio = document.getElementById('socketio');
					connSocketio.innerHTML = '<span class="label label-success">connected!</label>'; 
					socket.emit('join', "$scope.item.name");
				});
			 	
			 	socket.on('send-client-result', function (resultValue) {
			 		var result = document.getElementById('result');
					if (resultValue == 1) {
						result.innerHTML = "Você acertou!!!";
					} else if (resultValue == 2) {
						result.innerHTML = "Alguem acertou antes de você!!!";
					} else {
						result.innerHTML = "Você errou!!!";
					}

				});

			 	socket.on('send-client', function (msg) {
					var op = document.getElementById('op');
					op.innerHTML = msg;
					//$scope.op = msg;
				});

			 	socket.on('time', function (msg) {
					var mathTime = document.getElementById('mrtime');
					mathTime.innerHTML = msg;
					$scope.mTime = "time: "+msg;
					//$scope.op = msg;
				});

				socket.on('new-game', function (msg) {
					var op = document.getElementById('op');
					op.innerHTML = msg;
				});

				socket.on('new_operation',function(operation) {
					var op = document.getElementById('op');
					op.innerHTML = operation;
				});
				
				socket.on('scores',function(scores) {
					var score = document.getElementById('score');
					score.innerHTML= "";
					if (scores.length){ //effect
					  	for(var key in scores){
							score.innerHTML += "<br>" + scores[key].player + " : " + scores[key].score;
						}
					}
				});

      			socket.on('hall_of_fame',function() {
				// Se houve atualização do hall of fame, consultar o bd novamente
					$scope.load();
				});

				socket.on('disconnect',function() {
					// $('#socketio').html('<span class="label label-important">disconnected</label>'); 
				});

				$scope.load = function() {
					$scope.hof = UsersSrv.query();
				}

				$scope.get = function() {
					$scope.item = UsersSrv.get({id: $routeParams.id});
				}

				$scope.edit = function(item) {
					delete item._id; // Pois o mongo não aceita sobrescrever _id
					$scope.result = UsersSrv.update(
						{id: $routeParams.id},
						item,
						function(data, status, headers, config) {
							$location.path("/");
						}
					);
				}

				$scope.add = function(item) {
					$scope.result = UsersSrv.save(
						{},
						item,
						function(data, status, headers, config) {
							$location.path("/");
						}
					);
				}

				$scope.delete = function(id) {
					if(confirm("Deseja Realmente excluir esse usuário?")) {
						$scope.result = UsersSrv.remove(
							{id: id},
							{},
							function(data, status, headers, config) {
								$location.path("/");
							}
						);	
					}
				}

				$scope.verifyName = function(item) {
					if(item.name.length > 2) {
					 	$scope.valid = true;
					 } else {
					 	$scope.valid = false;
					 }					
				}

				$scope.sendResult = function(item) { 
					if (item.value) {
						socket.emit('send-server-result', item);
					};
				}

				$scope.race = function(item) {
					// item.value = "Iniciou MathRace!"; 
				}

				//$scope.load();
			}
		]
	);