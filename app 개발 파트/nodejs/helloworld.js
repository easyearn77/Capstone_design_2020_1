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
// 만약 그 거리가 100m 이내라면 믿의 코드를 활성화시켜 알림 전송


  /*   function getDistanceFromLatLonInKm(lat1,lng1,lat2,lng2) {
    function deg2rad(deg) {
        return deg * (Math.PI/180)
    }

    var R = 6371; // Radius of the earth in km
    var dLat = deg2rad(lat2-lat1);  // deg2rad below
    var dLon = deg2rad(lng2-lng1);
    var a = Math.sin(dLat/2) * Math.sin(dLat/2) + Math.cos(deg2rad(lat1)) * Math.cos(deg2rad(lat2)) * Math.sin(dLon/2) * Math.sin(dLon/2);
    var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));
    var d = R * c; // Distance in km
    return d;
} */


// 이건 km 단위니까 0.01 * d 가 100 이내면 전송하게끔 if문 식으로 만들어야  함.
// 그래서 socket.emit 식으로 보내면 될듯.

     // socket.emit('message_from_server', '위험 알림을 활성화합니다');
     // socket.emit('message_from_server', '"' +msg+ '" gps 수신완료');
     // 이 경우, emit 리스너를 하나 더 만드는게 나을듯. message_from_server 말고 하나 더 만들어서 알림만 띄우도록.
   });





});
