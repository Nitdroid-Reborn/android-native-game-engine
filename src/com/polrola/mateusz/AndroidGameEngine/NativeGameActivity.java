package com.polrola.mateusz.AndroidGameEngine;

import android.app.NativeActivity;
import android.content.Context;
import android.content.Intent;
import android.media.AudioManager;
import android.os.Bundle;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.util.Log;



public class NativeGameActivity extends NativeActivity {
	private WakeLock wakeLock;
	
	private final int REQUESTCODE_SKIJUMP = 1;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		Context context = (Context)this;
		PowerManager powerManager = (PowerManager)context.getSystemService(Context.POWER_SERVICE);
		wakeLock = powerManager.newWakeLock(PowerManager.FULL_WAKE_LOCK, "native-activity");
		
		setVolumeControlStream(AudioManager.STREAM_MUSIC);
	}
	
	public void lock() {
		wakeLock.acquire();
		Log.i("native-activity", "WakeLock acquired");
	}
	
	public void unlock() {
		wakeLock.release();
		Log.i("native-activity", "WakeLock released");
	}
	
	public void SendToServer() {
		Log.d("native-activity", "Data send");
		Intent in = new Intent(this, SkiJumpSendScoreActivity.class);
        in.putExtra(SkiJumpSendScoreActivity.INDATA_SCORE, (int)(Math.random() * 0xffff));
        startActivityForResult(in, REQUESTCODE_SKIJUMP);
	}
	
	static {
		System.loadLibrary("box2d");
		System.loadLibrary("png");
	}
}