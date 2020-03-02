const rl = require('readline').createInterface({
    input: process.stdin,
    output: process.stdout
});

const socket = require('./socket');

const connConfig = {
    port: '8888',
    host: '127.0.0.1'
}

socket.getConnection('Anteater', connConfig.port, connConfig.host);

rl.on('line', function(line) {
    socket.writeData(line);
}).on('close', function() {
    process.exit();
});