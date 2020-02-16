var io = require('socket.io').listen(999);
var matchMakingPool = new Array();
console.log(matchMakingPool.length);
//새로운 유저 연결되었을 시
io.sockets.on("connection", function(socket){
  console.log("A user connected : " ,socket.id);
 // objects[socket.id]=new UserObject();
  //클라이언트에게 매치메이킹 서버에 연결되었음을 알립니다.
  io.to(socket.id).emit('matchmaking server connected.');
  if(matchMakingPool.length!=0)
    console.log("대충 다른 유저랑 연결한다는 뜻.");
  else {
    matchMakingPool.push(socket.id);
    for(var i=0;i<matchMakingPool.length;i++)
    {
      console.log('기다리는 플레이어 :',matchMakingPool[i]);
    }
  }

  socket.on('disconnect',function(){
   // delete objects[socket.id];
    console.log('user disconnected:',socket.id);
  });
});
//새로운 유저에 대한 정보.
function UserObject() {

}
