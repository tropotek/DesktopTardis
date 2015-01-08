/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package tardisconsole;

import tardisconsole.TardisInterface;


/**
 *
 * @author mifsudm
 */
public class TardisConsole {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        
        //Create and set up the window.	 
	//javax.swing.JFrame frame = new TardisConsoleUI(new TardisInterface());
	javax.swing.JFrame frame = new TardisConsoleUI(new TardisInterface("COM4", 57600));
        //Display the window.
	frame.pack(); 
	frame.setVisible(true);
        
    }
    
    
}
