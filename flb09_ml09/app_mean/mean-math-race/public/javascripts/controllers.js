angular.module('sonApp.controllers', ['sonApp.services'])
	.controller('UsersCtrl',
		['$scope', 'UsersSrv', '$routeParams', '$location',
			function($scope, UsersSrv, $routeParams, $location) {
				$scope.nome = "|Var do angular - controller.js|";
			 	// valid var changed for tests
			 	$scope.valid = true; 
			 	 var socket = io.connect('http://localhost:3000');
			 	//var socket = io.connect('http://0v3r-trabs-filipe1309.c9.io/');
			 	// var socket = io.connect('http://0v3r-trabs-dreamer22001.c9.io/');
				
				socket.on('connect',function() {
					$('#socketio').html('<span class="label label-success">connected!</label>');
					socket.emit('join', "$scope.item.name");
				});

			 	socket.on('send-client-result', function (resultValue) {
					if (resultValue == 1) {
						$('#result').html("Good, you solve the operation!!!").addClass("alert alert-success");
					} else if (resultValue == 2) {
						$('#result').html("Someone solved this quest faster than you!!!!").addClass("alert alert-warning");
					} else {
						$('#result').html("Noooo, wrong result!!!").addClass("alert alert-danger");
					}
				});

			 	socket.on('send-client', function (msg) {
					// var op = document.getElementById('op');
					// op.innerHTML = msg;
					$('#op').html(msg);
					//$scope.op = msg;
				});

			 	socket.on('time', function (msg) {
					$('#mrtime').html(msg);
					// $scope.mTime = "time: "+msg;
				});

				socket.on('new-game', function (msg) {
					$('#op').html(msg);
				});

				socket.on('new_operation',function(operation) {
					$('#op').html(operation+": ");
					$('.in_usr').val('').select();
				});

				socket.on('scores',function(scores) {
					$('#score').html("");
					if (scores.length){ //effect
					  	for(var key in scores){
							$('#score').append("<br>" + scores[key].player + "  <span class='label label-default'>" + scores[key].score + "</span>");
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
						if(item.name === undefined) // change for tests
							item.name = Math.random().toString(36).substring(Math.floor((Math.random()*6)+1));
						socket.emit('send-server-result', item);
					};
				}

				$scope.race = function(item) {
					$('.in_usr').click(function(e){
						this.select();
					});

					$('.in_usr').focus();
					$('.in_usr').keydown(function(e){
						if (e.keyCode=='13'){ //press return
							$('.in_usr').select();
						}
				    });
				}
				//$scope.load();
			}
		]
	);