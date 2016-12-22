package group14.beaglebonecontrol;

import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.ActivityNotFoundException;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Color;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.speech.RecognizerIntent;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Locale;
import java.util.Set;
import java.util.Timer;

public class MainActivity extends AppCompatActivity {
    private Button forward, backward, clockwise, counterclockwise, auto, voiceCommand, speed;
    private AlertDialog.Builder alertBuilder;
    private boolean autoOn = false;
    // private SendByte thread;
    private Timer timer;
    private WriteTask task;
    private final int REQ_CODE_SPEECH_INPUT = 100;
    private final int DEFAULT_SPEED = 300;
    private final int FASTER_SPEED = 600;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // get the buttons
        forward = (Button) findViewById(R.id.f);
        backward = (Button) findViewById(R.id.b);
        clockwise = (Button) findViewById(R.id.cw);
        counterclockwise = (Button) findViewById(R.id.ccw);
        auto = (Button) findViewById(R.id.auto);
        voiceCommand = (Button) findViewById(R.id.voiceButton);

        // builder for alert dialogue
        alertBuilder = new AlertDialog.Builder(this);
        alertBuilder.setMessage("");
        alertBuilder.setCancelable(true);

        alertBuilder.setPositiveButton(
                "Yes",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }
                });

        alertBuilder.setNegativeButton(
                "No",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }
                });

        // connect to bluetooth
        BluetoothDevice device = connectToBlueTooth();

        // command for forward button when pressed
        forward.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                // turn auto off
                if (autoOn) {
                    autoOn = false;
                }
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    // if can't connect bluetooth, show message
                    if (!task.canConnect()) {
                        alertProblem("Problem with bluetooth");
                    }
                    setButtonsOff();
                    forward.setBackgroundColor(Color.parseColor("#FF669900"));
                    // when forward is being pressed, send 1
                    task.setMessage('1');
                } else if (event.getAction() == MotionEvent.ACTION_UP ||
                        event.getAction() == MotionEvent.ACTION_CANCEL) {
                    // if can't connect bluetooth, show message
                    if (!task.canConnect()) {
                        alertProblem("Problem with bluetooth");
                    }
                    // when forward is released, send 0
                    forward.setBackgroundResource(android.R.drawable.btn_default);
                    task.setMessage('0');
                }
                return true;
            }
        });

        // set the command for the backward button when pressed
        backward.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                // turn auto off
                if (autoOn) {
                    autoOn = false;
                }
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    // if can't connect bluetooth, show message
                    if (!task.canConnect()) {
                        alertProblem("Problem with bluetooth");
                    }
                    setButtonsOff();
                    backward.setBackgroundColor(Color.parseColor("#FF669900"));
                    // when backward is being pressed, send 2
                    task.setMessage('2');
                } else if (event.getAction() == MotionEvent.ACTION_UP ||
                        event.getAction() == MotionEvent.ACTION_CANCEL) {
                    // if can't connect bluetooth, show message
                    if (!task.canConnect()) {
                        alertProblem("Problem with bluetooth");
                    }
                    backward.setBackgroundResource(android.R.drawable.btn_default);
                    // when backward is release, send 0
                    task.setMessage('0');
                }
                return true;
            }
        });

        // set command for clockwise button when pressed
        clockwise.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                // turn auto off
                if (autoOn) {
                    autoOn = false;
                }
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    // if can't connect bluetooth, show message
                    if (!task.canConnect()) {
                        alertProblem("Problem with bluetooth");
                    }
                    setButtonsOff();
                    clockwise.setBackgroundColor(Color.parseColor("#FF669900"));
                    // when clockwise is being pressed, send 3
                    task.setMessage('3');

                } else if (event.getAction() == MotionEvent.ACTION_UP ||
                        event.getAction() == MotionEvent.ACTION_CANCEL) {
                    // if can't connect bluetooth, show message
                    if (!task.canConnect()) {
                        alertProblem("Problem with bluetooth");
                    }
                    clockwise.setBackgroundResource(android.R.drawable.btn_default);
                    // when clockwise is released, send 0
                    task.setMessage('0');
                }
                return true;
            }
        });

        // set command for counterclockwise button when pressed
        counterclockwise.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                // turn auto off
                if (autoOn) {
                    autoOn = false;
                }
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    // if can't connect bluetooth, show message
                    if (!task.canConnect()) {
                        alertProblem("Problem with bluetooth");
                    }
                    setButtonsOff();
                    counterclockwise.setBackgroundColor(Color.parseColor("#FF669900"));
                    // when counterclockwise is pressed, send 4
                    task.setMessage('4');
                } else if (event.getAction() == MotionEvent.ACTION_UP ||
                        event.getAction() == MotionEvent.ACTION_CANCEL) {
                    // if can't connect bluetooth, show message
                    if (!task.canConnect()) {
                        alertProblem("Problem with bluetooth");
                    }
                    counterclockwise.setBackgroundResource(android.R.drawable.btn_default);
                    // when counterclockwise is released, send 0
                    task.setMessage('0');
                }
                return true;
            }
        });

        // set actions for clicking on auto
        auto.setOnClickListener(new View.OnClickListener() {
            // if click, set to auto if auto is on, else turn auto off
            @Override
            public void onClick(View v) {
                if (autoOn) {
                    // if auto is on, turn on and send 0
                    // if can't connect bluetooth, show message
                    if (!task.canConnect()) {
                        alertProblem("Problem with bluetooth");
                    }
                    setButtonsOff();
                    autoOn = false;
                    task.setMessage('0');
                } else {
                    // if auto is off, turn on and send 5
                    // if can't connect bluetooth, show message
                    if (!task.canConnect()) {
                        alertProblem("Problem with bluetooth");
                    }
                    setButtonsOff();
                    autoOn = true;
                    auto.setBackgroundColor(Color.parseColor("#FF33B5E5"));
                    task.setMessage('5');
                }
            }
        });

        // set commands for voice command button
        voiceCommand.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // turn on voice command
                setButtonsOff();
                promptSpeechInput();
            }
        });

        // set the task
        task = new WriteTask(device, alertBuilder, '0');
        timer = new Timer();
        timer.schedule(task, 0, 3);
    }

    // start the speech input
    private void promptSpeechInput() {
        Intent intent = new Intent(RecognizerIntent.ACTION_RECOGNIZE_SPEECH);
        intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE_MODEL,
                RecognizerIntent.LANGUAGE_MODEL_FREE_FORM);
        intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE, Locale.getDefault());
        intent.putExtra(RecognizerIntent.EXTRA_PROMPT,
                getString(R.string.speech_prompt));
        try {
            startActivityForResult(intent, REQ_CODE_SPEECH_INPUT);
        } catch (ActivityNotFoundException a) {
            Toast.makeText(getApplicationContext(),
                    getString(R.string.speech_not_supported),
                    Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        switch (requestCode) {
            case REQ_CODE_SPEECH_INPUT: {
                if (resultCode == RESULT_OK && null != data) {

                    ArrayList<String> result = data
                            .getStringArrayListExtra(RecognizerIntent.EXTRA_RESULTS);

                    for (int i = 0; i < result.size(); i++) {
                        String s = result.get(i);
                        if (s.equalsIgnoreCase("go") || s.equalsIgnoreCase("forward") ||
                                s.equalsIgnoreCase("go forward")) {
                            // if said go, go forward
                            forward.setBackgroundColor(Color.parseColor("#FF669900"));
                            task.setMessage('1');
                            break;
                        } else if (s.equalsIgnoreCase("right") || s.equalsIgnoreCase("go right")) {
                            // if said right, turn clockwise
                            clockwise.setBackgroundColor(Color.parseColor("#FF669900"));
                            task.setMessage('3');
                            break;
                        } else if (s.equalsIgnoreCase("left") || s.equalsIgnoreCase("go left")) {
                            // if said left, turn clockwise
                            counterclockwise.setBackgroundColor(Color.parseColor("#FF669900"));
                            task.setMessage('4');
                            break;
                        } else if (s.equalsIgnoreCase("back") || s.equalsIgnoreCase("go back")) {
                            // if said back, go backward
                            backward.setBackgroundColor(Color.parseColor("#FF669900"));
                            task.setMessage('2');
                            break;
                        } else if (s.equalsIgnoreCase("auto") || s.equalsIgnoreCase("self drive") ||
                                result.contains("go cray cray")) {
                            // if said auto, set to auto
                            auto.setBackgroundColor(Color.parseColor("#FF33B5E5"));
                            autoOn = true;
                            task.setMessage('5');
                            break;
                        }
                        if (i == result.size() - 1) {
                            autoOn = false;
                            task.setMessage('0');
                        }
                    }
                }
                break;
            }

        }
    }

    // set the buttons colors back to default
    private void setButtonsOff() {
        forward.setBackgroundResource(android.R.drawable.btn_default);
        backward.setBackgroundResource(android.R.drawable.btn_default);
        clockwise.setBackgroundResource(android.R.drawable.btn_default);
        counterclockwise.setBackgroundResource(android.R.drawable.btn_default);
        auto.setBackgroundResource(android.R.drawable.btn_default);
    }

    // print message if problem with bluetooth
    private void alertProblem(String msg) {
        alertBuilder.setMessage(msg);
        AlertDialog alertBox = alertBuilder.create();
        alertBox.show();
    }

    /*
        Connect to bluetooth device
     */
    private BluetoothDevice connectToBlueTooth() {
        String deviceName = "CAS";
        BluetoothDevice bt = null;
        BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
        if (adapter == null) {
            alertProblem("No Bluetooth device connected");
        }
        if (!adapter.isEnabled()) {
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
        }
        Set<BluetoothDevice> devices = adapter.getBondedDevices();
        if (devices != null) {
            for (BluetoothDevice device : devices) {
                if (deviceName.equals(device.getName())) {
                    bt = device;
                    break;
                }
            }
        } else {
            alertProblem("No devices");
        }
        if (bt == null) {
            alertProblem("CAS is not connected");
        }
        return bt;
    }
}
