package com.example.samplepushproject;

        import android.app.Notification;
        import android.app.NotificationManager;
        import android.content.Context;
        import android.media.RingtoneManager;
        import android.util.Log;

        import androidx.annotation.NonNull;

        import com.google.firebase.messaging.FirebaseMessagingService;
        import com.google.firebase.messaging.RemoteMessage;

public class MyFirebaseMessagingService extends FirebaseMessagingService {

    protected static final String FCMTAG = "[FCM Service]";

    @Override
    public void onMessageReceived(@NonNull RemoteMessage remoteMessage) {
        super.onMessageReceived(remoteMessage);

        Log.d(FCMTAG, "온 메세지 리시브드 이즈 콜드");

        String msg, title;

        msg = remoteMessage.getNotification().getBody();
        title = remoteMessage.getNotification().getTitle();

        Notification.Builder noti = new Notification.Builder(this)
                .setContentTitle("New push from : " + title)
                .setContentText(msg)
                .setSound(RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION)) // 소리는 잘들림 ㅇㅇ
                .setVibrate(new long[]{1, 1000}) // 왜 진동은 안되누. 애뮬이라그런가.
                .setSmallIcon(R.mipmap.ic_launcher);




        NotificationManager notificationManager = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);

        notificationManager.notify(0, noti.build());

    }
}
