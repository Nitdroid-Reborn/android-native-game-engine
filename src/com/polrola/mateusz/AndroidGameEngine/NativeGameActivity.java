package com.polrola.mateusz.AndroidGameEngine;

import android.app.NativeActivity;
import android.content.Context;
import android.media.AudioManager;
import android.os.Bundle;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.util.Log;


public class NativeGameActivity extends NativeActivity {
	private WakeLock wakeLock;

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
		Log.i("native-activity", "locked");
	}
	
	public void unlock() {
		wakeLock.release();
		Log.i("native-activity", "unlocked");
	}
	
	static {
		System.loadLibrary("box2d");
		System.loadLibrary("png");
	}
}