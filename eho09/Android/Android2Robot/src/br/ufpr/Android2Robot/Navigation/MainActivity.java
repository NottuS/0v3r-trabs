package br.ufpr.Android2Robot.Navigation;

import java.io.IOException;
import br.ufpr.Android2Robot.R;
import br.ufpr.Android2Robot.Map.Map;
import br.ufpr.Android2Robot.Map.Map.Local;
import br.ufpr.Android2Robot.bluetooth.ControlActivity;
import br.ufpr.Android2Robot.labyrinth.LabyrinthActivity;
import br.ufpr.Android2Robot.wifi.WifiInterface;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends Activity {
	Button mapButton;
	Button navButton;
	Button controlButton;
	Button discButton;
	Button test;
	Button finishTest;
	Button createTables;
	Button loadTables;
	Button saveTbButton;
	TextView tv;
	EditText X;
	EditText Y;
	WifiInterface wi;
	BroadcastReceiver wifiReceiver;
	Map map;
	Handler handle = new Handler(){
		@Override
		public void handleMessage(Message msg) {
				// TODO Auto-generated method stub
				super.handleMessage(msg);
				switch (msg.what) {
				case 0:
					tv.setText("Done");
					test.setClickable(true);
					finishTest.setClickable(true);
					break;
				case 1:
					try {
						tv.setText(map.getPos().y + " " + map.getPos().x);
						
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					break;
				default:
					break;
				}
			}
		};
	
	Local startPos;
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main_layout);
		init();
		Log.d("A2R MainActivity", "onCreate()");
	}
	
	public void init(){
		mapButton = (Button) findViewById(R.id.MapButton);
		navButton = (Button) findViewById(R.id.NavigationButton);
		controlButton = (Button) findViewById(R.id.ControlButton);
		discButton = (Button) findViewById(R.id.DiscButton);
		test = (Button) findViewById(R.id.test);
		finishTest = (Button) findViewById(R.id.finishTest);
		loadTables = (Button) findViewById(R.id.LT);
		createTables = (Button) findViewById(R.id.CT);
		saveTbButton = (Button) findViewById(R.id.saveTable);
		wi = new WifiInterface(getApplicationContext());
		tv = (TextView) findViewById(R.id.text);
		X = (EditText) findViewById(R.id.X);
		Y = (EditText) findViewById(R.id.Y);
		Log.i("Main Act A2R", "OK");
		try {
			map = new Map("map.txt", getApplicationContext(), null, wi);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		/*if(wifiReceiver == null)
			wifiReceiver = wi.initializeWiFiListener(map.mHandler);
		registerReceiver(wifiReceiver, new IntentFilter(wi.getWifiManager().SCAN_RESULTS_AVAILABLE_ACTION));
		Log.i("Main Act A2R", "OK2");*/
		/*if (receiver == null)
            receiver = wi.initializeWiFiListener(mHandler);
		registerReceiver(receiver, new IntentFilter(wi.getWifiManager().SCAN_RESULTS_AVAILABLE_ACTION));
		//wi.getWifiManager().startScan();
        
        WifiTest wt = new WifiTest();
		wt.start();*/
		//tv.setText( tv.getText() + result.SSID + " : " + result.level + "\n"  );
		mapButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick (View v) {
    			
    		}
		});
		
		discButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick (View v) {
    			/*Intent i = new Intent(getApplicationContext(), NavigationService.class);
    			i.putExtra("discovey", true);
    
    			try {
    				int x, y, destX, destY;
					x = Integer.parseInt(X.getText().toString());
					y = Integer.parseInt(Y.getText().toString());
					destX = 12;
					destY = 40;
	    			i.putExtra("x", x);
	    			i.putExtra("y", y);
	    			i.putExtra("DestX", destX);
	    			i.putExtra("DestY", destY);
	    			Bundle value = new Bundle();
	    			
	    			i.putExtra("Handle", value);
	    			startService(i);
				} catch (Exception e) {
					AlertDialog.Builder builder = new AlertDialog.Builder(getApplicationContext());
					builder.setMessage("No X or Y typed");
				}*/

    		}
		});
		
		navButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick (View v) {
    			Intent i = new Intent(getApplicationContext(), LabyrinthActivity.class);
    			startActivity(i);
    			/*Intent i = new Intent(getApplicationContext(), NavigationService.class);
    			i.putExtra("discovery", false);
    			
    			try {
    				int x, y;
					x = Integer.parseInt(X.getText().toString());
					y = Integer.parseInt(Y.getText().toString());
	    			i.putExtra("x", x);
	    			i.putExtra("y", y);
	    			startService(i);
				} catch (Exception e) {
					AlertDialog.Builder builder = new AlertDialog.Builder(getApplicationContext());
					builder.setMessage("No X or Y typed");
				}*/
    		}
		});
		
		controlButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick (View v) {
    			Intent i = new Intent(getApplicationContext(), ControlActivity.class);
    			startActivity(i);
    		}
		});
		
		/*saveTbButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick (View v) {
    			Thread t = new Thread(new Runnable() {
					@Override
					public void run() {
						try {
							map.saveTable();
						} catch (Exception e) {

						}
					}
				});
				t.start();
    		}
		});
		
		test.setOnClickListener(new View.OnClickListener() {
    		public void onClick (View v) {
    			test.setClickable(false);
    			tv.setText("checking position");
    			Log.i("Main Act A2R", "OK3");
    			finishTest.setClickable(false);
				Thread t = new Thread(new Runnable() {
					@Override
					public void run() {
						try {
							int x, y;
							x = Integer.parseInt(X.getText().toString());
							y = Integer.parseInt(Y.getText().toString());
							map.updatePos(y, x);
							map.checkCell();
						} catch (Exception e) {
							AlertDialog.Builder builder = new AlertDialog.Builder(getApplicationContext());
							builder.setMessage("No X or Y typed");
						}
						handle.obtainMessage(0).sendToTarget();
					}
				});
				t.start();
    		}
		});
		
		finishTest.setOnClickListener(new View.OnClickListener() {
    		public void onClick (View v) {
    			tv.setText("checking position");
    			Log.i("Main Act A2R", "OK3");

				Thread t = new Thread(new Runnable() {
					@Override
					public void run() {
						try {
							map.setPos();
							Log.i("Main Act A2R", "OK6");
							map.getPos();
							handle.obtainMessage(1).sendToTarget();
						} catch (InterruptedException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
					}
				});
				t.start();
    		}
		});
		
		createTables.setOnClickListener(new View.OnClickListener() {
    		public void onClick (View v) {
    			tv.setText("creating Tables");
    			Thread t = new Thread(new Runnable() {
					@Override
					public void run() {
						try {
							map.createTables();
						} catch (IOException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
					}
				});
				t.start();
    		}
		});
		
		loadTables.setOnClickListener(new View.OnClickListener() {
    		public void onClick (View v) {
    			tv.setText("loading Tables");
    			Thread t = new Thread(new Runnable() {
					@Override
					public void run() {
						try {
							map.loadTables();
						} catch (IOException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
					}
				});
				t.start();
    		}
		});*/
	}
	
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
	}
	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		if(wifiReceiver == null && wi != null)
			wifiReceiver = wi.initializeWiFiListener(map.mHandler);
		registerReceiver(wifiReceiver, new IntentFilter(wi.getWifiManager().SCAN_RESULTS_AVAILABLE_ACTION));
		Log.d("A2R MainActivity", "onResume()");
	}
	@Override
	protected void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
		//unregisterReceiver(wifiReceiver);
		Log.d("A2R MainActivity", "onStop()");
	}
	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		Log.d("A2R MainActivity", "onPause()");
		if(wifiReceiver != null)
			unregisterReceiver(wifiReceiver);
	}
	
	
	
}
