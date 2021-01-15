package com.example.rescuerobotfinal;

import android.content.Intent;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.ImageButton;
import android.widget.Switch;

import androidx.appcompat.app.AppCompatActivity;

//import android.content.Intent;

public class settings extends AppCompatActivity {
    ImageButton goBack;
    Switch LDR, compass;

    public static String text = "";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);

        LDR = (Switch) findViewById(R.id.LDR);
        LDR.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    text = "x";
                } else {
                    text = "k";
                }
                //final MainActivity.ClientAsyncTask clientAsyncTask = new MainActivity.ClientAsyncTask();
                //clientAsyncTask.execute(new String[]{""});
            }
        });

        compass = (Switch) findViewById(R.id.compass);
        compass.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    text = "x";
                } else {
                    text = "k";
                }
                //final MainActivity.ClientAsyncTask clientAsyncTask = new MainActivity.ClientAsyncTask();
                //clientAsyncTask.execute(new String[]{""});
            }
        });


        goBack = (ImageButton) findViewById(R.id.goBack);
        goBack.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                openGoBack();
                return true;
            }
        });
    }

    private void openGoBack() {
        Intent intent = new Intent(this, MainActivity.class);
        startActivity(intent);
    }
}
