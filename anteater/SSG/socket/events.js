// eventListener 정의 분리
// 있어보이지만 net 모듈 객체 사용하는데 스니펫 적용안돼서 상당히 불편함.

module.exports = function(client) {
    client.on('data', function(data) {
        console.log(`${client.connName} 에게서 데이터가 옴 : ${data.toString()}`);
    });
    client.on('end', function() {
        console.log(`${client.connName} 커넥션 종료`);
    });
    client.on('error', function(err) {
        console.log(`에러 발생 : ${JSON.stringify(err)}`);
    });
    client.on('timeout', function() {
        console.log(`소켓 타임아웃`);
    });
    client.on('close', function() {
        console.log(`소켓 닫힘`);
    });
}