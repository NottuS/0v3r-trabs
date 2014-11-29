angular.module('sonApp.controllers', ['sonApp.services'])
	.controller('UsersCtrl',
		['$scope', 'UsersSrv', '$routeParams', '$location',
			function($scope, UsersSrv, $routeParams, $location) {
				$scope.nome = "|Var do angular - controller.js|";
			 	$scope.valid = false;
			 	var socket = io.connect('http://localhost:3000');

			 	socket.on('send-client', function (msg) {
					var chat = document.getElementById('op');
					chat.innerHTML += msg;
					$scope.item.value = "msg do sv:"+msg;
				});

				$scope.load = function() {
					$scope.registros = UsersSrv.query();
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
					item.value += "Foi"; 
					socket.emit('send-server', item);
				}

				$scope.race = function(item) {
					// item.value = "Iniciou MathRace!"; 
				}

				//$scope.load();
			}
		]
	);