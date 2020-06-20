package com.example.soketioex;

import android.app.FragmentManager;
import android.content.Context;
import android.content.pm.PackageManager;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.media.AudioManager;
import android.media.SoundPool;
import android.os.Build;
import android.os.Bundle;
import android.os.Vibrator;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;


import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.MapFragment;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MarkerOptions;

import org.json.JSONException;
import org.json.JSONObject;

import io.socket.client.IO;
import io.socket.client.Socket;
import io.socket.emitter.Emitter;


public class MainActivity extends AppCompatActivity implements OnMapReadyCallback {

    Socket socket;
    Button button;
    EditText editText;
    TextView textView;
    TextView txtResult;
    TextView txtResult1;
    double longitude;
    double latitude;
    double serverlongitude;
    double serverlatitude;

    SoundPool mPool;
    int mDdok;

    private FragmentManager fragmentManager;
    private MapFragment mapFragment;






    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mPool = new SoundPool(1, AudioManager.STREAM_MUSIC, 0);
        mDdok = mPool.load(this, R.raw.warning, 1);

        fragmentManager = getFragmentManager();
        mapFragment = (MapFragment)fragmentManager.findFragmentById(R.id.googleMap);
        mapFragment.getMapAsync(this);



        txtResult = (TextView)findViewById(R.id.txtResult);
        txtResult1 = (TextView)findViewById(R.id.txtResult1);

        final LocationManager lm = (LocationManager) getSystemService(Context.LOCATION_SERVICE);

        if ( Build.VERSION.SDK_INT >= 23 &&
                ContextCompat.checkSelfPermission( getApplicationContext(), android.Manifest.permission.ACCESS_FINE_LOCATION ) != PackageManager.PERMISSION_GRANTED ) {
            ActivityCompat.requestPermissions( MainActivity.this, new String[] {  android.Manifest.permission.ACCESS_FINE_LOCATION  },
                    0 );
        }
        else{
            Location location = lm.getLastKnownLocation(LocationManager.GPS_PROVIDER);
            String provider = location.getProvider();
            longitude = location.getLongitude(); // 경도
            latitude = location.getLatitude(); // 위도
            double altitude = location.getAltitude();

            String strTemp = Double.toString(longitude);
            String strTemp1 = Double.toString(latitude);

            txtResult.setText(strTemp);

            txtResult1.setText(strTemp1);

            lm.requestLocationUpdates(LocationManager.GPS_PROVIDER, // 1초마다 자신의 gps를 갱신
                    1000,
                    1,
                    gpsLocationListener);
            lm.requestLocationUpdates(LocationManager.NETWORK_PROVIDER,
                    1000,
                    1,
                    gpsLocationListener);




        }


// 개인적으로 이 연결하는 부분이 의심이 갑니다. IO.socket 부분에 로컬호스트 url을 넣으면 작동하지만 서버의 url을 넣으면 서버쪽에서 줘야하는 gps값이 emit으로 넘어오지 않습니다.
// 가끔은 연결조차 안될 때가 있습니다. 서버 쪽에서 socketio메세지가 왔다고도 뜨지 않는 경우가 있습니다. 이것은 uri부분에 서버의 포트를 적냐 안적냐의 차이로 생각됩니다.(잘 모르겠습니다.)
// 물론 서버쪽의 js파일도 서버부분 코드를 보면 아시겠지만 이 밑 부분의 코드와 연동될 수 있게 나름 작성을 했으니 연결이 되면 서버 쪽에서 gps값을 emit으로 넘겨주었어야 합니다.
       try {
            socket = IO.socket("http://192.168.0.46:8080"); // http:// + 현재 와이파이 ip + :8080. 여기에 집 wifi넣으시면 됩니다.
        }catch (Exception e) {
            e.printStackTrace();
        }



