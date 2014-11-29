/* GET users listing. */
var UserModel = require("../models/user");

exports.list = function(req, res){
  //res.send('respond with a resource');
  UserModel.list(req, res);
};

exports.get = function(req, res) {
	UserModel.get(req, res);
};

exports.create = function(req, res) {
	UserModel.create(req, res);
};

exports.update = function(req, res) {
	UserModel.update(req, res);
};

exports.delete = function(req, res) {
	UserModel.delete(req, res);
};
