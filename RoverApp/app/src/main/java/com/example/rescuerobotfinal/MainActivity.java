//Developed by: Mohamed Amine Sahla IT201
//Last changes made, Final delivery almost complete

package com.example.rescuerobotfinal;

import android.content.Intent;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;


import androidx.appcompat.app.AppCompatActivity;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class MainActivity extends AppCompatActivity {
    TextView speedTextField, distanceTextField;
    ImageButton buttonDown, buttonUp, buttonLeft, buttonRight, buttonSpeedUp, buttonSensorDown,
            settings;
    WebView webView;
    String SERVERIP = loginScreen.serverIp;
    int SERVERPORT = loginScreen.serverPort;

    public static String text = "";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        getSupportActionBar().hide();


        webView = (WebView) findViewById(R.id.webView);
        WebSettings webSettings = webView.getSettings();// Gets the WebSettings object used to control the settings for this WebView.
        webSettings.setJavaScriptEnabled(true); //Tells the WebView to enable JavaScript execution. The default is false.
        webView.setInitialScale(500);
        webSettings.setUseWideViewPort(true);//doesn't mess with the width and length of the camera (To Be Checked).
        webView.loadUrl("http://" + loginScreen.serverIp + ":8000/index.html"); // stream laden vanaf de URL


        final ClientAsyncTask clientAsyncTask = new ClientAsyncTask();

        buttonUp = (ImageButton) findViewById(R.id.buttonUp);
        buttonUp.setOnTouchListener(new View.OnTouchListener() {

            public boolean onTouch(View view, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    text = "w";
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    text = "x";

                }
                final ClientAsyncTask clientAsyncTask = new ClientAsyncTask();
                clientAsyncTask.execute(new String[]{""});
                return true;
            }
        });


        buttonDown = (ImageButton) findViewById(R.id.buttonDown);
        buttonDown.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    text = "s";
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    text = "x";

                }
                final ClientAsyncTask clientAsyncTask = new ClientAsyncTask();
                clientAsyncTask.execute(new String[]{""});
                return true;
            }
        });


        buttonLeft = (ImageButton) findViewById(R.id.buttonLeft);
        buttonLeft.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    text = "a";
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    text = "x";

                }
                final ClientAsyncTask clientAsyncTask = new ClientAsyncTask();
                clientAsyncTask.execute(new String[]{""});
                return true;
            }
        });

        buttonRight = (ImageButton) findViewById(R.id.buttonRight);
        buttonRight.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    text = "d";
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    text = "x";

                }
                final ClientAsyncTask clientAsyncTask = new ClientAsyncTask();
                clientAsyncTask.execute(new String[]{""});
                return true;
            }
        });

        buttonSpeedUp = (ImageButton) findViewById(R.id.buttonSpeedUp);
        buttonSpeedUp.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    text = "x";
                }else {}
                final ClientAsyncTask clientAsyncTask = new ClientAsyncTask();
                clientAsyncTask.execute(new String[]{""});
                return true;
            }
        });

        buttonSensorDown = (ImageButton) findViewById(R.id.buttonSpeedDown);
        buttonSensorDown.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    text = "k";
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    text = "x";

                }
                final ClientAsyncTask clientAsyncTask = new ClientAsyncTask();
                clientAsyncTask.execute(new String[]{""});
                return true;
            }
        });

        buttonSensorDown = (ImageButton) findViewById(R.id.buttonBulldozerBlade);
        buttonSensorDown.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    text = "b";
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    text = "x";

                }
                final ClientAsyncTask clientAsyncTask = new ClientAsyncTask();
                clientAsyncTask.execute(new String[]{""});
                return true;
            }
        });

        buttonSensorDown = (ImageButton) findViewById(R.id.buttonSensorLeft);
        buttonSensorDown.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    text = "k";
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    text = "x";

                }
                final ClientAsyncTask clientAsyncTask = new ClientAsyncTask();
                clientAsyncTask.execute(new String[]{""});
                return true;
            }
        });

        settings = (ImageButton) findViewById(R.id.settings);
        settings.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                openSettings();
                return true;
            }
        });
    }

    private void openSettings() {
        Intent intent = new Intent(this, settings.class);
        startActivity(intent);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    class ClientAsyncTask extends AsyncTask<String, Void, String> {
        @Override
        protected String doInBackground(String... params) {
            String result = null;
            try {
                //Create a client socket and define internet address and the port of the server
                Socket socket = new Socket(SERVERIP, SERVERPORT);
                //Get the input stream of the client socket
                InputStream inputStream = socket.getInputStream();
                //Get the output stream of the client socket
                PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
                //Write data to the output stream of the client socket
                System.out.println(text);
                out.println(text);
                System.out.println(text);
                text = "";
                System.out.println(text);

                //Buffer the data coming from the input stream
                BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(inputStream));
                //Read data in the input buffer
                result = bufferedReader.readLine();
            }catch(Exception e){
                e.printStackTrace();
                return "Exception";
            }
            return result;

        }


        @Override
        protected void onPostExecute(String result) {
            super.onPostExecute(result);

            if (result.equals("Exception")) {

                finish();
                Log.d("Test", "aantal");
                final Toast toast = Toast.makeText(getApplicationContext(), "Cannot Connect Try Again", Toast.LENGTH_SHORT);
                toast.show();

                Handler handler = new Handler();
                handler.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        toast.cancel();
                    }
                }, 1000);
            }
        }
    }

}