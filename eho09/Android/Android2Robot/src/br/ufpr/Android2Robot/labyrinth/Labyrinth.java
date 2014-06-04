package br.ufpr.Android2Robot.labyrinth;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.LineNumberReader;
import android.content.Context;
import android.content.res.AssetManager;
import android.util.Log;

public class Labyrinth {
	
	int path[][];
	int map[][];
	int length;
    int width;
	int direction;
	int Front = 8;
	int Right = 4;
	int Back = 2;
	int Left = 1;
	int x, y;
	int teta;
    
	public Labyrinth(String mapFile, Context context, int x,int y, int teta) {
		// TODO Auto-generated constructor stub
		int i = 0;
		int j = 0;
		this.x = x;
		this.y = y;
		this.teta = teta;
		AssetManager am = context.getAssets();
		try {
			InputStreamReader inputreader = new InputStreamReader(am.open(mapFile));
			BufferedReader buffreader = new BufferedReader(inputreader); 
			LineNumberReader  lnr = new LineNumberReader(new InputStreamReader(am.open(mapFile)));
			lnr.skip(Long.MAX_VALUE);
			int lineNumber = lnr.getLineNumber() + 1;
			lnr.close();
			Log.i("MAP", lineNumber + "");
	        String line;
			while ((line = buffreader.readLine()) != null){
				
	        	Log.i("MAP lab" + i, line);
			    if (this.map == null) {
			    	length = lineNumber;
				    width = line.split(" ").length;
			    	map = new int[length][width];
			    	path = new int[length][width];
			    }
			    
			    for(String cell : line.split(" ")){
			    	path[i][j] = 0;
			    	map[i][j++] = Integer.parseInt(cell);			    	
			    }
			    
			    j = 0;
			    i++;
			} 
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		Log.i("MAP", "END");
	}
	
	public int getConfig(int front, int left, int right){
				
		for(int i = 0; i < teta / 90 -1; i++){
			front = (front << 1) % 16 + (front << 1) / 16;
			left = (left << 1) % 16 + (left << 1) / 16;
			right = (right << 1) % 16 + (right << 1) / 16;
		}
		
		return right + left + front;
	}
	
	public void updatePos(){
		path[y][x]++;
		x -= (int)Math.cos(Math.toRadians(teta));
		y -= (int)Math.sin(Math.toRadians(teta));
	}
}
