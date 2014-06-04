package br.ufpr.Android2Robot.Navigation;

import java.io.IOException;

import org.apache.commons.math3.linear.RealMatrix;

import br.ufpr.Android2Robot.Map.Map;
import br.ufpr.Android2Robot.Map.Map.Local;
import br.ufpr.Android2Robot.bluetooth.BluetoothInterface;
import br.ufpr.Android2Robot.bluetooth.RobotBTI;
import br.ufpr.Android2Robot.wifi.WifiInterface;
import android.app.IntentService;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.util.Log;

public class NavigationService extends IntentService {
	
	WifiInterface wi;
	BluetoothInterface bi;
	BroadcastReceiver wifiReceiver;
	RobotBTI robot;
	Localization localization;
	Local dest;
	Map map;
	int distance = 8;
	
	public NavigationService(String name) {
		super(name);
		// TODO Auto-generated constructor stub
	}

	/** ONCREATE */
	@Override
	public void onCreate() {
		// TODO Auto-generated method stub
		super.onCreate();
		wi = new WifiInterface(getApplicationContext());
		bi = new BluetoothInterface(getApplicationContext(), null);
		bi.setConDevice(bi.getmBluetoothAdapter().getRemoteDevice("20:13:01:30:08:13"));
		robot = new RobotBTI(bi,1);
		
		
		//AssetManager am = getAssets();
		
		
		/*AssetFileDescriptor afd;
		try {
			afd = am.openFd("map.txt");
			OutputStream fo = afd.createOutputStream();
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}*/
		Log.d("A2R NavigationService", "onCreate()");
	}
	
	
	/**ONHANDLE*/
	@Override
	protected void onHandleIntent(Intent arg0) {
		// TODO Auto-generated method stub
		Local startPos = null;
		if(arg0.getBooleanExtra("discovery", false))
			startPos = new Local(arg0.getIntExtra("x", 0), arg0.getIntExtra("y", 0));
		try {
			map = new Map("map.txt", getApplicationContext(), startPos, wi);
			if(wifiReceiver == null)
				wifiReceiver = wi.initializeWiFiListener(map.mHandler);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}	
		try {
			map.getPos();
			map.calculatePath(null);
			if(bi.isConnected() == BluetoothInterface.CONNECTED){
				try {
					while( !reachLocation() ){
						
						moveRobot(distance);
					}
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			
			e.printStackTrace();
		}
		
		Log.d("A2R NavigationService", "onHandleIntent()");
	}
	
	private void moveRobot(int distance) throws InterruptedException {
		// TODO Auto-generated method stub
		RealMatrix m;
		byte direction = map.getPath();
		robot.move(direction, distance);
		robot.wait();
		switch (direction) {
		case Map.FOWARD:
			map.updatePos(map.getPos().y+1, map.getPos().x);
			break;
		case Map.BACK:
			map.updatePos(map.getPos().y-1,map.getPos().x);
			break;
		case Map.LEFT:
			map.updatePos(map.getPos().y, map.getPos().x-1);
			break;
		case Map.RIGHT:
			map.updatePos(map.getPos().y, map.getPos().x+1);
			break;
		default:
			break;
		}
		Log.d("A2R NavigationService", "moveRobot()");
	}

	private boolean reachLocation() throws InterruptedException {
		// TODO Auto-generated method stub
		if (map.getPos().x == dest.x && map.getPos().y == dest.y)
			return true;
		return false;
	}
	
	@Override
	public void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		unregisterReceiver(wifiReceiver);
		Log.d("A2R NavigationService", "onDestroy()");
	}
}
