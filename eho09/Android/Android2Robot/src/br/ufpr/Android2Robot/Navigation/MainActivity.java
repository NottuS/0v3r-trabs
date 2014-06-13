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
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
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
import android.widget.Toast;

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
	ProgressDialog dialog;
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
					String log = "Current Cell info:\n"; 
					if(map.getCurrentCell() != null){
						log +="X:" + map.getCurrentCell().pos.x + " Y:" 
									+ map.getCurrentCell().pos.y + "\n";
						Log.i("FUUUUUUUUUUUUUU", "TOOOOOOOOOOONN");
						for(CellTable ct: map.getCurrentCell().apTable){
							log += ct.ap.name + " " + ct.meanRSS + "\n"; 
						}
						Log.i("FUUUUUUUUUUUUUU", "T111111111111NN");
						logTextView.setText(/*logTextView.getText().toString() +*/ "\n" +log);
					}
					break;
				case 1:
					break;
				case 2:
					AlertDialog.Builder builder = new AlertDialog
							.Builder(MainActivity.this);
					builder.setMessage((String) msg.obj);
					builder.show();
					break;
				case 3:
					dialog = ProgressDialog.show(MainActivity.this,
							"",(String) msg.obj, true);
					break;
				case 4:
					dialog.setMessage((String) msg.obj);
					break;
				case 5:
					dialog.dismiss();
					break;
				case 6:
					Toast.makeText(MainActivity.this, (String) msg.obj, Toast.LENGTH_LONG).show();
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
	
	@SuppressLint("ShowToast")
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
    			handle.obtainMessage(3, "Saving table. Please wait...").sendToTarget();
    			Thread t = new Thread(new Runnable() {
					@Override
					public void run() {
						String message = "";
						try {
							Log.i("Main Act A2R", "Map ok");
							map.saveTable();
							Log.i("Main Act A2R", "Map ok1");
							message = "Table Saved.";
							//handle.obtainMessage(2, "Table Saved.").sendToTarget();
						} catch (Exception e) {
							message = "Error trying to save the table!";
							
							Log.i("Main Act A2R", "Map Not ok " + e.getMessage());
						} finally {
							Log.i("Main Act A2R", "Map ok1 finally " + message);
							handle.obtainMessage(5).sendToTarget();
							handle.obtainMessage(6, message).sendToTarget();
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
    			wi.getWifiManager().setWifiEnabled(true);
    			try {
					Integer.parseInt(XeditText.getText().toString());
					Integer.parseInt(YeditText.getText().toString());
				} catch (Exception e) {
					AlertDialog.Builder builder = new AlertDialog
							.Builder(MainActivity.this);
					builder.setMessage("Invalid X or Y typed.");
					builder.show();
					return;
				}
    			final int  x = Integer.parseInt(XeditText
    					.getText().toString());
				final int  y = Integer.parseInt(YeditText
						.getText().toString());
				handle.obtainMessage(3, "Getting samples. Please wait...").sendToTarget();
    			Log.i("Main Act A2R", "OK3");
				Thread t = new Thread(new Runnable() {
					@Override
					public void run() {
						String message = "";
						try {
							Log.i("A2R",x + " " + y);
							map.updatePos(y, x);
							map.checkCell(handle);
						} catch (Exception e) {
							message = "Error trying to get the samples!";
							handle.obtainMessage(2, message)
								.sendToTarget();
						} finally {
							handle.obtainMessage(0).sendToTarget();
							handle.obtainMessage(5).sendToTarget();
						}
					}
				});
				t.start();
    		}
		});
		
		getPosButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick (View v) {
    			Log.i("Main Act A2R", "OK3");
    			wi.getWifiManager().setWifiEnabled(true);
    			handle.obtainMessage(3, "Getting your current position. Please wait...").sendToTarget();
				Thread t = new Thread(new Runnable() {
					@Override
					public void run() {
						String message = "";
						try {
							map.setPos();
							Log.i("Main Act A2R", "OK6");
							map.getPos();
							message = "Current Position:" 
							 + "(" + map.getPos().x + "," +map.getPos().y + ")";
							
						} catch (Exception e) {
							// TODO Auto-generated catch block
							message = "Error trying to get the current position!";
							Log.e("Main Act A2R", "get Pos error: " + e.getMessage());
						} finally {
							handle.obtainMessage(0).sendToTarget();
							Log.i("Main Act A2R", message);
							handle.obtainMessage(5).sendToTarget();
							handle.obtainMessage(2, message).sendToTarget();
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
		if(wifiReceiver == null && wi != null && 
				wi.getWifiManager().isWifiEnabled())
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
