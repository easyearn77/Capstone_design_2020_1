var express = require('express');
var http = require('http');
var app = express();

var server = http.createServer(app);
server.listen(8080, () => {
  console.log("8080번 포트에서 서버 대기중입니다.");
});

var io = require('socket.io')(server);

/*
express 모듈을 이용하여  http 메소드 get방식으로 아두이노에서 데이터가 들어오면 쿼리를 분석하여
위도와 경도만을 추출하여 리스트를 만들고 그 값을 다시 안드로이드에 emit 하여
위도 경도를 계산할 수 있게 할려고 한다.
*/
app.get('/', function(req, res){
  res.send(req.query.longitude+','+req.query.latitude);
  longitudeData = req.query.longitude;
  latitudeData = req.query.latitude;
});
/*
여기서 longitudeData 와 latitudeData에 데이터가 들어간다고 생각했는데
서버를 실행하면 정의 되지 않았다는 메시지가 도출하고 url 모듈을 이용하여 parse() 하는
방식과 똑같은 작업이라고 생각했는데 그렇지 않은 것 같아 해결이 필요합니다.
*/

var user = [longitudeData, latitudeData];

var userstring = JSON.stringify(user);

io.on('connect', function(socket){
  console.log( '클라이언트 접속 ');
  console.log( longitudeData, latitudeData);
  socket.on('disconnection', function(){
    console.log('클라이언트 접속 종료');
  });
    socket.emit('messsage_from_server', userstring);

    socket.on('messsage_from_server', function(data){  // 추가적으로 안드로이드와 이벤트를 맞춰봐야하 한다. 

    });
});
