package com.livelife.playwaretestapp;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

import java.util.ArrayList;

/**
 * Created by Max on 09/03/2017.
 */

public class DBHelper extends SQLiteOpenHelper {

    public static final String DATABASE_NAME = "Main.db";



    public DBHelper(Context context) {

        super(context, DATABASE_NAME , null, 1);

    }

    @Override
    public void onCreate(SQLiteDatabase sqLiteDatabase) {
        String sql_sessions = "CREATE TABLE sessions (session_id integer primary key autoincrement not null, user_name text, score integer)";
        sqLiteDatabase.execSQL(sql_sessions);
    }

    @Override
    public void onUpgrade(SQLiteDatabase sqLiteDatabase, int i, int i1) {
        // Do nothing
    }


    public boolean insertSession(String user_name, int score) {
        SQLiteDatabase db = this.getWritableDatabase();

        ContentValues contentValues = new ContentValues();
        contentValues.put("user_name",user_name);
        contentValues.put("score", score);

        db.insert("sessions",null,contentValues);

        db.close();

        return true;
    }


    public ArrayList<Session> getAllSessions() {

        ArrayList<Session> sessions = new ArrayList<>();

        SQLiteDatabase db = this.getReadableDatabase();

        Cursor res = db.rawQuery("SELECT * FROM sessions ORDER BY score DESC",null);

        while(res.moveToNext()) {
            String name = res.getString(res.getColumnIndex("user_name"));
            int score = res.getInt(res.getColumnIndex("score"));
            int rowId = res.getInt(res.getColumnIndex("session_id"));

            Session s = new Session(name,score,rowId);
            sessions.add(s);
        }

        res.close();
        db.close();

        return sessions;


    }
}
