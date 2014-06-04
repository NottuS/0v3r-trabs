package br.ufpr.Android2Robot.wifi;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.wifi.*;
import android.os.Handler;
import android.util.Log;

import java.util.List;

public class WifiInterface {
	
	Context context;
	WifiManager wifiManager;
	WifiInfo wifiInfo;
	//WifiConfiguration wifiConfiguration = wifiManager.;
	
	public WifiInterface(Context context) {
		// TODO Auto-generated constructor stub
		this.context = context;
		wifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
	}
	
	
	public WifiManager getWifiManager() {
		return wifiManager;
	}


	public void setWifiManager(WifiManager wifiManager) {
		this.wifiManager = wifiManager;
	}


	public WifiInfo getWifiInfo() {
		return wifiInfo;
	}


	public void setWifiInfo(WifiInfo wifiInfo) {
		this.wifiInfo = wifiInfo;
	}
	
	
	public BroadcastReceiver initializeWiFiListener(final Handler mHandler){
		BroadcastReceiver wifiReceiver = new BroadcastReceiver(){
			@Override
		    public void onReceive(Context context, Intent intent) 
		    {
				Log.d("A2R wifiReceiver", "onReceive()");
				List<ScanResult> results = wifiManager.getScanResults();
			    //ScanResult bestSignal = null;
			    mHandler.obtainMessage(0, results).sendToTarget();
			    /*for (ScanResult result : results) {
			    	
			    }*/
			    
		    }   
		};
		
		return wifiReceiver;
	}
}
