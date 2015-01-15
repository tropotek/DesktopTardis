/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package tardisconsole;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import gnu.io.CommPortIdentifier; 
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent; 
import gnu.io.SerialPortEventListener; 
import java.util.Arrays;
import java.util.Enumeration;



/**
 *
 * @author mifsudm
 */
public class TardisInterface implements SerialPortEventListener {
    
    public SerialPort serialPort;
    public static BufferedReader input;
    public static OutputStream output;
    /** Milliseconds to block while waiting for port open */
    public static final int timeOut = 2000;
    private String comPort = "COM4";
    private int baud = 57600;
    
    
    /** Temp in Deg C */
    private double temp = 0;
    /** top light on/off */
    private boolean topLedEnabled = true;
    /** Delay in ms */
    private int topDelay = 500;
    /** top light RGB color */
    private int[] topLed = {255, 255, 255};
    
    /** Window light on/off */
    private boolean winLedEnabled = true;
    /** Enable window color to be set by temp sensor.
     * @todo Disable the color input field if this is true.
     */
    private boolean winTempLed = true;
    /** Window light default color */
    private int[] winLed = {255, 255, 255};
    
    /** The current track ID */
    private int trackId = 0;
    /** is true if the track is currently playing */
    private boolean isPlayingTrack = false;
    /** If true the next time an update is sent the selected track should begin playing */
    private boolean playTrack = false;
    
    
        
    /**
     * _constructor
     * 
     * @param comPort
     * @param baud 
     */
    public TardisInterface(String comPort, int baud) throws Exception
    {
        setComPort(comPort);
        setBaud(baud);
        connectSerial();
    }
    
    
    /**
     * 
     */
    public void connectSerial() throws Exception 
    {
        CommPortIdentifier portId = null;
        Enumeration portEnum = CommPortIdentifier.getPortIdentifiers();
        //First, Find an instance of serial port as set in PORT_NAMES.
        while (portEnum.hasMoreElements()) {
            CommPortIdentifier currPortId = (CommPortIdentifier) portEnum.nextElement();
            //System.out.println(currPortId.getName());
            if (currPortId.getName().equals(comPort)) {
                portId = currPortId;
                break;
            }
        }
        
        if (portId == null) {
            System.out.println("Could not find COM port: " + comPort);
            return;
        }

        try {
            // open serial port, and use class name for the appName.
            serialPort = (SerialPort) portId.open(this.getClass().getName(),
                    timeOut);

            // set port parameters
            serialPort.setSerialPortParams(baud,
                    SerialPort.DATABITS_8,
                    SerialPort.STOPBITS_1,
                    SerialPort.PARITY_NONE);

            // open the streams
            input = new BufferedReader(new InputStreamReader(serialPort.getInputStream()));
            output = serialPort.getOutputStream();
            char ch = 1;
            output.write(ch);

            // add event listeners
            serialPort.addEventListener(this);
            serialPort.notifyOnDataAvailable(true);
            System.out.println("TARDIS: Connected");
        } catch (Exception e) {
            throw e;
        }
    }
    
    public synchronized void close() 
    {
        if (serialPort != null) {
            serialPort.removeEventListener();
            serialPort.close();
            System.out.println("TARDIS: Disconnected");
        }
    }

    public synchronized void serialEvent(SerialPortEvent oEvent) 
    {
        if (oEvent.getEventType() == SerialPortEvent.DATA_AVAILABLE) {
            try {
                String inputLine = input.readLine();
                readData(inputLine);
            } catch (Exception e) {
                System.err.println(e.toString());
            }
        }

    }

    public static synchronized void writeData(String data) 
    {
        System.out.println("Sent: " + data);
        try {
            data += "\n";
            output.write(data.getBytes());
        } catch (Exception e) {
            System.out.println("Could not write to port: " + e.toString());
        }
    }
 