        socket.on(Socket.EVENT_CONNECT, new Emitter.Listener() {
            @Override
            public void call(Object... args) {
                //socket.emit("message_from_client", "Hi~ 나는 안드로이드야."); -> 이 줄은 신경 안쓰셔도 됩니다.
            }
        }).on("message_from_server", new Emitter.Listener() { // 여기 emit리스너에서 서버에서 온 emit을 받게됩니다. 서버에서 emit이 왔다면 리스너 안의 코드가 수행되어야 합니다.


            @Override
            public void call(final Object... args) { // 그래서 이 아래부분의 코드가 실행되어, 서버에게 디바이스의 위도 경도를 전송하고 자신은 서버가 보내준 gps와 자신의gps를기반으로 거리를 계산하여
                // 100m이내이면 토스트 메세지를 띄워놓는 형식으로 일단 간단하게 작성해 놓았습니다. 이것이 local호스트에서는 되지만 웹 서버와 연동하면 이 일련의 과정이 작동하지않습니다.
                // 이제 질문내용 워드를 보러가시면 됩니다.
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        textView.setText(args[0].toString()); // 이 args의 용도를 자세히 모르겠지만 서버에서 준 json을 그냥 string형태로 바꿔 그대로 textview로 안드로이드 화면에 띄워줍니다.
                        String msg1 = txtResult.getText().toString(); // 경도
                        String msg2 = txtResult1.getText().toString(); // 위도
                        socket.emit("message_from_client", "디바이스 위도 : " + msg2); // 서버 쪽으로 다바이스 위도 경도를 전송하여 cmd창에 출력해 보도록 하기 위한 emit입니다.
                        socket.emit("message_from_client", "디바이스 경도 : " + msg1);



                            JSONObject jsonObject = new JSONObject();

                            try{
                                jsonObject.put("latitude", txtResult1.getText().toString()); // 위도
                                jsonObject.put("longitude", txtResult.getText().toString()); // 경도
                            }catch (JSONException e){
                                e.printStackTrace();
                            }

                        socket.emit("message_from_client", jsonObject); // 그냥 json형식으로도 위도 경도를 보내보기 위한 코드입니다.

                        String test = textView.getText().toString(); // textView의 스트링을 저장하고
                        JSONObject json = null; // 다시 json형식으로 바꿔서 위도와 경도를 double형식으로 빼냅니다.
                        // 결과적으로 서버에서 받은 json을 textview에 문자열 형식으로 띄워보기 위해 스트링으로 바꿨다가 다시 json으로 바꾸어 필요한 위도 경도 값을 double로 빼 온 것이 되었습니다.
                        try {
                            json = new JSONObject(test);
                            serverlatitude = json.getDouble("latitude");
                            serverlongitude = json.getDouble("longitude");

                            double distance;

                            Location locationA = new Location("point A");
                            locationA.setLatitude(latitude);
                            locationA.setLongitude(longitude);

                            Location locationB = new Location("point B");
                            locationB.setLatitude(serverlatitude);
                            locationB.setLongitude(serverlongitude);

                            distance = locationA.distanceTo(locationB);

                            socket.emit("message_from_client", distance); // 거리를 계산하여 맞게 나오는지 보기 위해 서버창에 띄워보기 위한 코드입니다.

                            if(distance <= 100.00){
                                Toast.makeText(getApplicationContext(), "100m 이내에 보행자가 도로에 있습니다", Toast.LENGTH_LONG).show();
                                mPool.play(mDdok, 1, 1, -1, 0,1);
                                Vibrator vibe = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
                                vibe.vibrate(500);




                            } // 측정한 거리가 100m이내이면 토스트 메세지를 띄웁니다. 이 알림 부분은 소리가 다게 한다든지 하여 좀더 보강될 예정입니다.

                        } catch (JSONException e) {
                            e.printStackTrace();
                        }

                        socket.emit("message_from_client", json); // 서버 창에서 서버가 보낸 위도, 경도가 맞게 왔는지 보기 위한 코드입니다.


// 이 밑 부분의 코드들은 이제 서버 연결과는 관련이 없는 코드들입니다.



                    }
                });
            }





        });

        socket.connect();


        editText = (EditText) findViewById(R.id.editText);
        textView = (TextView) findViewById(R.id.textView);

        button = (Button) findViewById(R.id.button);



        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String msg = editText.getText().toString();
                socket.emit("message_from_client", msg);
            }
        });
    }

    @Override
    public void onMapReady(GoogleMap googleMap) { // 구글맵이 준비되면 호출.
        LatLng location = new LatLng(37.328707, 127.116459);
        MarkerOptions markerOptions = new MarkerOptions();
        markerOptions.title("코소나 팀");
        markerOptions.snippet("오병설"); // 이런거는 신경안쓰셔도 됩니다. 바꾸고싶으면 바꾸셔도됩니다.
        markerOptions.position(location);
        googleMap.addMarker(markerOptions);

        // googleMap.moveCamera(CameraUpdateFactory.newLatLngZoom(location, 16));
        googleMap.animateCamera(CameraUpdateFactory.newLatLngZoom(location, 16));
        // 16배율을 쓰겠다는 말.



    }



    final LocationListener gpsLocationListener = new LocationListener() {
            public void onLocationChanged(Location location) {

            String provider = location.getProvider();
            longitude = location.getLongitude();
            latitude = location.getLatitude();
            double altitude = location.getAltitude();

            String strTemp = Double.toString(longitude);
            String strTemp1 = Double.toString(latitude);

            txtResult.setText(strTemp
            );

            txtResult1.setText(strTemp1
            );

            // gps요청이 올때마다 자신의 달라진 gps값을 송신할수 있음.

        }

        public void onStatusChanged(String provider, int status, Bundle extras) {
        }

        public void onProviderEnabled(String provider) {
        }

        public void onProviderDisabled(String provider) {
        }
    };


}
