package br.ufpr.Android2Robot.Map;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.LineNumberReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.math3.stat.regression.RegressionResults;
import org.apache.commons.math3.stat.regression.SimpleRegression;

import br.ufpr.Android2Robot.wifi.WifiInterface;
import android.annotation.SuppressLint;
import android.content.Context;
import android.content.res.AssetManager;
import android.net.wifi.ScanResult;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;


@SuppressLint({ "HandlerLeak", "NewApi" })
public class Map {
	public final int INF = 0x7FFFFFFF;
	private boolean test; 
	public class AP {
		public String name;
		public Local pos;
		
		AP(String name, Local local){
			this.name = name;
			pos = local;
		}
	}
	
	public static class Local{
		public int x,y;
		public Local(int y, int x) {
			// TODO Auto-generated constructor stub
			this.x = x;
			this.y = y; 
		}
	}
	
	public class CellTable{
		public AP ap;
		public int meanRSS;
		public long meanTS;
		public int stdDeviantion;
		
		CellTable(AP ap){
			this.ap = ap;
		}
	}
	
	public static class Cell{
		public Local pos;
		public int value;
		public boolean checked;
		public ArrayList<CellTable> apTable;
		
		public CellTable getCellTable(String name){
			for(CellTable ap: apTable){
				if(ap.ap.name.equals(name))
					return ap;
			}
			return null;
		}
		
		public static CellTable getCellTable(String name, 
				ArrayList<CellTable> apTable){
			for(CellTable ct: apTable){
				if(ct.ap.name.equals(name))
					return ct;
			}
			return null;
		}
	}
	
	public AP getAP(String name){
		for(AP ap: apList){
			if(ap.name.equalsIgnoreCase(name))
				return ap;
		}
		return null;
	}
	
	/**=======CONSTANTES=======*/
	public static final int SAMPLE = 8;
	public static final int TAMCELL = 1;
	public static final byte STOP = 'S';
	public static final byte FOWARD = 'F';
	public static final byte BACK = 'B' ;
	public static final byte LEFT = 'L';
	public static final byte RIGHT = 'R';
	/**=======CONSTANTES=======*/
	
	/**=======ATRIBUTOS=======*/
	private Cell[][] map;
	ArrayList<AP> apList;
	public ArrayList<Cell> checkedCells;
	ArrayList<Byte> path;
	private List<ScanResult> results;
	public List<ScanResult> getResults() {
		return results;
	}

	public void setResults(List<ScanResult> results) {
		this.results = results;
	}

	private WifiInterface wi;
	public Handler mHandler;
	Context context;
	private int length;
	private int width;
	Local currentPos;
	private int indexPath;
	/**=======ATRIBUTOS=======*/
	
	
	/**=======METODOS=======*/
	public Map(String mapFile, Context context, 
			Local startPos, WifiInterface wi) throws IOException{
		this.context = context;
		this.wi = wi;
		
		checkedCells = new ArrayList<Cell>();
		apList = new ArrayList<Map.AP>();
		indexPath = 0;
		test = false;
		int i = 0;
		int j = 0;
		int k = 0;
		String line;
		this.map = null;
		if(startPos != null)
			currentPos = startPos;
		/*else
			currentPos = new Local(0, 0);*/
		AssetManager am = context.getAssets();
		InputStreamReader inputreader = new InputStreamReader(am.open(mapFile));

		BufferedReader buffreader = new BufferedReader(inputreader); 
		LineNumberReader  lnr = new LineNumberReader(new InputStreamReader(am.open(mapFile)));
		lnr.skip(Long.MAX_VALUE);
		int lineNumber = lnr.getLineNumber() + 1;
		lnr.close();
		Log.i("MAP", lineNumber + "");
        while ((line = buffreader.readLine()) != null){
        	Log.i("MAP " + i, line);
		     if (this.map == null) {
		    	map = new Cell[lineNumber][(line.length()/2)+1];
		    	length = lineNumber;
		    	width = (line.length()/2)+1;
		     }
		    	 
		     while(j < line.length()){
		    	 map[i][k] = new Cell();
		    	 map[i][k].value = Integer.parseInt(line.charAt(j)+"");
		    	 j += 2;
		    	 k++;
		     }
		     k = 0;
		     j = 0;
		     i++;
		} 
		initAPs();
		try {
			initTable();
		} catch (Exception e) {
			// TODO: handle exception
			Log.v("MAP ", "No tables " + e.getMessage());
		}

		wifiHandler();
		buffreader.close();
	}
	
