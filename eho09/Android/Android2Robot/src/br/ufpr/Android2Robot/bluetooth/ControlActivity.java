package br.ufpr.Android2Robot.bluetooth;

import java.util.ArrayList;
import br.ufpr.Android2Robot.R;
import android.os.Bundle;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.util.Log;
import android.view.*;
import android.view.View.OnTouchListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.ScrollView;
import android.widget.TextView;
import android.bluetooth.*;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.Configuration;

@SuppressLint({ "NewApi", "ShowToast" })
public class ControlActivity extends Activity {
	// Register the BroadcastReceiver
	BluetoothInterface bi = new BluetoothInterface(this, null);
	private ArrayList<String> devices = new ArrayList<String>();
	ArrayAdapter<String> arrayAdapter;
	private BroadcastReceiver mReceiver;
    int deviceIndex;
    
    Button fowardButton;
	Button leftButton;
	Button rightButton;
	Button backButton;
	Button stopButton;
	Button conButton;
	Button refreshButton;
	ScrollView devicesView;
	ListView lv;
	TextView status;
	Button nextButton;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mReceiver = new BroadcastReceiver() {
        	@Override
        	public void onReceive(Context context, Intent intent) {
                String action = intent.getAction();
                // When discovery finds a device
                if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                    // Get the BluetoothDevice object from the Intent
                    BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                    // Add the name and address to an array adapter to show in a ListView
                    arrayAdapter.add(device.getName() + ":" + device.getAddress());
                } else if (BluetoothAdapter.ACTION_DISCOVERY_STARTED.equals(action)) {
                	
                } else if (BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action)) {
                	
                } else if (BluetoothAdapter.ACTION_STATE_CHANGED.equals(action)) {
                	if (!bi.getmBluetoothAdapter().enable()) {
                		Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                        startActivityForResult(enableBtIntent, 1);
					}
                	
                }
            }
        };
        IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        registerReceiver(mReceiver, filter); // Don't forget to unregister during onDestroy
        
        /*filter = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_STARTED);
        registerReceiver(mReceiver, filter);
        
        filter = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        registerReceiver(mReceiver, filter);
        
        filter = new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED);
        registerReceiver(mReceiver, filter);*/
        
        arrayAdapter =  new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1);
        
        
        setContentView(R.layout.activity_control);
        init();
   }

    public void init() {
    	if(true){
    		Intent discoverableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
    		discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 0);
    		startActivity(discoverableIntent);
    	}
		fowardButton = (Button) findViewById(R.id.FowardButton);
		leftButton = (Button) findViewById(R.id.LeftButton);
		rightButton = (Button) findViewById(R.id.RightButton);
		backButton = (Button) findViewById(R.id.BackButton);
		stopButton = (Button) findViewById(R.id.StopButton);
		conButton = (Button) findViewById(R.id.Con);
		refreshButton = (Button) findViewById(R.id.Refresh);
		devicesView = (ScrollView) findViewById(R.id.scrollDevices);
		lv = (ListView) findViewById(R.id.Devices);    
		status = (TextView) findViewById(R.id.Status);
		nextButton =  (Button) findViewById(R.id.NextButton);
		deviceIndex = 0;
		// TODO Auto-generated method stub
		 // Device does not support Bluetooth
		if (bi.getmBluetoothAdapter() == null) {
		  	// 1. Instantiate an AlertDialog.Builder with its constructor
			AlertDialog.Builder builder = new AlertDialog.Builder(this);
		
			// 2. Chain together various setter methods to set the dialog characteristics
			builder.setMessage("Device does not support Bluetooth")
			       .setTitle("Warning!!!")
			       .setCancelable(false)
			       .setPositiveButton("OK", new DialogInterface.OnClickListener() {
			           public void onClick(DialogInterface dialog, int id) {
			        	   
			           }
			       });
		
			// 3. Get the AlertDialog from create()
		    	AlertDialog dialog = builder.create();
		    	
		    	dialog.show();
		    	
		    } else if (!bi.getmBluetoothAdapter().isEnabled()) {
		    	Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
		        startActivityForResult(enableBtIntent, 1);
		    }  
		   
		/*fowardButton.setOnClickListener(new View.OnClickListener() {
    		public void onClick (View v) {
    			byte bytes[] = {'1'};
    			
    			bi.write(bytes);
    		}
		});
		
		fowardButton.setOnLongClickListener(new View.OnLongClickListener() {
    		@Override
			public boolean onLongClick(View v) {
				// TODO Auto-generated method stub
    			byte bytes[] = {'1'};
    			bi.write(bytes);
				return false;
			}
		});*/
		
		fowardButton.setOnTouchListener(new OnTouchListener() {
			
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub
				
				byte bytes[] = {'8', '\n'};
    			
    			bi.write(bytes);
				return false;
			}
			
		});
		
		leftButton.setOnClickListener(new View.OnClickListener() {
			public void onClick (View v) {
				byte bytes[] = {'3'};
				bi.write(bytes);
			}
		});
		
		rightButton.setOnClickListener(new View.OnClickListener() {
			public void onClick (View v) {
				byte bytes[] = {'4'};
				bi.write(bytes);
			}
		});
		
		backButton.setOnClickListener(new View.OnClickListener() {
			public void onClick (View v) {
				byte bytes[] = {'2'};
				bi.write(bytes);
			}
		});
		
		stopButton.setOnClickListener(new View.OnClickListener() {
			public void onClick (View v) {
				byte bytes[] = {'0'};
				bi.write(bytes);
			}
		});
		
		conButton.setOnClickListener(new View.OnClickListener() {
			public void onClick (View v) {
				
				if (bi.getConDevice()!=null){
					status.setText("CONNECTING:" + bi.getConDevice().getName());
					bi.startCon();
					
					if(bi.isConnected() == bi.CONNECTED){
						status.setText("CONNECTED:" + bi.getConDevice().getName());
					} else {
						
					}
				} else {
					status.setText("No devices");
				}
			}
		});
		
		nextButton.setOnClickListener(new View.OnClickListener() {
			public void onClick (View v) {
				if( !bi.devices.isEmpty() ){
					status.setText("Connect to :" + bi.devices.get(deviceIndex));
					bi.setConDevice(bi.devices.get(deviceIndex));
					deviceIndex = (deviceIndex + 1) % bi.devices.size();
					Log.i("A2R", bi.devices.size()+"");
					for(BluetoothDevice device : bi.devices){
						Log.i("A2R", " Device: " + device.getName() + " " + device.getAddress());
					}
				}	
			}
		});
		
		refreshButton.setOnClickListener(new View.OnClickListener() {
			public void onClick (View v) {
				bi.startDiscovery();
				devices.clear();
				devices = bi.pairedDevices( );
				arrayAdapter.addAll(devices);
				lv.setAdapter(arrayAdapter);
				//lv.refreshDrawableState();
			}
		});
	}

	@Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
    	// TODO Auto-generated method stub
    	super.onActivityResult(requestCode, resultCode, data);
    	if(resultCode !=  RESULT_OK)
    		finish();
    		
    }
    
    @Override
    protected void onPause() {
    	// TODO Auto-generated method stub
    	super.onPause();
    	//unregisterReceiver(mReceiver); 
    }
    
    @Override
    protected void onDestroy() {
    	// TODO Auto-generated method stub
    	super.onDestroy();
    	unregisterReceiver(mReceiver); 
    }
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.control, menu);
               
        return true;
    }

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);
	}
}
