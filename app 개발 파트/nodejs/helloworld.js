var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);

app.get('/', function(req, res){
   res.sendfile('index.html');//default page
});

http.listen(8080, function(){
   console.log('listening on *:8080');
});


io.sockets.on('connection', function (socket){

   //원격에서 접속이 되면 기본 응답
   // 아두이노 부분 예상. if 아두이노 센서로부터 신호가 들어왔을 때
   socket.emit('message_from_server', '현재 gps 값을 요청합니다.');

  //메세지가 들어 오면 응답
   socket.on('message_from_client', function (msg){
     console.log('message:', msg);
     /*받은 메세지를 되돌려 주자.
     아니면 받은 데이터를 이용 라즈베리파에서 뭐든 할 수 있다.
     */


     // 이 부분에서 앱에서 받은 gps와 아두이노 에서 얻은 gps의 거리를 계산
     // 만약 그 거리가 100m 이내라면 믿의 코드를 활성화시켜 알림 전
     // socket.emit('message_from_server', '위험 알림을 활성화합니다');
     // socket.emit('message_from_server', '"' +msg+ '" gps 수신완료');
   });





});
