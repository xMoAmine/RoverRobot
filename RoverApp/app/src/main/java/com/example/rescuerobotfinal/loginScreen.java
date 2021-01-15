//Developed by Mohamed Amine Sahla IT201
//To-do: the use of negative numbers and incomplete IP-addresses and ports should be seen as invalid numbers.(-4 not allowed)
//To-do extra: Making an actual start screen instead of the login screen being the start screen.
//To-do extra: Adding texture and color for extra eye-candy for the application.

package com.example.rescuerobotfinal;

import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;

import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.view.View;
import android.widget.Toast;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import static java.lang.Integer.parseInt;

public class loginScreen extends AppCompatActivity {
    TextView textResponse;
    EditText address, port;
    Button buttonConnect, buttonClear;


    public static String serverIp;
    public static int serverPort;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.loginscreen);
        getSupportActionBar().hide();

        address = (EditText) findViewById(R.id.address);
        port = (EditText) findViewById(R.id.port);
        buttonConnect = (Button) findViewById(R.id.connect);
        buttonClear = (Button) findViewById(R.id.clear);
        textResponse = (TextView) findViewById(R.id.response);


        buttonConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (address.getText().toString().equals("") || port.getText().toString().equals("")) {
                    AlertDialog alertDialog = new AlertDialog.Builder(loginScreen.this).create();
                    alertDialog.setTitle("Niet alle velden zijn ingevuld");
                    alertDialog.setMessage("Vul alle velden in om verder te gaan");
                    alertDialog.setButton(AlertDialog.BUTTON_NEUTRAL, "OK", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int which) {
                            dialog.dismiss();
                        }
                    });
                    alertDialog.show();
                }else {
                    serverIp = address.getText().toString();
                    try {
                        serverPort = parseInt(port.getText().toString());
                    } catch (NumberFormatException e) {
                        serverPort = 0;
                    }

                    startActivity(new Intent(v.getContext(), MainActivity.class));
                    Toast.makeText(getApplicationContext(), "Connecting to Raspberry Pi", Toast.LENGTH_SHORT).show();

                }
            }


        });

        buttonClear.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                address.setText("");
                port.setText("");

            }
        });
    }

}