	public boolean isTest() {
		return test;
	}

	public void setTest(boolean test) {
		this.test = test;
	}

	@SuppressLint("NewApi")
	private void initTable() throws IOException {
		// TODO Auto-generated method stub
		File file = new File(Environment.
				getExternalStoragePublicDirectory(Environment
						.DIRECTORY_DOWNLOADS), "wifiTable.txt");
		if(!file.exists()) {
			return;
		}
		
		BufferedReader buffreader = new BufferedReader(new FileReader(file));
		String line;
		while ((line = buffreader.readLine()) != null){
			String data[] = line.split(";");
			Cell current = map[Integer.parseInt(data[0])][Integer.parseInt(data[1])];

			if(!current.checked) {
				current.apTable = new ArrayList<Map.CellTable>();
				current.checked = true;
				current.pos = new Local(Integer.parseInt(data[0]), 
						Integer.parseInt(data[1]));
				checkedCells.add(current);
			}
			
			AP ap = getAP(data[2]);
			Log.i("A2R Map init Table APs", data[2] + " " + data[3]);
			if(ap != null) {
				addApTable(ap, current, Integer.parseInt(data[3]));
			} else {
				ap = new AP(data[2], new Local(-1,-1));
				apList.add(ap);
				addApTable(ap, current, Integer.parseInt(data[3]));
			}
		}
	}

	public void saveTable() throws IOException{
		File file = new File(Environment.
				getExternalStoragePublicDirectory(Environment
						.DIRECTORY_DOWNLOADS), "wifiTable.txt");
        FileOutputStream f = new FileOutputStream(file, false);
        f.getChannel().truncate(0);
        f.getChannel().force(true);
        f.getChannel().lock();
        PrintWriter pw = new PrintWriter(f);
		Log.i("A2R Map", "Almost there");
		for(Cell c : checkedCells){
			for(CellTable ct: c.apTable ){
				Log.i("A2R Map",c.pos.y + " " + 
						c.pos.x + " " + ct.ap.name + " " + ct.meanRSS);
				pw.write(c.pos.y + ";" + 
						c.pos.x + ";" + ct.ap.name + ";" + ct.meanRSS + "\n");
			}
			pw.flush();
		}	
        pw.close();
        f.close();
	}
	
	public Boolean updateCell(Local local, int meanRSS, int stdDeviantion){
		return true;
	}
	
	
	private void initAPs() throws IOException {
		// TODO Auto-generated method stub
		AssetManager am = context.getAssets();
		InputStreamReader inputreader = 
				new InputStreamReader(am.open("APs.txt"));
		BufferedReader buffreader = new BufferedReader(inputreader);
		String line;
		
		while ((line = buffreader.readLine()) != null){
	         String name = line.split(" ")[0];
		     int x = Integer.parseInt(line.split(" ")[1]);
		     int y = Integer.parseInt(line.split(" ")[2]);
		     apList.add(new AP(name, new Local(y, x)));
		}
		inputreader.close();
	}
	
	public void wifiHandler() {
		mHandler = new Handler(){
		@SuppressWarnings("unchecked")
		@Override
		public void handleMessage(Message msg) {
				// TODO Auto-generated method stub
				super.handleMessage(msg);
				switch (msg.what) {
				case 0:
					results = (List<ScanResult>) msg.obj;
					synchronized (mHandler) {
						mHandler.notifyAll();
					}
					break;
				case 1:
					Toast.makeText(context, (String)msg.obj, 
							Toast.LENGTH_LONG).show();
					break;
				default:
					break;
				}
			}
		};
	}
	
	public boolean isChecked() {
		return map[currentPos.y][currentPos.x].checked;
	}
	
	public void updatePos(int y, int x){
		if (currentPos == null) {
			currentPos = new Local(y, x);
			return;
		}
		currentPos.x = x;
		currentPos.y = y;
	}
	
	public ArrayList<CellTable> test(){
		String[] scan = {
			"5;4;Dinf3;-57", 
			"5;4;NR2_top;-71",
			"5;4;c3sl;-345",
			"5;4;PET_EST;-80",
			"5;4;Emmati;-85",
			"5;4;rei dos copos;-82",
			"5;4;LEG;-86",
			"5;4;NR2;-80",
			"5;4;;-68",
			"5;4;NR2Lenovo;-79",
			"5;4;NR2 5 GHz;-94",
			"5;4;galileu;-34" 
		};
		
		ArrayList<CellTable> apTable = new ArrayList<Map.CellTable>();
		for (String sc : scan) {
			String apInfo[] = sc.split(";");
			AP ap = getAP(apInfo[2]);
			if(ap != null) {
				CellTable ct = new CellTable(ap);
				ct.meanRSS = Integer.parseInt(apInfo[3]);
				apTable.add(ct);
			}
		}
		
		return apTable;
	}
	
