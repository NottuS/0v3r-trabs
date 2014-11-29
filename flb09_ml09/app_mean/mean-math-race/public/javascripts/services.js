// Responsavel por chamar de forma assincrona as requisições da api de crud users
angular.module('sonApp.services', ['ngResource']) // ngResource - para requisiões REST
	.factory('UsersSrv', ['$resource', 
		function($resource) {

			return $resource('/api/users/:id', 
				//paramDefaults
				{
					id: '@id'
				},
				//actions 
				{
					update: {
						method: "PUT",
						url: "/api/users/:id"
					}
				}
			);
		}
	]);