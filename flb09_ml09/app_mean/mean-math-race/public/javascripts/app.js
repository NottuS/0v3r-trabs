angular.module('sonApp', ['ngRoute','sonApp.controllers']) // Nome, Dependencias
	.config(
		['$routeProvider','$locationProvider',
			function($routeProvider, $locationProvider) {
				$routeProvider
					.when("/", {
						//templateUrl: '/angular/users/index'
						templateUrl: '/angular/challenge/index'
					})
					.when("/edit/:id", {
						templateUrl: '/angular/users/edit'
					})
					.when("/new", {
						templateUrl: '/angular/users/new'
					});
				$locationProvider.html5Mode(true); // Remove # da url
			}
		]
	);