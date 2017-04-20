package com.livelife.playwaretestapp;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

import java.util.ArrayList;

public class RankingActivity extends AppCompatActivity {

    LinearLayout scrollContent;
    DBHelper dbHelper;
    ArrayList<Session> sessions = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ranking);

        scrollContent = (LinearLayout) findViewById(R.id.scrollContent);
        dbHelper = new DBHelper(this);
        sessions = dbHelper.getAllSessions();

        for (Session s: sessions) {
            TextView tv = new TextView(this);
            tv.setText(s.user_name+"        "+s.score);
            tv.setTextSize(22);
            tv.setTextAlignment(TextView.TEXT_ALIGNMENT_CENTER);
            tv.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT));

            scrollContent.addView(tv);
        }


    }
}
