var io = require('socket.io').listen(999);
var matchMakingPool = new Array();
matchMakingPool.push('none');
var user2;
var user = {
  socketId:'',
  userId:'',
  elo:''
}
//새로운 유저 연결되었을 시
io.sockets.on("connection", function(socket){
  //클라이언트에게 매치메이킹 서버에 연결되었음을 알립니다.
  io.to(socket.id).emit('NoticeConnected');
  //유저의 정보를 받아옵니다.
  socket.on('UpdateUserInfo',function (data){
    user2={socketId: socket.id, userId: data.userId , elo: data.elo }
    console.log('socketId:',socket.id,'  Id:',user.userId);
  });

  //기다려야 하는가?
  var isWait=true;
  for(var i=0;i<matchMakingPool.length;i++){
    //기다리고 있는 상대가 있을 경우
    if(matchMakingPool[i]!='none'){
      var temp=matchMakingPool[i];
      console.log('상대는 이녀석',temp);
      isWait=false;
      io.to(matchMakingPool[i].socketId).emit('FoundUser',user);
      io.to(socket.id).emit('FoundUser',matchMakingPool[i]);
      matchMakingPool[i]='none'
    }
  }
  if(isWait) {
    console.log('상대가 없어..',user2)
    matchMakingPool.push(user2);
  }

  socket.on('disconnect',function(){
    console.log('user disconnected:',socket.id);
    //유저 이탈 시 매치메이킹 풀에서 제거해줌.
    if(matchMakingPool[i].socketId==user.socketId){
      matchMakingPool[i]='none';
    }
  });
});

