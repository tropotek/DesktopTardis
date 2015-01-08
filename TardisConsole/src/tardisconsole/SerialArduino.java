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
 * TODO: 
 * 
 * We should move all serial Com's to here, and communicate to
 * Arduino in a re-usable way so other project can benefit
 * Consider using JASON strings for communications.
 *
 * We could use an adapter to translate the sent/received data.
 * 
 * @author mifsudm
 */
public class SerialArduino {
    
    
    public SerialArduino()
    {
        
    }
    
    
    public void sendData(String s)
    {
        
    }
    
    
    
}
