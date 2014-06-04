package br.ufpr.Android2Robot.labyrinth;

import java.util.ArrayList;

public class Markov {
	class Pose{
		int x;
		int y;
		int teta;
		double prob;
		
		public Pose(int x, int y, 	int teta, double prob) {
			// TODO Auto-generated constructor stub
			this.x = x;
			this.y = y;
			this.teta = teta;
			this.prob = prob;
		}
	}
	
	double belief[][];
	int numCell2update;
	ArrayList<Pose> actionModel;
	ArrayList<Integer> perceptionModel;
	Labyrinth lab;
	double odometry[] = {0.25, 0.5, 0.25};
	
	
	public Pose higherProb(){
		Pose higher = new Pose(-1, -1, 0, -1);
		
		/*for(Pose pose: belief)
			if (pose.prob >= higher.prob)
				higher = pose;*/
		
		return higher;
	}
	
	public int calcAction(byte direction){
		
		/*int x = lab.x; int y = lab.y;
		
		for(int i = 0; i < numCell2update; i++){
			belief[y][x] =  
		}*/
		
		return 0;
	}
	
	public int calcPerception(){
		return 0;
	}
	
	public Markov(Labyrinth lab) {
		// TODO Auto-generated constructor stub
		this.lab = lab;
		belief = new double[lab.length][lab.width];
		belief[lab.y][lab.x] = 1; 
		numCell2update = 1;
	}
}
