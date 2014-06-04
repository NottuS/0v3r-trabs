package br.ufpr.Android2Robot.Navigation;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import android.content.Context;
import android.content.res.AssetManager;
import android.net.wifi.ScanResult;
import android.os.Handler;
import android.os.Message;
import android.widget.Toast;
import br.ufpr.Android2Robot.Map.Map;
import br.ufpr.Android2Robot.Map.Map.AP;
import br.ufpr.Android2Robot.Map.Map.Cell;
import br.ufpr.Android2Robot.wifi.WifiInterface;

public class Localization {
	/*private class AP{
		String name;
		Local pos;
		
		public AP(String name, Local local) {
			// TODO Auto-generated constructor stub
			pos = local;
			this.name = name;
		}
	}*/
	
	public class Local{
		public int x,y;
		public Local(int x, int y) {
			// TODO Auto-generated constructor stub
			this.x = x;
			this.y = y; 
		}
	}
	
	class AP{
		String name;
		int x,y;
		int meanRSS;
		
		public AP(String name, int x, int y) {
			// TODO Auto-generated constructor stub
			this.name = name;
			this.x = x;
			this.y = y;
		}
		
	}
	
	public AP getAP(String name){
		for(AP ap: aps){
			if(ap.name.equalsIgnoreCase(name))
				return ap;
		}
		return null;
	}
	
	/*private class CheckedCell{
		AP ap;
		int meanRSS;
		int stdDeviation;
		
		public CheckedCell(AP ap, int meaRSS, int stdDeviation) {
			// TODO Auto-generated constructor stub
			this.ap = ap;
			this.meanRSS = meaRSS;
			this.stdDeviation = stdDeviation;					
		}
	}*/
	
	Local currentPos;
	Map map;
	/*ArrayList<CheckedCell> checkedCells;*/
	public ArrayList<Cell> checkedCells;
	public ArrayList<AP> aps;
	private WifiInterface wi;
	public static final int sample = 4;
	public Handler mHandler;
	List<ScanResult> results;
	Context context;
	
	public Localization(Map map, Local local, WifiInterface wi, Context c) throws IOException {
		// TODO Auto-generated constructor stub
		currentPos = local;
		checkedCells = new ArrayList<Cell>();
		aps = new ArrayList<Localization.AP>();
		initAPs();
		this.map = map;
		this.wi = wi;
		context = c;  
		mHandler = new Handler(){
				@Override
			public void handleMessage(Message msg) {
				// TODO Auto-generated method stub
				super.handleMessage(msg);
				switch (msg.what) {
				case 0:
					results = (List<ScanResult>) msg.obj;
					mHandler.notifyAll();
					break;
				case 1:
					Toast.makeText(context, (String)msg.obj, Toast.LENGTH_LONG).show();
					break;
				default:
					break;
				}
			}
		};
		
	}
	
	private void initAPs() throws IOException {
		// TODO Auto-generated method stub
		AssetManager am = context.getAssets();
		InputStreamReader inputreader = new InputStreamReader(am.open("map.txt"));
		BufferedReader buffreader = new BufferedReader(inputreader);
		String line;
		
		while ((line = buffreader.readLine()) != null){
	         String name = line.split(" ")[0];
	         line = line.split(" ")[1];
		     int y = Integer.parseInt(line.split(" ")[0]);
		     line = line.split(" ")[1];
		     int x = Integer.parseInt(line.split(" ")[0]);
		     aps.add(new AP(name, x, y));
		}
	}

	public Local getPos(List<ScanResult> results){
		if (currentPos == null){
			for(ScanResult sr: results){
				
			}
		}
		return currentPos;
	}
	
	public void updatePos(Local local){
		currentPos = local;
	}
	
	public void checkCell() throws InterruptedException{
		int i;
		for(i = 0; i < sample; i++){
			wi.getWifiManager().startScan();
			mHandler.wait();
			for(ScanResult sr: results){
				
			}
		}
		map.getMap()[currentPos.y][currentPos.x].checked = true;
		map.getMap()[currentPos.y][currentPos.x].pos.x = currentPos.x;
		map.getMap()[currentPos.y][currentPos.x].pos.y = currentPos.y;
		
		checkedCells.add(map.getMap()[currentPos.y][currentPos.x]);
	}

	public boolean isChecked() {
		return map.getMap()[currentPos.y][currentPos.x].checked;
	}
}
