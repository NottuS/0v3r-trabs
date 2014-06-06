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
	Button scanButton;
	Button getPosButton;
	Button createTables;
	Button loadTables;
	Button saveTbButton;
	Button addXButton;
	Button addYButton;
	Button subXButton;
	Button subYButton;
	TextView tv;
	EditText XeditText;
	EditText YeditText;
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
					scanButton.setClickable(true);
					getPosButton.setClickable(true);
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
		/*mapButton = (Button) findViewById(R.id.MapButton);
		navButton = (Button) findViewById(R.id.NavigationButton);
		controlButton = (Button) findViewById(R.id.ControlButton);
		discButton = (Button) findViewById(R.id.DiscButton);
		loadTables = (Button) findViewById(R.id.LT);
		createTables = (Button) findViewById(R.id.CT);*/
		scanButton = (Button) findViewById(R.id.scan);
		getPosButton = (Button) findViewById(R.id.getPosition);
		addXButton = (Button) findViewById(R.id.addX);
		addYButton = (Button) findViewById(R.id.addY);
		subXButton = (Button) findViewById(R.id.subX);
		subYButton = (Button) findViewById(R.id.subY);
		saveTbButton = (Button) findViewById(R.id.saveTable);
		XeditText = (EditText) findViewById(R.id.X);
		YeditText = (EditText) findViewById(R.id.Y);
		
		wi = new WifiInterface(getApplicationContext());
		tv = (TextView) findViewById(R.id.text);

		Log.i("Main Act A2R", "OK");
		try {
			map = new Map("map.txt", getApplicationContext(), null, wi);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		if(wifiReceiver == null && wi.getWifiManager().isWifiEnabled()){
			wifiReceiver = wi.initializeWiFiListener(map.mHandler);
			registerReceiver(wifiReceiver, 
					new IntentFilter(wi.getWifiManager().SCAN_RESULTS_AVAILABLE_ACTION));
		}
		Log.i("Main Act A2R", "OK2 " + wi.getWifiManager().isWifiEnabled());
		/*if (receiver == null)
            receiver = wi.initializeWiFiListener(mHandler);
		registerReceiver(receiver, new IntentFilter(wi.getWifiManager().SCAN_RESULTS_AVAILABLE_ACTION));
		//wi.getWifiManager().startScan();
        
        WifiTest wt = new WifiTest();
		wt.start();*/
		//tv.setText( tv.getText() + result.SSID + " : " + result.level + "\n"  );
		/*mapButton.setOnClickListener(new View.OnClickListener() {
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
				}

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
				}
    		}
		});
		
		controlButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick (View v) {
    			Intent i = new Intent(getApplicationContext(), ControlActivity.class);
    			startActivity(i);
    		}
		});*/
		
		saveTbButton.setOnClickListener(new View.OnClickListener() {
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
		
		addXButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick (View v) {
    			try {
					int x = Integer.parseInt(XeditText.getText().toString());
					XeditText.setText(((x + 1) % map.getWidth()) + "");
				} catch (Exception e) {
					// TODO: handle exception
					XeditText.setText(0 + "");
				}
    		}
		});
		
		addYButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick (View v) {
    			try {
					int y = Integer.parseInt(YeditText.getText().toString());
					YeditText.setText(((y + 1) % map.getLength()) + "");
				} catch (Exception e) {
					// TODO: handle exception
					YeditText.setText(0 + "");
				}
    		}
		});
		
		subXButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick (View v) {
    			try {
					int x = Integer.parseInt(XeditText.getText().toString());
					XeditText.setText((x - 1 >= 0 ? x - 1 : map.getWidth() - 1) + "");
				} catch (Exception e) {
					// TODO: handle exception
					XeditText.setText((map.getWidth() - 1) + "");
				}
    		}
		});
		
		subYButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick (View v) {
    			try {
					int y = Integer.parseInt(YeditText.getText().toString());
					YeditText.setText((y - 1 >= 0 ? y - 1 : map.getLength() - 1) + "");
				} catch (Exception e) {
					// TODO: handle exception
					YeditText.setText((map.getLength() - 1) + "");
				}
    		}
		});
		scanButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick (View v) {
    			scanButton.setClickable(false);
    			tv.setText("checking position");
    			Log.i("Main Act A2R", "OK3");
    			getPosButton.setClickable(false);
				Thread t = new Thread(new Runnable() {
					@Override
					public void run() {
						try {
							int x, y;
							x = Integer.parseInt(XeditText.getText().toString());
							y = Integer.parseInt(YeditText.getText().toString());
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
		
		getPosButton.setOnClickListener(new View.OnClickListener() {
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
		
		/*createTables.setOnClickListener(new View.OnClickListener() {
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
		if(wifiReceiver == null && wi != null && wi.getWifiManager().isWifiEnabled())
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
		if(wifiReceiver != null)
			unregisterReceiver(wifiReceiver);
		Log.d("A2R MainActivity", "onPause()");
	}
}
