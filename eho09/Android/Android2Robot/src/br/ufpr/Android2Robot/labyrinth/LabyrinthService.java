package br.ufpr.Android2Robot.labyrinth;

import java.util.ArrayList;

import br.ufpr.Android2Robot.bluetooth.BluetoothInterface;
import br.ufpr.Android2Robot.bluetooth.RobotBTI;
import br.ufpr.Android2Robot.labyrinth.LabyrinthActivity.LabyrinthReceiver;
import br.ufpr.Android2Robot.labyrinth.Markov.Pose;
import android.app.IntentService;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.Resources.Theme;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;

public class LabyrinthService extends IntentService {
	public LabyrinthService() {
		super("FUCK");
		// TODO Auto-generated constructor stub
	}

	Labyrinth lab;
	Markov markov;
	Handler mHandler;
	RobotBTI robot;
	BluetoothInterface bi;
	private MyReceiver mReceiver;
	final int INF = 0xffff;
	int next;
	
	@Override
	public void onCreate() {
		// TODO Auto-generated method stub
		lab = new Labyrinth("Labyrinth.txt", this, 0, 4, 90);
		next = 0;
		ArrayList<Pose> belief = new ArrayList<Pose>();
		//belief.add(markov.new Pose(0, 4, 90, 1));
		IntentFilter filter = new IntentFilter("com.lab.service");  
	    mReceiver = new MyReceiver();  
	    registerReceiver(mReceiver, filter);
		markov = new Markov(lab);
		mHandler = new Handler(){
			@Override
			public void handleMessage(Message msg) {
				// TODO Auto-generated method stub
				super.handleMessage(msg);
				switch (msg.what) {
				case 1:
					byte direction = 0;
					Log.i("Labirinto Service", "AQUI");
					String answer = (String)msg.obj;
					try {
						
						robot.scan(answer);
					} catch (InterruptedException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					direction = (byte) ((byte)giveDirection() + '0');
					if(direction != -1){
						try {
							robot.move(direction,1);
							//markov.calcPerception();
							lab.updatePos();
							Log.i("Labirinto Service Update", lab.teta + " " + lab.y + " " + lab.x + " " + direction);
							sendMap();
						} catch (InterruptedException e1) {
							// TODO Auto-generated catch block
							e1.printStackTrace();
						}
					}
					next = 1;
				case 0:
					
					
				default:
					break;
				}
			}
		};
		bi = new BluetoothInterface(getApplicationContext(), mHandler);
		bi.startDiscovery();
		bi.pairedDevices();
		for(BluetoothDevice b: bi.devices)
			if(b.getAddress().contains(/*"30:76:6F:64:F8:2F"*/"20:13:01:30:08:13")){
				bi.setConDevice(b);
				break;
			}
		if(bi.getConDevice() == null)
			try {
				Log.i("Labirinto Service", "No device");
				Toast.makeText(getApplicationContext(), "No device", Toast.LENGTH_LONG);
				
				stopSelf();
			} catch (Throwable e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		else {
			bi.startCon();
			robot = new RobotBTI(bi,4);
		}
		Log.i("Labirinto Service", "onCreate");
	    super.onCreate();
	}
	
	@Override
	public IBinder onBind(Intent arg0) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	protected void onHandleIntent(Intent arg0) {
		// TODO Auto-generated method stub
		
		int i = 0;
		
		//while(bi.isConnected() != bi.CONNECTED);
		
		Log.i("Labirinto Service", "onHI");
		while(bi.isConnected() != bi.CONNECTED);
		do {
			//markov.calcAction(direction);
			Log.i("Labirinto Service", "YYYYYYYYYYYYYYYYY");
			byte bytes[] = {robot.SCAN};
			bi.write(bytes);
			while(next == 0);
			next = 0;
			i++;
			Log.i("Labirinto Service", "onHI1");
			
			
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		} while(i < 2/*!reachEnd()*/);
		Log.i("Labirinto Service", "onHI END");
	}
	
	private int giveDirection() {
		int direction[] = new int[4];
		int teta, menor, i;
		if (robot.getSensors().size() < robot.sensorsNumber)
			return -1;
		for(i = 0; i < 3; i++){
			teta = (lab.teta + (i - 1) * 90) % 360;
			if(teta <= 0)
				teta = 360;
			/*Log.i("Labirinto Service GD",teta + " " + (lab.y - (int)Math.sin(Math.toRadians(teta))) + " " + (lab.x - (int)Math.cos(Math.toRadians(teta))));
			Log.i("Labirinto Service GD",(int)Math.sin(Math.toRadians(teta)) + " " +  (int)Math.cos(Math.toRadians(teta)) + " " + Math.toRadians(teta));*/
			if(!haveWall(i))
				direction[i] = lab.path[(lab.y - (int)Math.sin(Math.toRadians(teta)))][(lab.x - (int)Math.cos(Math.toRadians(teta)))];
			else 
				direction[i] = INF;
		}
		
		direction[i] = INF / 2;
		menor = 0;
		String aux = null;
		for(i = 1; i < 4; i++){
			if(direction[menor] > direction[i])
				menor = i;
		}
		for(i = 0; i < 4; i++)
			aux += direction[i] + " ";
		//Log.i("Labirinto Service Menor", aux);
		lab.teta = (lab.teta + (menor - 1) * 90) % 360;
		if(lab.teta <= 0)
			lab.teta = 360;
		//Log.i("Labirinto Service GD direction", menor + " " + lab.teta);
		return  menor;
	}
	
	public boolean haveWall(int side){
		if((Integer)robot.getSensors().get(side) != null && (Integer)robot.getSensors().get(side) < 20)
			return true;
		side = 1<<side;
		for(int i = 0; i < lab.teta / 90 -1; i++)
			side = (side << 1) % 16 + (side << 1) / 16;
		//Log.i("Labirinto Service hw", side+" "+(lab.map[lab.y][lab.x] & side));
		if((lab.map[lab.y][lab.x] & side) != 0 ) 
			return true;
		return false;
	}
	
	private boolean reachEnd() {
		// TODO Auto-generated method stub
		/*if((Integer)robot.getSensors().get(robot.light) > 470)
			return true;
		if(lab.x == 2 && lab.y == 4)
			return true;*/
		return false;
	}	
	
	public class MyReceiver extends BroadcastReceiver {  

		@Override
		public void onReceive(Context arg0, Intent arg1) {
			// TODO Auto-generated method stub
			bi.disconect();
			try {
				Log.i("Labirinto Service", "Exit");
				stopSelf();
			} catch (Throwable e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}  
    }
	
	public void sendMap(){
		/*Intent intent = new Intent(this, LabyrinthReceiver.class);*/
		Intent intent = new Intent("com.Labyrinth");
		intent.putExtra("lenght", lab.length);
		intent.putExtra("width", lab.width);
		if (robot.getSensors().size() > robot.sensorsNumber){
			intent.putExtra("leftSonar", (Integer)robot.getSensors().get(robot.leftSonar));
			intent.putExtra("frontSonar", (Integer)robot.getSensors().get(robot.frontSonar));
			intent.putExtra("rightSonar", (Integer)robot.getSensors().get(robot.rightSonar));
			intent.putExtra("ldr", (Integer)robot.getSensors().get(robot.light));
		}
		for(int i = 0; i < lab.length; i++)
			intent.putExtra("Path" + i, lab.path[i]);
	    sendBroadcast(intent); 
	}
	
	@Override
	public void onDestroy() {
		// TODO Auto-generated method stub
		
		unregisterReceiver(mReceiver);
		Log.i("Labirinto Service", "onDestroy");
		super.onDestroy();
	}
}
