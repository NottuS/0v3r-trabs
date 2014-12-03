var mongoose = require("mongoose");

// mongoose.connect("mongodb://"+(process.env.IP || "localhost")+"/MEAN");
mongoose.connect("mongodb://localhost/MEAN");

var db = mongoose.connection;

db.on("error", function(err) {
	console.log("Erro durante conexão, sudo service mongod start", err);
});

db.once("open", function() {
	console.log("Conexão realizada com o banco");
});

/*
show dbs
use MEAN
show collections
db.users.find()
*/

exports.mongoose = mongoose;