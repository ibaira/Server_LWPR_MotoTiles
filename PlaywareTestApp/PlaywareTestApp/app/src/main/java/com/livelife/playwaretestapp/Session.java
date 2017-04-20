package com.livelife.playwaretestapp;

/**
 * Created by Max on 09/03/2017.
 */

public class Session {

    String user_name;
    int score;
    int rowId;

    public Session(String user_name, int score, int rowId) {
        this.user_name = user_name;
        this.score = score;
        this.rowId = rowId;
    }

}
