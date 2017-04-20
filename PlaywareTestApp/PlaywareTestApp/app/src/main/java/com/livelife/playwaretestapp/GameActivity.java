package com.livelife.playwaretestapp;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.util.HashMap;
import java.util.Random;

import dk.dtu.antlibrary.AntConnection;
import dk.dtu.antlibrary.AntConnectionException;
import dk.dtu.antlibrary.AntData;
import dk.dtu.antlibrary.OnMessageReceivedListener;


public class GameActivity extends AppCompatActivity implements OnMessageReceivedListener {

    byte messageId = 0;
    byte TILE_1 = 1;
    byte TILE_2 = 2;
    byte TILE_3 = 3;
    byte TILE_4 = 4;

    HashMap<Byte,AntData> tilesData = new HashMap<>();
    byte [] tilesSet = new byte[] {TILE_1,TILE_2};

    android.os.Handler handler = new android.os.Handler();
    int time = 1000;
    long sessionTimeSeconds = 30;

    Button startButton,gobackButton;
    TextView timeTextView,scoreTextView;
    EditText playerName;
    String playerNameText = "Player";

    DBHelper dbHelper;

    boolean playing = false;
    int score = 0;

    Runnable runnable = new Runnable() {
        @Override
        public void run() {


            sessionTimeSeconds -= 1;

            if(sessionTimeSeconds == 0) {
                startButton.setEnabled(true);
                startButton.setAlpha(1.0f);
                playing = false;
                resetTiles();
                dbHelper.insertSession(playerName.getText().toString(),score);
            } else {
                handler.postDelayed(this, time);
            }

            timeTextView.setText(""+sessionTimeSeconds+"");
        }
    };

    @Override
    protected void onDestroy() {
        super.onDestroy();

        AntConnection.getInstance().stopANT();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_game);


        playerName = (EditText) findViewById(R.id.playerName);
        playerName.setText(playerNameText);

        startButton = (Button) findViewById(R.id.startButton);
        startButton.setEnabled(false);
        startButton.setAlpha(0.4f);

        gobackButton = (Button) findViewById(R.id.gobackButton);
        timeTextView = (TextView) findViewById(R.id.timeTextView);
        scoreTextView = (TextView) findViewById(R.id.scoreTextView);


        startButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                score = 0;
                sessionTimeSeconds = 30;
                timeTextView.setText(""+sessionTimeSeconds+"");

                startButton.setEnabled(false);
                startButton.setAlpha(0.4f);

                playing = true;
                nextGameStep(0);
                handler.postDelayed(runnable,time);
            }
        });

        gobackButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                handler.removeCallbacksAndMessages(null);
                finish();
            }
        });


        dbHelper = new DBHelper(this);

        AntConnection.getInstance().setDebug(true);
        AntConnection.getInstance().registerMessageReceivedListener(this);
        AntConnection.getInstance().startANT(this, 7, true);


    }

    void resetTiles() {


        for (byte tileId : tilesSet) {
            messageId++;

            AntData d;

            if(tilesData.containsKey(tileId)) {
                d = tilesData.get(tileId);
            } else {
                d = new AntData(tileId);
            }

            d.setBroadcastData(new byte[]{tileId, messageId, Constants.CMD_SET_COLOR, 0x02, 0, 0, 0, 0});
            tilesData.put(tileId,d);


            try {
                AntConnection.getInstance().update(d);
            } catch (AntConnectionException e) {
                e.printStackTrace();
            }
        }



    }


    @Override
    public void onMessageReceived(byte[] bytes) {
        byte tileId = bytes[0];
        byte tileEvent = bytes[1];
        byte tileColor = bytes[2];


        if(tileEvent == Constants.EVENT_PRESS && tileColor == Constants.LED_COLOR_RED && playing) {


            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    score++;
                    scoreTextView.setText(""+score+"");
                }
            });
            nextGameStep(tileId);
        }

    }

    @Override
    public void onAntServiceConnected(boolean connected) {
        if(connected) {
            startButton.setEnabled(true);
            startButton.setAlpha(1.0f);
            resetTiles();
        }
    }

    public int getNextTile(int lastTileId) {


        boolean trying = true;
        int nextTileId = 0; // 0 is only to initialize the value

        while(trying) {
            Random r = new Random();
            nextTileId = r.nextInt(tilesSet.length) + 1; // Generates random int between 1 and 4
            if(nextTileId != lastTileId) {
                trying = false;
            }
        }

        return nextTileId;


    }

    int nextTileId;


    public void nextGameStep(int tile) {


        // We have received a press from a red tile
        // Now we will search for the next tile to light up taking care it's not the last one pressed

        nextTileId = getNextTile(tile);

        for (byte tileId : tilesSet) {
            messageId++;
            AntData d;

            if(tilesData.containsKey(tileId)) {
                d = tilesData.get(tileId);
            } else {
                d = new AntData(tileId);
            }

            if(tileId == nextTileId) {
                d.setBroadcastData(new byte[]{tileId, messageId, Constants.CMD_SET_COLOR, Constants.LED_COLOR_RED, 0, 0, 0, 0});
            } else {
                d.setBroadcastData(new byte[]{tileId, messageId, Constants.CMD_SET_COLOR, Constants.LED_COLOR_OFF, 0, 0, 0, 0});
            }

            tilesData.put(tileId,d);


            try {
                AntConnection.getInstance().update(d);
            } catch (AntConnectionException e) {
                e.printStackTrace();
            }
        }


    }

}
