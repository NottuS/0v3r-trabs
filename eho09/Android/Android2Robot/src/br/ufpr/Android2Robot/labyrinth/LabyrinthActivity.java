package br.ufpr.Android2Robot.labyrinth;

import br.ufpr.Android2Robot.R;
import br.ufpr.Android2Robot.labyrinth.LabyrinthService;
import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

@SuppressLint({ "ValidFragment", "NewApi", "CutPasteId" })
public class LabyrinthActivity extends Activity {
	String x,y, teta;
	TextView map;
	Button exit;
	int ind;
	private LabyrinthReceiver mReceiver;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.labyrinth);
		map = (TextView) findViewById(R.id.PoseMap);
		DialogFragment poseDialog = new PoseDialog();
		poseDialog.show(getFragmentManager(), "Initial Pose");
		IntentFilter filter = new IntentFilter("com.Labyrinth");  
	    mReceiver = new LabyrinthReceiver();  
	    registerReceiver(mReceiver, filter);
	    exit = (Button) findViewById(R.id.labExit);
	    
	    exit.setOnClickListener(new View.OnClickListener() {
			public void onClick (View v) {
				Intent intent = new Intent("com.lab.service");
				sendBroadcast(intent);
				finish();
			}
		});
		
	}
	
	
	
	@TargetApi(Build.VERSION_CODES.HONEYCOMB)
	@SuppressLint("NewApi")
	public class PoseDialog extends DialogFragment {

	    @Override
	    public Dialog onCreateDialog(Bundle savedInstanceState) {
	    	
	    	LayoutInflater inflater = getActivity().getLayoutInflater();

	        // Use the Builder class for convenient dialog construction
	        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
	        View layout = inflater.inflate(R.layout.pose, null);
	        
	        builder.setView(layout);
	        final EditText X = (EditText) layout.findViewById(R.id.PoseX);
            final EditText Y = (EditText) layout.findViewById(R.id.PoseY);
            final EditText Teta = (EditText) layout.findViewById(R.id.Poseteta);
	        builder.setMessage("Initial Pose");
	        builder.setPositiveButton("start", new DialogInterface.OnClickListener() {
	                   public void onClick(DialogInterface dialog, int id) {
	                	   x = X.getText().toString();
	                	   y = Y.getText().toString();
	                	   teta = Teta.getText().toString();
	                	   Log.i("POSE", x+ " " + y + " " + teta);
	                	   Intent i = new Intent(getApplicationContext(), LabyrinthService.class);
	                	   /*i.putExtra("x", x);
	                	   i.putExtra("y", y);
	                	   i.putExtra("teta", teta);*/
	                	   startService(i);
	                   }
	               })
	               .setNegativeButton("cancel", new DialogInterface.OnClickListener() {
	                   public void onClick(DialogInterface dialog, int id) {
	                       finish();
	                   }
	               });

	        // Create the AlertDialog object and return it
	        return builder.create();
	    }
	}
	
	public class LabyrinthReceiver extends BroadcastReceiver {  

		@Override
		public void onReceive(Context arg0, Intent arg1) {
			// TODO Auto-generated method stub
			int lenght, width;
			int path[][];
			String update = "";
			lenght = arg1.getIntExtra("lenght", -1);
			width = arg1.getIntExtra("width", -1);
			int left = arg1.getIntExtra("leftSonar", -1);
			int front = arg1.getIntExtra("frontSonar", -1);
			int right = arg1.getIntExtra("rightSonar", -1);
			int ldr = arg1.getIntExtra("ldr", -1);
			Log.i("Lab Receiver", "onReceive");
			if(width != -1 && lenght != -1){
				path = new int[lenght][width];
				for(int i = 0; i < lenght; i++){
					path[i] = arg1.getIntArrayExtra("Path" + i);
				}
				
				for(int i = 0; i < lenght; i++){
					for(int j = 0; j < lenght; j++){
						update += path[i][j] + "\t";
					}
					update += "\n";
				}
				update += "\n leftSonar: " + left;
				update += "\n frontSonar: " + front;
				update += "\n rightSonar: " + right;
				update += "\n LDR: " + ldr;
				map.setText(update);
				Log.i("Lab BR", update);
			} else {
				Log.i("Lab BR", "No extra");
			}
		}  
    }
	
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onPause();
		unregisterReceiver(mReceiver);
	}
}
