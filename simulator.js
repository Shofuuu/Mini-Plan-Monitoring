var express = require('express');
const { title } = require('process');
var app = express();

let posmapx = [
    346,
    484,
    793,
    647,
    433,
    346,
];

let posmapy = [
    294,
    81,
    200,
    443,
    421,
    294,
];

let posangle= [
    //315,
    -45,
    0,
    45
];

function getRandomInt(min, max) {
    min = Math.ceil(min);
    max = Math.floor(max);
    return Math.floor(Math.random() * (max - min)) + min; //The maximum is exclusive and the minimum is inclusive
}

app.get('/status', function(req, res){
    var getrand = getRandomInt(0,6);

    res.json(
        {
            current : getRandomInt(0,13),
            checkpoint : getRandomInt(0,6),
            battery : getRandomInt(0,25),
            //posx: posmapx[getrand],
            //posy: posmapy[getrand],
            posx: posmapx[getrand],
            posy: posmapy[getrand],
            head: posangle[getRandomInt(0,3)]
        }
    );
});

app.listen(8080, function () {
    console.log('Listening on port 8080');
});