package br.ufpr.Android2Robot.bluetooth;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.Set;
import java.util.UUID;

import android.annotation.SuppressLint;
import android.bluetooth.*;
import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;

@SuppressLint({ "NewApi", "ShowToast" })
public class BluetoothInterface {
	 
	private static final int SUCCESS = 0;
	private static final int FAIL = 1;
	public static final int ANSWER = 2;
	public static final int DISCONNECTED = 0;
	public static final int CONNECTED = 1;
	public static final int CONNECTING = 2;
	
	private BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
	private UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
	private AcceptThread at;
	private ConnectThread ct;
	private ConnectedThread ctedt;
	private BluetoothDevice conDevice;
	private int isConnected;
	private Context context;
	public Handler mHandler;
	public Handler aHandler;
	public ArrayList<BluetoothDevice> devices;
	String answer;
	
	public int isConnected() {
		return isConnected;
	}
	public void setConnected(int isConnected) {
		this.isConnected = isConnected;
	}
	public UUID getMY_UUID() {
		return MY_UUID;
	}
	public void setMY_UUID(UUID mY_UUID) {
		MY_UUID = mY_UUID;
	}
	public AcceptThread getAt() {
		return at;
	}
	public void setAt(AcceptThread at) {
		this.at = at;
	}
	public ConnectThread getCt() {
		return ct;
	}
	public void setCt(ConnectThread ct) {
		this.ct = ct;
	}
	ConnectedThread getCtedt() {
		return ctedt;
	}
	public void setCtedt(ConnectedThread ctedt) {
		this.ctedt = ctedt;
	}
	public BluetoothDevice getConDevice(){
		return conDevice;
	}
	public void setConDevice(BluetoothDevice conDevice) {
		this.conDevice = conDevice;
	}
	public BluetoothInterface(final Context context, Handler aHandler) {
		// TODO Auto-generated constructor stub
		this.context = context;
		isConnected = DISCONNECTED;
		conDevice = null;
		devices = new ArrayList<BluetoothDevice>();
		MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
		answer = "";
		this.aHandler = aHandler;  
		mHandler = new Handler(){
			@Override
			public void handleMessage(Message msg) {
				// TODO Auto-generated method stub
				super.handleMessage(msg);
				switch (msg.what) {
				case SUCCESS:
					Toast.makeText(context, "Connected " + conDevice.getName(), 2000).show();
					ctedt = new ConnectedThread((BluetoothSocket) msg.obj);
					ctedt.start();
					break;
				case FAIL:
					Toast.makeText(context, (String) msg.obj, 2000).show();
					Log.e("BI", (String) msg.obj );
					break;
				case ANSWER:
					answer = (String)msg.obj;
					//Toast.makeText(context, answer, Toast.LENGTH_LONG).show();
										
				default:
					break;
				}
			}
		};
	}
	public BluetoothAdapter getmBluetoothAdapter() {
		return mBluetoothAdapter;
	}
	public void setmBluetoothAdapter(BluetoothAdapter mBluetoothAdapter) {
		this.mBluetoothAdapter = mBluetoothAdapter;
	}
	
	public void startCon(){
		/*at = new AcceptThread();
		at.run();*/
		setConnected(CONNECTING);
		ct = new ConnectThread(conDevice);
		ct.start();
		at = new AcceptThread();
		at.start();
  	}
	
	public void disconect(){
		if(ctedt != null)
			ctedt.cancel();
	}
	
	public void startDiscovery(){
		mBluetoothAdapter.cancelDiscovery();
	    mBluetoothAdapter.startDiscovery();
	}
	
	public ArrayList<String> pairedDevices( ){
		Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
		ArrayList<String> devices = new ArrayList<String>();
		// If there are paired devices
		if (pairedDevices.size() > 0) {
		    // Loop through paired devices
		    for (BluetoothDevice device : pairedDevices) {
		        // Add the name and address to an array adapter to show in a ListView
		    	devices.add(device.getName() + "\n" + device.getAddress());
		    	Log.i("A2R BI", " Device: " + device.getName() + " " + device.getAddress());
		    	this.devices.add(device);
		    }
		}
		
		return devices;
	}
	
	private class AcceptThread extends Thread {
	    private final BluetoothServerSocket mmServerSocket;
	 
	    public AcceptThread() {
	        // Use a temporary object that is later assigned to mmServerSocket,
	        // because mmServerSocket is final
	        BluetoothServerSocket tmp = null;
	        try {
	            // MY_UUID is the app's UUID string, also used by the client code
	            tmp = mBluetoothAdapter.listenUsingRfcommWithServiceRecord(getName(), MY_UUID);
	        } catch (IOException e) { }
	        mmServerSocket = tmp;
	    }
	 
	    public BluetoothServerSocket getMmServerSocket() {
			return mmServerSocket;
		}

		public void run() {
	        BluetoothSocket socket = null;
	        // Keep listening until exception occurs or a socket is returned
	        while (true) {
	            try {
	                socket = mmServerSocket.accept();
	            } catch (IOException e) {
	                break;
	            }
	            // If a connection was accepted
	            if (socket != null) {
	                // Do work to manage the connection (in a separate thread)
	                manageConnectedSocket(socket);
	                try {
						mmServerSocket.close();
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
	                break;
	            }
	        }
	    }
	 
	    private void manageConnectedSocket(BluetoothSocket socket) {
	    	
			
		}

		/** Will cancel the listening socket, and cause the thread to finish */
	    public void cancel() {
	    	setConnected(DISCONNECTED);
	        try {
	            mmServerSocket.close();
	        } catch (IOException e) { }
	    }
	}
	
