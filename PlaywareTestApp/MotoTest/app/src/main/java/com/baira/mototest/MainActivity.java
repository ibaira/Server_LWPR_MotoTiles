package com.baira.mototest;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import java.util.HashMap;
import java.util.Random;

import dk.dtu.antlibrary.AntConnection;
import dk.dtu.antlibrary.AntConnectionException;
import dk.dtu.antlibrary.AntData;
import dk.dtu.antlibrary.OnMessageReceivedListener;

public class MainActivity extends AppCompatActivity implements OnMessageReceivedListener {


    Button button;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        AntConnection.getInstance().setDebug(true); // Receive debug messages
        // Register the messages received with this activity class. When tile send event report it will be triggered inside the listener method (8 bytes)
        // Event type (press, released, color)
        AntConnection.getInstance().registerMessageReceivedListener(this);
        AntConnection.getInstance().startANT(this, 7, true); // Master ID tablet

        // Create button
        button = (Button) findViewById(R.id.button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                changeColor();
            }
        });
        button.setEnabled(false);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        AntConnection.getInstance().stopANT();
    }

    @Override
    public void onMessageReceived(byte[] bytes) {

    }


    public void changeColor() {

        Random r1 = new Random();
        int color =  r1.nextInt(4)+1;
        int tileId = r1.nextInt(2)+1;

        AntData data;
        int message_id = 1;
        if(tilesData.containsKey((byte)tileId)) {
            data = tilesData.get((byte)tileId);
            message_id = data.getBroadcastData()[1];
            Log.v("NBV","message_id: "+message_id+" deviceId: "+tileId);
        } else {
            data = new AntData((byte)tileId);
        }

        // id_tile, message_id incremented every time you send a new message, command_code, color_code, 4 zeros: for byte shifting and 8 different color to set
        data.setBroadcastData(new byte[]{(byte)tileId, (byte) message_id, 0x02, (byte) color, 0, 0, 0, 0});
        tilesData.put((byte)tileId,data);
        try {
            AntConnection.getInstance().update(data);
            Log.v("AntData","Data sent...");
        } catch (AntConnectionException e) {
            e.printStackTrace();
        }

    }

    HashMap<Byte,AntData> tilesData = new HashMap<>();  // Key, and Data

    @Override
    public void onAntServiceConnected(boolean b) {
        button.setEnabled(true);
        //changeColor();
    }

}
