var io = require('socket.io').listen(999);
var matchMakingPool = new Array();

//새로운 유저 연결되었을 시
io.sockets.on("connection", function(socket){

  //클라이언트에게 매치메이킹 서버에 연결되었음을 알립니다.
  io.to(socket.id).emit('NoticeConnected');

  //유저의 정보를 받아옵니다.
  socket.on('UpdateUserInfo',function (data){
    console.log('updateuserinfo');
    user={socketId: socket.id, userId: data.userId , elo: data.elo }
    console.log('socketId:',socket.id,'  Id:',user.userId);
  });

  socket.on('disconnect',function(){
    console.log('user disconnected:',socket.id);
  });
});

var user = {
  socketId:'',
  userId:'',
  elo:''
}