	private class ConnectThread extends Thread {
	   
		private final BluetoothSocket mmSocket;
	    private final BluetoothDevice mmDevice;
	 
	    public BluetoothSocket getMmSocket() {
			return mmSocket;
		}

		public BluetoothDevice getMmDevice() {
			return mmDevice;
		}

		public ConnectThread(BluetoothDevice device) {
	        // Use a temporary object that is later assigned to mmSocket,
	        // because mmSocket is final
	        BluetoothSocket tmp = null;
	        mmDevice = device;
	 
	        // Get a BluetoothSocket to connect with the given BluetoothDevice
	        try {
	            // MY_UUID is the app's UUID string, also used by the server code
	            tmp = device.createRfcommSocketToServiceRecord(MY_UUID);
	        } catch (IOException e) { 
	        	mHandler.obtainMessage(FAIL, "ct" + e.getMessage()).sendToTarget();
	        	setConnected(DISCONNECTED);
	        }
	        mmSocket = tmp;
	    }
	 
	    public void run() {
	        // Cancel discovery because it will slow down the connection
	        mBluetoothAdapter.cancelDiscovery();
	 
	        try {
	            // Connect the device through the socket. This will block
	            // until it succeeds or throws an exception
	            mmSocket.connect();
	        } catch (IOException connectException) {
	        	mHandler.obtainMessage(FAIL, "ct.run: "+ mmDevice.getName() + " "  + connectException.getMessage()).sendToTarget();
	            // Unable to connect; close the socket and get out
	            try {
	                mmSocket.close();
	            } catch (IOException closeException) { 
	            	mHandler.obtainMessage(FAIL, closeException.getMessage()).sendToTarget();
	            	setConnected(DISCONNECTED);
	            }
	            
	            return;
	        }
	        mHandler.obtainMessage(SUCCESS, mmSocket).sendToTarget();
	        // Do work to manage the connection (in a separate thread)
	        manageConnectedSocket(mmSocket);
	    }
	 
	    private void manageConnectedSocket(BluetoothSocket mmSocket2) {
			// TODO Auto-generated method stub
		}

		/** Will cancel an in-progress connection, and close the socket */
	    public void cancel() {
	        try {
	            mmSocket.close();
	        } catch (IOException e) { }
	    }
	}

	private class ConnectedThread extends Thread {
	    private final BluetoothSocket mmSocket;
	    private final InputStream mmInStream;
	    private final OutputStream mmOutStream;
	 
		public ConnectedThread(BluetoothSocket socket) {
	        mmSocket = socket;
	        InputStream tmpIn = null;
	        OutputStream tmpOut = null;
	 
	        // Get the input and output streams, using temp objects because
	        // member streams are final
	        try {
	            tmpIn = socket.getInputStream();
	            tmpOut = socket.getOutputStream();
	        } catch (IOException e) { mHandler.obtainMessage(FAIL,"cted"+ e.getMessage()).sendToTarget(); }
	 
	        mmInStream = tmpIn;
	        mmOutStream = tmpOut;
	    }
	 
	    public void run() {
	        String info = "";
	        setConnected(CONNECTED);
	        // Keep listening to the InputStream until an exception occurs
	        //Scanner s = new Scanner(mmInStream,"UTF-8").useDelimiter("\\A");
	        int i = 0;
	        while (true) {
	            // Read from the InputStream
	        	byte buffer[] = new byte[2048];
				try {
					InputStreamReader dinput = new InputStreamReader(mmInStream);
					BufferedReader bufferedReader = new BufferedReader( dinput );
					info = bufferedReader.readLine();
					//int bytes = mmInStream.read(buffer);
					/*while(buffer[i] != '\n' && i < 1023)
						info += buffer[i++];
					// Send the obtained bytes to the UI activity
					if(buffer[i] == '\n'){*/
						aHandler.obtainMessage(1, info )
					        .sendToTarget();
						info ="";
					//}
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
	        	/*s.reset();
	        	
				if(s.hasNext()){
					info += s.nextInt() + " ";
					if(i == 3)
						aHandler.obtainMessage(1, info)
						.sendToTarget();
					i++;
				}*/
				
				
	        }
	    }
	 
	    /* Call this from the main activity to send data to the remote device */
	    public void write(byte[] bytes) {
	        try {
	            mmOutStream.write(bytes);
	            
	        } catch (IOException e) {Log.i("BI", "FUUUUUUUUUUUUUUUUUu"); }
	    }
	 
	    /* Call this from the main activity to shutdown the connection */
	    public void cancel() {
	        try {
	            mmSocket.close();
	        } catch (IOException e) {  }
	    }
	}

	public void write(byte bytes[]) {
		// TODO Auto-generated method stub
		if( isConnected == CONNECTED )
			ctedt.write(bytes);
	}
	
	void cancel () {
		ct.cancel();
	}
	
	
}
