// eventListener 정의 분리
// 있어보이지만 net 모듈 객체 사용하는데 스니펫 적용안돼서 상당히 불편함.

const clc = require('cli-color');

module.exports = function(client) {
    client.on('data', function(data) {
        require('./index').msgQueue.push(MyProtocol(data));
    });
    client.on('end', function() {
        console.log(`${client.connName} 커넥션 종료`);
    });
    client.on('error', function(error) {
        console.log(`에러 발생 : ${JSON.stringify(error)}`);
    });
    client.on('timeout', function() {
        console.log(`소켓 타임아웃`);
    });
    client.on('close', function() {
        console.log(`소켓 닫힘`);
    });
}

function MyProtocol(msg) {
    const msgs = msg.split('|');

    if (msgs[0] === "MSG")
        return msg;
    else if (msgs[0] === "ATK") {
        const dmg = parseInt(msgs[1]);
        if (dmg <= 13)
            msgs[1] = `누군가가 당신을 때렸어요. (-${clc.yellow(dmg)})|${dmg}`;
        else if (dmg <= 17)
            msgs[1] = `불의의 일격에 당했습니다. (-${clc.yellow(dmg)})|${dmg}`;
        else if (dmg >= 18)
            msgs[1] = `으악! 너무 아프다! (-${clc.red(dmg)})|${dmg}`;
        
        return msgs[0] + '|' + msgs[1];
    }
}