    /**
     * 
     */
    public void readData(String str)
    {
        // Read the settings from the TARDIS Serial Port
        if (str.startsWith("#")) { // Is a system comment/log
            System.out.println(str);
        } else if (str.startsWith("S00")) { // receving TARDIS update
            String[] data = str.substring(4).split(" ");
            
            System.out.println("-----------------------------------");
            System.out.println(topLedEnabled);
            System.out.println(data[1]);
            System.out.println(data[1].trim().equals("1"));
            System.out.println("-----------------------------------");
            
            temp = Double.parseDouble(data[0]);
            topLedEnabled = data[1].trim().equals("1");
            topDelay = Integer.parseInt(data[2]);
            topLed[0] = Integer.parseInt(data[3]);
            topLed[1] = Integer.parseInt(data[4]);
            topLed[2] = Integer.parseInt(data[5]);
            winLedEnabled = data[6].trim().equals("1");
            winTempLed = data[7].trim().equals("1");
            winLed[0] = Integer.parseInt(data[8]);
            winLed[1] = Integer.parseInt(data[9]);
            winLed[2] = Integer.parseInt(data[10]);
            trackId = Integer.parseInt(data[11]);
            isPlayingTrack = data[12].trim().equals("1");
            //System.out.println("Interface Update: " + str);
        } else {
            System.out.println("Unknown CMD: " + str);
        }
    }
    
    public synchronized void writeSettings()
    {
        System.out.println(this.toString());
        String data = "S01 " +
            temp+" "+
            topLedEnabled+" "+
            topDelay+" "+
            topLed[0]+" "+
            topLed[1]+" "+
            topLed[2]+" "+
            winLedEnabled+" "+
            winTempLed+" "+
            winLed[0]+" "+
            winLed[1]+" "+
            winLed[2]+" "+
            trackId+" "+
            isPlayingTrack+" "+
            playTrack+" "+    // Play track switch 
            "0 " +  // Reserved
            "0 " +  // Reserved
            "0 " +  // Reserved
            "0 " +  // Reserved
            "0 "    // Reserved
            ;
        playTrack = false;  // Once sent then no longer needed
        writeData(data);
    }
    
    
    
    /**
     * 
     * @param s 
     */
    public void setComPort(String s) {
        comPort = s;
    }
    
    /**
     * 
     * @param i 
     */
    public void setBaud(int i) {
        //"4800", "9600", "14400", "19200", "28800", "38400", "57600", "115200"
        switch(i) {
            case 4800:
            case 9600:
            case 14400:
            case 19200:
            case 28800:
            case 38400:
            case 57600:
            case 115200:
                baud = i;
                break;
            default:
                baud = 9600;
        }
    }

    
    public void enableTopLed(boolean b) {
        topLedEnabled = b;
    }

    public void setTopColor(int r, int g, int b) {
        topLed[0] = r;
        topLed[1] = g;
        topLed[2] = b;
    }

    public void setTopDelay(int i) {
        topDelay = i;
    }

    public void enableWinLed(boolean b) {
        winLedEnabled = b;
    }

    public void setWinColor(int r, int g, int b) {
        winLed[0] = r;
        winLed[1] = g;
        winLed[2] = b;
    }

    public void enableWinTempLed(boolean b) {
        winTempLed = b;
    }

    public void setTrackId(int i) {
        trackId = i;
    }
    

    public double getTemp() {
        return temp;
    }

    public boolean topLedEnabled() {
        return topLedEnabled;
    }

    public int[] getTopLed() {
        return topLed;
    }

    public int getTopDelay() {
        return topDelay;
    }

    public boolean winLedEnabled() {
        return winLedEnabled;
    }

    public int[] getWinLed() {
        return winLed;
    }

    public boolean winTempLedEnabled() {
        return winTempLed;
    }

    public int getTrackId() {
        return trackId;
    }

    public boolean isPlayingTrack() {
        return isPlayingTrack;
    }
    
    public void playTrack(int i)
    {
        setTrackId(i);
        playTrack = true;
    }
    
    
    
    public String toString()
    {
        String str = 
                "Temp: " + temp + "\n" +
                "topLedEnabled: " + topLedEnabled + "\n" +
                "topDelay: " + topDelay + "\n" +
                "topLed: ["+topLed[0]+","+topLed[1]+","+topLed[2]+"]\n" +
                "winLedEnabled: " + winLedEnabled + "\n" +
                "winLed: ["+winLed[0]+","+winLed[1]+","+winLed[2]+"]\n" +
                "winTempLed: " + winTempLed + "\n" +
                "trackId: " + trackId + "\n" +
                "isPlayingTrack: " + isPlayingTrack + "\n"  +
                "playTrack: " + playTrack + "\n" 
                ;
        return str;
    }
    
    
}
