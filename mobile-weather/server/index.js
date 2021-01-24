const express = require('express')
const app = express()
const port = 5500
const fetch = require('node-fetch');
const fs = require('fs');
const bodyParser = require('body-parser');

var cors = require('cors');

app.use(cors());
app.use(bodyParser.urlencoded());
app.use(bodyParser.json());

// app.get('/', (req, res) => res.send('Hello World!'))

app.listen(port, () => console.log(`Example app listening on port ${port}!`))

app.get('/', (req, res) => {
    console.log(req.query);
    const temperature = req.query.temperature;
    const humidity = req.query.humidity;
    const d = Date.now();
    console.log(d,',',temperature,',',humidity);
    res.send('ciao');
})

