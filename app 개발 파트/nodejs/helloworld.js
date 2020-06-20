var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);

// 이게 서버에서온 위도 경도 정보라고 가정합니다.
// 아직 아두이노, 앱과 서버간의 연결이 완벽하지 않습니다. 앱은 서버에 아두이노에서 준 gps값이 있어야 작동을 하기에 우선 이렇게 가정했습니다.
var user = {

  latitude: '37.328707',

  longitude: '127.116459' //바꿨습니다

// 현재 안드로이드 기기(제가 설정한 노트북 에뮬레이터의 초기 gps설정값)와 44m 차이나는 위도 경도의 좌표입니다.
};



var userString = JSON.stringify(user); // 해당 gps정보를 json 형식으로 변환합니다.




app.get('/', function(req, res){
   res.sendfile('index.html');// 이 부분은 제 로컬호스트와 연결할때 웹페이지를 살짝 꾸미기 위한 용도로 있는 html을 불러오는것입니다. 신경 안쓰셔도됩니다.
});

http.listen(8080, function(){
   console.log('listening on *:8080');
}); // localhost8080으로 url을 입력하면 index.html을 인식하여 웹 페이지가 켜지게 됩니다.


io.sockets.on('connection', function (socket){
  // socket.emit('message_from_server', '현재 gps 값을 요청합니다.');

  socket.emit('message_from_server', userString);
  // 서버는 앱이 연결되어있고 아두이노에서 받은 gps가 있으면, 바로 그것을 emit으로 앱에게 전달합니다.

  //메세지가 들어 오면 응답. 이 부분은 사실 상관없지만 그냥 제 로컬 호스트 서버가 잘 연결되어있는지 확인해 보기위해 만들었습니다.
  //앱에서 메세지를 보내면 그냥 그대로 cmd창에 띄워주는 용도입니다.
   socket.on('message_from_client', function (msg){
     console.log('message:', msg);

     //socket.emit('message_from_server', userString);
     // socket.emit('message_from_server', '"' +msg+ '" gps 수신완료');
   });



});
