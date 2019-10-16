var express = require('express');
var app = express();
var bodyParser = require('body-parser');
  
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({
    extended: true
}));
  
  
// default route
app.get('/', function (req, res) {
    return res.send({ error: true, message: 'hello' })
});
// connection configurations
const Pool = require('pg').Pool
const pool = new Pool({
  user: 'postgres',
  host: 'localhost',
  database: 'hartslagmeter',
  password: '', // fill in password of pgadmin
  port: 5432,
})


// Retrieve average bpm 
app.get('/bpms', function (req, res) {
    pool.query('SELECT AVG(bpm) FROM userbpm', (error, results) => {
        if (error) {
          throw error
        }
        const bpmaverage = parseInt(results.rows[0].avg, 10);
        console.log(bpmaverage);
        res.status(200).send(bpmaverage.toString(10));
      })
    
});
  
// Add a new user  
app.post('/bpm', function (req, res) {
  
    const {bpm} = req.body
    console.log(req.body);
    

  pool.query('INSERT INTO userbpm (bpm) VALUES ($1)', [bpm], (error, results) => {
    if (error) {
      throw error
    }
    res.status(201).send(`OK`)
  })
});
 
 
//  Update user with id
app.put('/user', function (req, res) {
  
    let user_id = req.body.user_id;
    let user = req.body.user;
  
    if (!user_id || !user) {
        return res.status(400).send({ error: user, message: 'Please provide user and user_id' });
    }
  
    dbConn.query("UPDATE users SET user = ? WHERE id = ?", [user, user_id], function (error, results, fields) {
        if (error) throw error;
        return res.send({ error: false, data: results, message: 'user has been updated successfully.' });
    });
});
 
// set port
app.listen(9000, function () {
    console.log('Node app is running on port 3000');
});
 
module.exports = app;