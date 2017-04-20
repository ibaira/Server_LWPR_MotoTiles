package com.livelife.playwaretestapp;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;


public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    Button startGame,rankingButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        startGame = (Button) findViewById(R.id.startGame);
        startGame.setOnClickListener(this);
        rankingButton = (Button) findViewById(R.id.rankingButton);
        rankingButton.setOnClickListener(this);

    }

    @Override
    public void onClick(View view) {

        if(view.getId() == R.id.startGame) {
            Intent intent = new Intent(this, GameActivity.class);
            startActivity(intent);
        }
        else if(view.getId() == R.id.rankingButton) {
            Intent intent = new Intent(this, RankingActivity.class);
            startActivity(intent);
        }

    }

}