	@SuppressLint("NewApi")
	public Local getPos() throws InterruptedException{
		int i;
		if (currentPos == null){
			int euclideanDist = INF;
			int smallerDist =  INF;
			Cell current = null;
			ArrayList<CellTable> apTable;
			ArrayList<Cell> positions = new ArrayList<Cell>();
			if(!isTest()) {
				apTable = new ArrayList<Map.CellTable>();
				for(i = 0; i < SAMPLE; i++){
					wi.getWifiManager().startScan();
					synchronized (mHandler) {
						mHandler.wait();
					}
					for(ScanResult sr: results){
						AP ap = getAP(sr.SSID);
						if(ap != null) {
							CellTable ct = Cell.getCellTable(ap.name, apTable);
							if(ct == null){
								ct = new CellTable(ap);
								ct.meanRSS = sr.level;
								//ct.meanTS = sr.timestamp;
								apTable.add(ct);
							} else {
								ct.meanRSS += sr.level;
								//ct.meanTS += sr.timestamp;
							}
						}
						Log.i("MAP APs", sr.SSID + " " + sr.level);
					}
				}
			} else {
				apTable = test();
			}
			for(int j = 0; j < apTable.size() - (apTable.size() > 2 ? 2 : 0); j++) {
				for(Cell cell: checkedCells){
					int partial = 0;
					for(int k = j; k < (apTable.size() > 2 ? 2 : 0) + j; k++){
						CellTable ct = cell.getCellTable(apTable.get(k).ap.name);
						if (ct != null)
							partial += Math.pow(ct.meanRSS - 
									apTable.get(k).meanRSS, 2);
						else
							partial += Math.pow(apTable.get(k).meanRSS, 2);
					}
					euclideanDist = (int) Math.sqrt(partial);
					if(smallerDist > euclideanDist){
						smallerDist = euclideanDist;
						current = cell;
					}
				}
				Log.i("A2R Map", current.pos.x + " " + current.pos.y );
				positions.add(current);
				smallerDist = INF;
			}
			
			SimpleRegression leastSquare = new SimpleRegression();
			int meanX = 0;
			for(Cell pos: positions){
				meanX += pos.pos.x;
				leastSquare.addData(pos.pos.x, pos.pos.y);
			}
			
			//RegressionResults r = leastSquare.regress();
			/*for(Cell cell: checkedCells){
				int partial = 0;
				for(CellTable ct: cell.apTable){
					CellTable sample = Cell.getCellTable(ct.ap.name, apTable);
					if (sample != null)
						partial += Math.pow(ct.meanRSS - sample.meanRSS, 2);
					else
						partial += Math.pow(ct.meanRSS, 2);
				}
				euclideanDist = (int) Math.sqrt(partial);
				Log.i("MAP cell get Pos",  cell.pos.x + " " + cell.pos.y + " " + euclideanDist + " " + cell.value);             
				if(smallerDist > euclideanDist){
					smallerDist = euclideanDist;
					current = cell;
				}
			}*/
			RegressionResults r = leastSquare.regress();
			Log.i("A2R Map", meanX + " " + leastSquare.getN());
			if(current != null){
				int x = (int) (meanX / leastSquare.getN());
				int y =  (int) leastSquare.predict(x)/*(int)leastSquare.getIntercept() + 
						(int)leastSquare.getSlope() * x*/;
				currentPos = new Local(y, x);
			}
			
			double[][] data = { { 4, 5 }, {4, 5 }, {4, 5 }, {4, 8 }, {5, 11 }};
			SimpleRegression regression = new SimpleRegression();
			//the argument, false, tells the class not to include a constant
			regression.addData(data);
			Log.i("FUCK", (int)regression.predict(4)+"");
		}
		setTest(false);
		return currentPos;
	}
	
