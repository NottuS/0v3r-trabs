/* GET users listing. */
var HofModel = require("../models/hof");

exports.list = function(req, res){
  //res.send('respond with a resource');
  HofModel.list(req, res);
};

// exports.get = function(req, res) {
// 	UserModel.get(req, res);
// };

exports.create = function(req, res) {
	HofModel.create(req, res);
};

exports.update = function(req, res) {
	HofModel.update(req, res);
};

// exports.delete = function(req, res) {
// 	UserModel.delete(req, res);
// };
