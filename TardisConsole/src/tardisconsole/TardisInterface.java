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
    private int temp = 0;
    /** top light on/off */
    private boolean topLight = false;
    /** top light RGB color */
    private int topColor[] = {0, 0, 0};
    /** Delay in ms */
    private int topDelay = 500;
    /** Window light on/off */
    private boolean winLight = false;
    /** Window light default color */
    private int winColor[] = {0, 0, 0};
    /** Enable window color to be set by temp sensor.
     * @todo Disable the color input field if this is true.
     */
    private boolean winTempSensor = false;
    /** The current track ID */
    private int trackId = 0;
    /** is true if the track is currently playing */
    private boolean isPlayingTrack = false;
    
    
    
    /**
     * _constructor
     * 
     */
    public TardisInterface()
    {
        
        
    }
    
    /**
     * _constructor
     * 
     * @param comPort
     * @param baud 
     */
    public TardisInterface(String comPort, int baud)
    {
        setComPort(comPort);
        setBaud(baud);
        connectSerial();
    }
    
    
    /**
     * 
     */
    public void connectSerial() 
    {
        CommPortIdentifier portId = null;
        Enumeration portEnum = CommPortIdentifier.getPortIdentifiers();

        //First, Find an instance of serial port as set in PORT_NAMES.
        while (portEnum.hasMoreElements()) {
            CommPortIdentifier currPortId = (CommPortIdentifier) portEnum.nextElement();
            
            System.out.println(currPortId.getName());
            
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
        } catch (Exception e) {
            System.err.println(e.toString());
        }
    }
    
    public synchronized void close() 
    {
        if (serialPort != null) {
            serialPort.removeEventListener();
            serialPort.close();
        }
    }

    public synchronized void serialEvent(SerialPortEvent oEvent) 
    {
        if (oEvent.getEventType() == SerialPortEvent.DATA_AVAILABLE) {
            try {
                String inputLine = input.readLine();
                System.out.println(inputLine);
            } catch (Exception e) {
                System.err.println(e.toString());
            }
        }

    }

    public static synchronized void writeData(String data) 
    {
        System.out.println("Sent: " + data);
        try {
            output.write(data.getBytes());
        } catch (Exception e) {
            System.out.println("could not write to port");
        }
    }
 
    
    
    
    /**
     * 
     */
    public void readSettings()
    {
       // Read the settings from the TARDIS Serial Port
        
        
        
    }
    
    public void writeSettings()
    {
        // Write these settings to the serial port.
        
        
    }
    
    
    
    /**
     * 
     * @param s 
     */
    public void setComPort(String s) 
    {
        // TODO: validate port
        comPort = s;
    }
    
    
    /**
     * 
     * @param i 
     */
    public void setBaud(int i)
    {
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

    public void setTopLight(boolean topLight) {
        this.topLight = topLight;
    }

    public void setTopColor(int r, int g, int b) {
        topColor[0] = r;
        topColor[1] = g;
        topColor[2] = b;
    }

    public void setTopDelay(int topDelay) {
        this.topDelay = topDelay;
    }

    public void setWinLight(boolean winLight) {
        this.winLight = winLight;
    }

    public void setWinColor(int r, int g, int b) {
        winColor[0] = r;
        winColor[1] = g;
        winColor[2] = b;
    }

    public void setWinTempSensor(boolean winTempSensor) {
        this.winTempSensor = winTempSensor;
    }

    public void setTrackId(int trackId) {
        this.trackId = trackId;
    }
    
    
    
    

    public int getTemp() {
        return temp;
    }

    public boolean isTopLight() {
        return topLight;
    }

    public int[] getTopColor() {
        return topColor;
    }

    public int getTopDelay() {
        return topDelay;
    }

    public boolean isWinLight() {
        return winLight;
    }

    public int[] getWinColor() {
        return winColor;
    }

    public boolean isWinTempSensor() {
        return winTempSensor;
    }

    public int getTrackId() {
        return trackId;
    }

    public boolean isIsPlayingTrack() {
        return isPlayingTrack;
    }
    
    public void playTrack(int i)
    {
        setTrackId(i);
        // Send a message to the tardis to stop any existing and play new
        System.out.println("Playing Track "+i);
    }
    
    
    
    public String toString()
    {
        String str = 
                "Temp: " + temp + "\n" +
                "topLight: " + topLight + "\n" +
                "topDelay: " + topDelay + "\n" +
                "topColor: ["+topColor[0]+","+topColor[1]+","+topColor[2]+"]\n" +
                "winLight: " + winLight + "\n" +
                "winTempSensor: " + winTempSensor + "\n" +
                "winColor: ["+winColor[0]+","+winColor[1]+","+winColor[2]+"]\n" +
                "trackId: " + trackId + "\n" +
                "isPlayingTrack: " + isPlayingTrack + "\n" 
                ;
        return str;
    }
    
    
}