	@SuppressLint("NewApi")
	public void checkCell(Handler h) throws InterruptedException{
		int i;
		Cell current = map[currentPos.y][currentPos.x];
		current.apTable = new ArrayList<Map.CellTable>();
		AP ap;
		for(i = 0; i < SAMPLE; i++){
			wi.getWifiManager().startScan();
			synchronized (mHandler) {
				mHandler.wait();
			}
			String message = "Getting samples. Please wait...\nSample " 
					+ (i + 1) + " de " + SAMPLE + ":";
			for(ScanResult sr: results){
				ap = getAP(sr.SSID);
				message += "\n" + sr.SSID + " " + sr.level;
				Log.i("MAP APs", sr.SSID + " " + sr.level);
				if(ap != null){
					addApTable(ap, current, sr.level);
				} else {
					ap = new AP(sr.SSID, new Local(-1,-1));
					apList.add(ap);
					addApTable(ap, current, sr.level);
				}
			}
			h.obtainMessage(4, message).sendToTarget();
		}
		
		for(CellTable ct: current.apTable){
			ct.meanRSS /= SAMPLE;
			//ct.meanTS /= SAMPLE;
			Log.i("Map Cell Mean", ct.meanRSS + " " + ct.ap.name);
		}
		
		if(current.pos == null) {
			checkedCells.add(current);
			current.pos = new Local(currentPos.y, currentPos.x);
		}
		
		current.checked = true;
		for(Cell c : checkedCells)
			Log.i("A2R checked cell", c.pos.x + " " + c.pos.y);
	}
	
	private void addApTable(AP ap, Cell current, int level) {
		// TODO Auto-generated method stub
		CellTable ct = current.getCellTable(ap.name);
		if(ct == null){
			ct = new CellTable(ap);
			ct.meanRSS = level;
			//ct.meanTS = sr.timestamp;
			current.apTable.add(ct);
		} else {
			if(current.checked){
				ct.meanRSS = 0;
				current.checked = false;
			}
			ct.meanRSS += level;
			//ct.meanTS += sr.timestamp;
		}
	}

	public Cell getCurrentCell(){
		if(currentPos != null)
			return map[currentPos.y][currentPos.x];
		return null;
		
	}
	public void calculatePath(ArrayList<Byte> path){
		indexPath = 0;
		if (path == null) {
			
		} else{
			path.clear();
			this.path = path;
		}
	}
	
	public byte getPath(){
		if (path != null) {
			indexPath++;
			return path.get(indexPath);
		}
		return 0;
	}
	
	public Cell[][] getMap(){
		return map;
	}

	public int getLength() {
		return length;
	}

	public int getWidth() {
		return width;
	}
	
	public void setPos(){
		currentPos = null;
	}
	
	@SuppressLint({ "WorldReadableFiles", "NewApi" })
	public void createTables() throws IOException{
		File tableDir = new File(Environment
				.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS), "Table");
		File checked = new File(Environment
				.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS), "checked.txt");
		
		if(!tableDir.exists())
			tableDir.mkdir();
		else{
			for(File file: tableDir.listFiles()){
				file.delete();
			}
			tableDir.delete();
		}
		upadateTable(tableDir);
	}
	
	public void upadateTable(File tableDir) throws IOException{
		for(Cell checkedCell : checkedCells){
			for(CellTable ap : checkedCell.apTable){
				File file = new File(tableDir,ap.ap.name + ".txt");
				FileOutputStream fOut = new FileOutputStream(file, true);
				OutputStreamWriter osw = new OutputStreamWriter(fOut); 
				osw.write(Integer.toString(ap.meanRSS) + " " 
						+ Integer.toString(checkedCell.pos.y) + " " 
						+ Integer.toString(checkedCell.pos.x) + "\n");
				osw.close();
			}
		}
	}
	
	@SuppressLint("NewApi")
	public boolean loadTables() throws IOException{
		File tableDir = new File(Environment
				.getExternalStoragePublicDirectory(
						Environment.DIRECTORY_DOWNLOADS), "Table");
		
		if(!tableDir.exists())
			return false;
		
		for(File file: tableDir.listFiles()){
			BufferedReader in = new BufferedReader(new FileReader(file));
			String line;
			AP ap = getAP(file.getName());
			
			if(ap == null) {
				ap = new AP(file.getName(), new Local(-1,-1));
				apList.add(ap);
			}
			
			while ((line = in.readLine()) != null) {
				int meanRSS, x,y;
				meanRSS = Integer.parseInt(line.split(" ")[0]);
				y = Integer.parseInt(line.split(" ")[1]);
				x = Integer.parseInt(line.split(" ")[2]);
				addApTable(ap, map[y][x], meanRSS);
				
				if (!map[y][x].checked) {
					checkedCells.add(map[y][x]);
					map[y][x].checked = true;
				}				
			}
		}
		
		return true;
	}
}
