package com.example.t_s_v;

import androidx.appcompat.app.AppCompatActivity;

import java.io.BufferedReader;
import java.io.InputStreamReader;

import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;


import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.util.Log;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.os.Vibrator;
import android.os.Bundle;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.RadioGroup;
import android.widget.Toast;

    public class MainActivity extends AppCompatActivity {
        Toast mToast = null;
        int count;
        String str;
        SoundPool mPool;
        int mDdok;
        AudioManager mAm;
        private int LENGTH_TO_SHOW = Toast.LENGTH_SHORT;
        private EditText textToShow;
        private Button showToastButton;


        public void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            setContentView(R.layout.activity_main);

            textToShow = (EditText)findViewById(R.id.TextInput);
            showToastButton = (Button)findViewById(R.id.ShowToast);

            findViewById(R.id.shortmsg).setOnClickListener(mClickListener);
            findViewById(R.id.longmsg).setOnClickListener(mClickListener);
            findViewById(R.id.count1).setOnClickListener(mClickListener);
            findViewById(R.id.count2).setOnClickListener(mClickListener);

            mPool = new SoundPool(1, AudioManager.STREAM_MUSIC, 0);
            mDdok = mPool.load(this, R.raw.ddok, 1);
            mAm = (AudioManager)getSystemService(AUDIO_SERVICE);

            findViewById(R.id.shortmsg).setOnClickListener(mClickListener);
        }

        Button.OnClickListener mClickListener = new Button.OnClickListener() {
            public void onClick(View v) {
                MediaPlayer player;
                switch (v.getId()) {
                    case R.id.shortmsg:
                        Toast.makeText(MainActivity.this, "잠시 나타나는 메시지",
                                Toast.LENGTH_SHORT).show();
                        mPool.play(mDdok, 1, 0, -1, 0,1);
                        Vibrator vibe = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
                        vibe.vibrate(500);
                        break;
                    case R.id.longmsg:
                        Toast.makeText(MainActivity.this, "조금 길게 나타나는 메시지",
                                Toast.LENGTH_LONG).show();
                        Vibrator vibe1 = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
                        vibe1.vibrate(200);
                        break;
                    case R.id.count1:
                        str = "현재 카운트 = " + count++;
                        if (mToast != null) {
                            mToast.cancel();
                        }
                        mToast = Toast.makeText(MainActivity.this, str, Toast.LENGTH_SHORT);
                        mToast.show();
                        break;
                    case R.id.count2:
                        str = "현재 카운트 = " + count++;
                        if (mToast == null) {
                            mToast = Toast.makeText(MainActivity.this, str, Toast.LENGTH_SHORT);
                        } else {
                            mToast.setText(str);
                        }
                        mToast.show();
                        break;
                    case R.id.ShowToast:
                        Toast.makeText(MainActivity.this, textToShow.getText().toString(),
                                Toast.LENGTH_LONG).show();
                }
            }
        };
    }