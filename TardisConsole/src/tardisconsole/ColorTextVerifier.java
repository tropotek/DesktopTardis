/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package tardisconsole;

import java.util.Set;
import java.util.HashSet;

import java.awt.*;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.event.*;
import javax.swing.text.*;

import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeEvent;

import java.text.*;


/**
 *
 * @author godar
 */
public class ColorTextVerifier extends InputVerifier implements ActionListener {
    
    /**
     * 
     * @param input
     * @return 
     */
    public boolean shouldYieldFocus(JComponent input) {
        boolean inputOK = verify(input);
        if (inputOK) {
            return true;
        } else {
            Toolkit.getDefaultToolkit().beep();
            return false;
        }
    }
    
    /**
     * 
     * @param input
     * @return 
     */
    public boolean verify(JComponent input) {
        int i = Integer.parseInt( ((JTextField)input).getText() );
        if (i < 0) {
            ((JTextField)input).setText("0");
            return false;
        }
        if (i > 255) {
            ((JTextField)input).setText("255");
            return false;
        }
        return true;
    }
    
    /**
     * 
     * @param e 
     */
    public void actionPerformed(ActionEvent e) {
        JTextField source = (JTextField)e.getSource();
        shouldYieldFocus(source); //ignore return value
        source.selectAll();
    }
}
