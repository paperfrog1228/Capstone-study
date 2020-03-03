const rl = require('readline').createInterface({
    input: process.stdin,
    output: process.stdout
});
const clc = require('cli-color');

const socket = require('../socket');

const userInfo = {
    name: "",
    hp: 0,
}

rl.on('line', line => {
    if (line === '3')
        rl.close();
    else if (line === '2') {
        socket.writeData(`RFR`);
        printInterface();
    }
    else if (line === '1') {
        socket.writeData('ATK');
        printInterface();
    }
    else {
        socket.writeData(line);
        printInterface();
    }
}).on('close', () => {
    console.log('프로그램을 종료합니다.');
    process.exit(0);
});

const init = function (connConfig) {
    console.clear();
    
    rl.question('당신의 이름은? : ', answer => {
        userInfo.name = answer;
        userInfo.hp = 100;
        console.log(`반갑습니다. ${userInfo.name}`);
        
        console.log(`서버에 연결을 시도합니다.`);
        socket.getConnection(userInfo.name, connConfig.port, connConfig.host, function() {
            socket.writeData(`LOGIN|${userInfo.name}`);
            printInterface();
        });
    });
}

function printInterface() {
    let curHp;

    rl.pause();
    setTimeout(function() {
        console.clear();
        console.log(clc.cyan(`SSG`) + ` - ` + clc.red(`Super Simple Game`));

        checkMsg();
        
        if (userInfo.hp > 75)
            curHp = clc.green(userInfo.hp);
        else if (userInfo.hp > 50)
            curHp = clc.yellow(userInfo.hp);
        else
            curHp = clc.red(userInfo.hp);

        console.log(`${userInfo.name}의 행동은? (hp ${curHp})`);
        if (userInfo.hp > 0) {
            console.log(`(1 = ${clc.red(`공격`)} | 2 = ${clc.cyan(`새로고침`)} | 3 = 종료)`);
            rl.resume();
        }
        else {
            console.log(`${clc.red(`사망했습니다... (hp ${userInfo.hp})`)}`);
            rl.close();
        }
    }, 1000);
}

function checkMsg() {
    console.log(`=================`);
    if (socket.msgQueue.length == 0)
        console.log(`메세지가 없습니다.`);
    

    let msgs = [];
    while (socket.msgQueue.length != 0) {
        msgs = socket.msgQueue.shift().split('|');
        console.log(`MSG: ${msgs[1]}`);

        if(msgs[0] === "ATK") {
            userInfo.hp -= parseInt(msgs[2]);
        }
    }
    console.log(`=================`);
}

module.exports = {
    init
}