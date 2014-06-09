package br.ufpr.Android2Robot.Navigation;

import java.io.IOException;
import java.util.Currency;

import br.ufpr.Android2Robot.R;
import br.ufpr.Android2Robot.Map.Map;
import br.ufpr.Android2Robot.Map.Map.CellTable;
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
import android.net.wifi.ScanResult;
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
	TextView statusTextView;
	TextView logTextView;
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
					statusTextView.setText("Status: Done");
					scanButton.setClickable(true);
					getPosButton.setClickable(true);
					saveTbButton.setClickable(true);
					String log = "Current Cell info:\n"; 
					if(map.getCurrentCell() != null){
						log +="X:" + map.getCurrentCell().pos.x + " Y:" 
									+ map.getCurrentCell().pos.y + "\n";
						for(CellTable ct: map.getCurrentCell().apTable){
							log += ct.ap.name + " " + ct.meanRSS + "\n"; 
						}
						logTextView.setText(logTextView.getText().toString() + "\n" +log);
					}
					break;
				case 1:
					try {
						statusTextView.setText("Status: Atual Pos:" 
								+ map.getPos().y + " " + map.getPos().x);
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
		statusTextView = (TextView) findViewById(R.id.status);
		logTextView = (TextView) findViewById(R.id.log);
		
		wi = new WifiInterface(getApplicationContext());
		Log.i("Main Act A2R", "OK");
		try {
			map = new Map("map.txt", getApplicationContext(), null, wi);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		if(!wi.getWifiManager().isWifiEnabled())
			wi.getWifiManager().setWifiEnabled(true);
		if(wifiReceiver == null){
			wifiReceiver = wi.initializeWiFiListener(map.mHandler);
			registerReceiver(wifiReceiver, 
					new IntentFilter(wi.getWifiManager().SCAN_RESULTS_AVAILABLE_ACTION));
		}
		Log.i("Main Act A2R", "OK2 " + wi.getWifiManager().isWifiEnabled());
		
		saveTbButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick (View v) {
    			scanButton.setClickable(false);
    			getPosButton.setClickable(false);
    			saveTbButton.setClickable(false);
    			Thread t = new Thread(new Runnable() {
					@Override
					public void run() {
						try {
							Log.i("Main Act A2R", "Map ok");
							map.saveTable();
							Log.i("Main Act A2R", "Map ok1");
						} catch (Exception e) {
							Log.i("Main Act A2R", "Map Not ok " + e.getMessage());
						} finally {
							scanButton.setClickable(true);
							getPosButton.setClickable(true);
							saveTbButton.setClickable(true);
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
    			try {
					Integer.parseInt(XeditText.getText().toString());
					Integer.parseInt(YeditText.getText().toString());
				} catch (Exception e) {
					AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
					builder.setMessage("No X or Y typed");
					//builder.create();
					builder.show();
					return;
				}
    			final int  x = Integer.parseInt(XeditText.getText().toString());
				final int  y = Integer.parseInt(YeditText.getText().toString());
    			scanButton.setClickable(false);
    			getPosButton.setClickable(false);
    			saveTbButton.setClickable(false);
    			statusTextView.setText("Status: Get APs for:" + XeditText.getText().toString() 
    						+ " " + YeditText.getText().toString());
    			Log.i("Main Act A2R", "OK3");
				Thread t = new Thread(new Runnable() {
					@Override
					public void run() {
						try {
							Log.i("A2R",x + " " + y);
							map.updatePos(y, x);
							map.checkCell();
						} catch (Exception e) {

						} finally {
							handle.obtainMessage(0).sendToTarget();
						}
					}
				});
				t.start();
    		}
		});
		
		getPosButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick (View v) {
    			statusTextView.setText("Status: checking position");
    			Log.i("Main Act A2R", "OK3");
    			scanButton.setClickable(false);
    			getPosButton.setClickable(false);
    			saveTbButton.setClickable(false);
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
						} finally {
							handle.obtainMessage(0).sendToTarget();
						}
					}
				});
				t.start();
    		}
		});
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
		registerReceiver(wifiReceiver, new IntentFilter(wi.getWifiManager()
				.SCAN_RESULTS_AVAILABLE_ACTION));
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
}
