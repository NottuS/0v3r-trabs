var connection = require("./");

var mongoose = connection.mongoose
	, Schema = mongoose.Schema;

var UserSchema = new Schema({
	name: {type: String, default: ''},
	email: {type: String, default: '', unique: true},
	password: {type: String, default: ''},
});

var User = mongoose.model("User", UserSchema);

exports.list = function(req, res) {
	User
		.find()
		.exec(function(err, users) {
			if(err) {
				console.log(err);
			} else {
				res.json(users);
			}
		});
};

exports.get = function(req, res) {
	var id = req.params.id; // Recebe o parametro passado pelo node

	User
		.findOne({_id: id})
		.exec(function(err, users) {
			if(err) {
				console.log(err);
				res.json(err);
			} else {
				res.json(users);
			}
		});
};

exports.create = function(req, res) {
	var data = req.body;

	var dados = {
		name: data.name,
		email: data.email,
		password: data.password
	};

	var user = new User(dados);
	user.save(function(err, data) {
		if(err) {
			res.json(err);
		} else {
			res.json(data);
		}
	});
};

exports.update = function(req, res) {
	var id = req.params.id;
	var data = req.body;

	User.update({_id: id}, data,function(err, data) {
		if(err) {
			res.json(err);
		} else {
			res.json(data);
		}
	});
};

exports.delete = function(req, res) {
	var id = req.params.id;

	User.remove({_id: id}, function(err, data) {
		if(err) {
			res.json(err);
		} else {
			res.json(data);
		}
	});
}
