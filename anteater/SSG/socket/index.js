// Ref1. https://ryeom2.tistory.com/122
// Ref2. https://nodejs.org/api/net.html
// node.js tcp socket client
// 전체적으로 socketIO랑 비슷함. 정확히는 socketIO가 net 모듈이랑 비슷하다고 해야할듯.

const net = require('net');

const setEventListener = require('./events.js');

let client = {};
client.msgQueue = [];

client.getConnection = function(connName, port, host, callback = () => {}) {
    client = net.connect({port: port, host: host}, function() {
        client.connName = connName;
        console.log(`${client.connName} 연결 됨.`);
        console.log(`로컬 : ${client.localAddress}:${client.localPort}`);
        console.log(`리모트 : ${client.remoteAddress}:${client.remotePort}`);

        //client.setTimeout(500);
        client.setEncoding('utf8');

        setEventListener(client);
        
        callback();
    });
};

client.writeData = function(data) {
    const success = client.write(data + '\0');
    /*
    socket.write(data[, encoding][, callback])
    기본적인 tcp write 함수인데...
    전체 데이터가 버퍼로 빠져나가면 true를 리턴,
    아직 데이터가 일부라도 유저 메모리 큐에 남아있으면 false를 리턴.
    버퍼가 다시 사용가능한 상태가 되면 'drain' 이벤트가 발생한다.
    */
   if (!success) {
       (function(socket, data) {
            socket.once('drain', function() {
                socket.writeData(data);
            });
       })(client, data); // 따라서 이런 복잡한 과정을 추가로 실시.
       /*
       if문 안에서 'drain' 이벤트 시 writeData를 재귀호출하는 함수를 정의한 다음,
       그 함수에 client와 data를 줘서 바로 호출.
       */
   }
};

module.exports = client;