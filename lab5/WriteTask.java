package group14.beaglebonecontrol;

import android.app.AlertDialog;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;

import java.io.IOException;
import java.util.TimerTask;
import java.util.UUID;

/**
 * Created by Daisy on 12/4/2016.
 */

public class WriteTask extends TimerTask {
    private BluetoothDevice device;
    private BluetoothSocket socket;
    private AlertDialog.Builder alertBuilder;
    private char msg;
    private int speed;
    static private final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private boolean canConnect;

    public WriteTask(BluetoothDevice dev, AlertDialog.Builder builder, char mes) {
        device = dev;
        alertBuilder = builder;
        msg = mes;
        canConnect = true;
        this.speed = speed;
        connectSocket();
    }

    @Override
    public void run() {
        // send message to bluetooth
        canConnect = true;
        write();
    }

    public void setMessage(char msg) {
        this.msg = msg;
    }

    // write message to bluetooth
    private void write() {
        // if bluetooth isn't connected, connect to bluetooth socket
        if (!socket.isConnected()) {
            connectSocket();
        }
        try {
            // write to bluetooth
            socket.getOutputStream().write((int) msg);
        } catch (IOException e) {
        }
    }

    // connect to bluetooth socket
    private void connectSocket() {
        try {
            socket = device.createInsecureRfcommSocketToServiceRecord(myUUID);
        } catch (IOException e) {
        }
        try {
            socket.connect();
        } catch (IOException e) {
            // set canConnect to false if cannot connect
            canConnect = false;
        }
    }

    // return if can connect to bluetooth socket or not
    public boolean canConnect() {
        return canConnect;
    }
}
