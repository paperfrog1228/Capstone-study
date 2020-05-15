const cui = require('./cui');

const connConfig = {
    port: '8888',
    host: '127.0.0.1'
}

cui.init(connConfig);

process.on('uncaughtException', function(err) {
    console.log(`예상치 못한 오류가 발생하였습니다. ${err.stack}`);
    process.exit(1);
});