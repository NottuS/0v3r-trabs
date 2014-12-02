var connection = require("./");

var mongoose = connection.mongoose
	, Schema = mongoose.Schema;

var HofSchema = new Schema({
	player: {type: String, default: '', unique: true},
	score: {type: String, default: ''},
	timestamp: {type: String, default: ''},
});

var Hof = mongoose.model("hof", HofSchema);

var hof_data;

exports.list = function(req, res) {
	Hof
		.find()
		.exec(function(err, hofs) {
			if(err) {
				console.log("LIST: ERROR MONGODB: "+err+" , data:"+hofs);
				console.log(err);
			} else {
				console.log("LIST: MONGODB, data:"+hofs);
				hof_data = hofs;
				res.json(hofs);
			}
		});
};

exports.listNode = function() {
	return hof_data;
};

exports.create = function(new_score) {
	var hof_data = {
		player: new_score.player,
		score: new_score.score,
		timestamp: new_score.timestamp
	};

	var new_hof = new Hof(hof_data);
	new_hof.save(function(err, data) {
		if(err) {
			console.log("CREATE: ERROR MONGODB: "+err+" , data:"+hof_data);
		} else {
			console.log("CREATE: INSERTED MONGODB: "+data);
		}
	});
};

exports.update = function(udata) {
	var uplayer = udata.player;
	var data = udata;

	Hof.update({player: uplayer}, data,function(err, data) {
		if(err) {
			console.log("UPDATE: ERROR MONGODB: "+err+" , data:"+hof_data);			
		} else {
			console.log("UPDATE: UPDATED MONGODB: "+err);
		}
	});
};
