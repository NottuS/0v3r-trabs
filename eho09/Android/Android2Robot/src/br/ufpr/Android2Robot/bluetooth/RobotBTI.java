package br.ufpr.Android2Robot.bluetooth;

import java.util.ArrayList;

import android.bluetooth.BluetoothDevice;
import android.util.Log;

public class RobotBTI {
	public static final byte LEFT = 0;
	public static final byte FOWARD = 1;
	public static final byte RIGHT = 2;
	public static final byte BACK = 3 ;
	public static final byte STOP = 4;	
	public static final byte ANSWER = 'A';
	public static final byte SCAN = 'S';
	public static final int leftSonar = 0;
	public static final int frontSonar = 1;
	public static final int rightSonar = 2;
	public static final int light = 3;
	public int sensorsNumber;
	
	ArrayList<Object> sensors;
	private BluetoothInterface bi;
	
	public RobotBTI(BluetoothInterface bi, int sensorsNumber) {
		// TODO Auto-generated constructor stub
		this.sensorsNumber = sensorsNumber;
		sensors = new ArrayList<Object>();
		this.bi = bi;
	}
	
	public boolean turnLeft(){
		byte bytes[] = {LEFT};
		bi.write(bytes);
		
		return true;
	}
	
	public boolean turnRight(){
		byte bytes[] = {RIGHT};
		bi.write(bytes);
		return true;
	}
	
	public boolean foward(int distance){
		byte bytes[] = new byte[distance];
		int i;
		
		for (i = 0; i < bytes.length ; i++) {
			bytes[i] = FOWARD;
		}
		
		bi.write(bytes);
		
		return true;
	}
	
	public boolean back(int distance){
		byte bytes[]  = new byte[distance];
		int i;
		
		for (i = 0; i < bytes.length; i++) {
			bytes[i] = BACK;
		}
		
		bi.write(bytes);
		
		return true;
	}
	
	public boolean move(byte direction, int distance) throws InterruptedException{
		switch (direction) {
		case FOWARD:
			foward(distance);
			break;
		case BACK:
			back(distance);
			break;
		case LEFT:
			turnLeft();
			break;
		case RIGHT:
			turnRight();
			break;
		default:
			break;
		}
		/*synchronized (bi.answer) {
			bi.answer.wait();
		}*/
		
		return true;
	}

	public ArrayList<Object> getSensors() {
		// TODO Auto-generated method stub
		return sensors;
	}

	public void scan(String answer) throws InterruptedException {
		// TODO Auto-generated method stub
		
		
		int j = 0;
		Log.i("A2R Bluetooth Answer", answer);
		for (String s: answer.split(" ")) {
			Integer sensor = null;
			try{
				sensor = Integer.valueOf(s);
				Log.i("A2R Robot" + j, sensor + "");
			} catch (Exception e) {
				// TODO: handle exception
			}
			
			try{
				sensors.set(j, sensor);
			} catch (Exception e) {
				sensors.add(sensor);
				// TODO: handle exception
			}
			j++;
		}
		
		/*Log.i("A2R Bluetooth", "Left" + (Integer)sensors.get(0));
		Log.i("A2R Bluetooth", "Front" + (Integer)sensors.get(1));
		Log.i("A2R Bluetooth", "Right" + (Integer)sensors.get(2));
		Log.i("A2R Bluetooth", "LDR" + (Integer)sensors.get(3));*/
	}
}
