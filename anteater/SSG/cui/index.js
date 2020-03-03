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
    if (line === 'exit')
        rl.close();
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
            printInterface();
        });
    });
}

function printInterface() {
    let curHp;
    if (userInfo.hp > 75)
        curHp = clc.green(userInfo.hp);
    else if (userInfo.hp > 50)
        curHp = clc.yellow(userInfo.hp);
    else
        curHp = clc.red(userInfo.hp);

    rl.pause();
    setTimeout(function() {
        console.clear();
        console.log(clc.cyan(`SSG`) + ` - ` + clc.red(`Super Simple Game`));
        checkMsg();
        console.log(`${userInfo.name}의 행동은? (hp ${curHp})`);
        console.log(`(1 = ${clc.red(`공격`)} | 2 = ${clc.cyan(`새로고침`)})`);
        rl.resume();
    }, 1000);
}

function checkMsg() {
    console.log(`=================`);
    if (socket.msgQueue.length == 0)
        console.log(`메세지가 없습니다.`)
    
    let msg = "";
    while (socket.msgQueue.length != 0) {
        console.log(`MSG: ${socket.msgQueue.shift()}`)
    }
    console.log(`=================`);
}

module.exports = {
    init
}