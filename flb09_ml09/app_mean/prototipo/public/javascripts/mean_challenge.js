function ChallengeController($scope, $http) {
	 $scope.$watch('login.$valid',function(newValue,oldvalue) {
        if(newValue) {
            alert('Model is valid: '+$scope.login.length);
            //Can do a ajax model submit.
        }
        
    });

	$scope.validUser = function() {
		//alert($scope.user.name + $scope.user.name.length);
		 if($scope.user.name.length > 2) {
		 	$scope.valid = true;
		 } else {
		 	$scope.valid = false;
		 }
	}
	
}