package com.polrola.mateusz.AndroidGameEngine.debug;

import android.app.NativeActivity;
import android.content.Context;
import android.content.Intent;
import android.media.AudioManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.PowerManager;
import android.os.Vibrator;
import android.os.PowerManager.WakeLock;
import android.util.Log;



public class NativeGameActivity extends NativeActivity {
	private WakeLock wakeLock;
	
	private final int REQUESTCODE_SKIJUMP = 1;
	private Vibrator vibrator;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		Context context = (Context)this;
		PowerManager powerManager = (PowerManager)context.getSystemService(Context.POWER_SERVICE);
		wakeLock = powerManager.newWakeLock(PowerManager.FULL_WAKE_LOCK, "native-activity");
		
		vibrator = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
		
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
	
	public void SendToServer(int distance) {
		Intent in = new Intent(this, SkiJumpSendScoreActivity.class);
        in.putExtra(SkiJumpSendScoreActivity.INDATA_SCORE, (int)((float)distance/10));
        startActivityForResult(in, REQUESTCODE_SKIJUMP);
	}
	
	public void Vibrate(int miliseconds) {
		vibrator.vibrate(miliseconds);
	}
	
	public void ShowMenu() {
		Intent in = new Intent(this, MainMenu.class);
		in.putExtra(MainMenu.INDATA_ENABLE_NEWGAME_BUTTON, (boolean)false);
		startActivity(in);
	}
	
	
	public void SendCrashReport(String crashFileName) {
		Log.e("ski jump", "Sending creash " + crashFileName);
		Intent intent = new Intent(Intent.ACTION_SEND);
		intent.putExtra(Intent.EXTRA_EMAIL, new String[]{"mateo343@gmail.com"});
		intent.putExtra(Intent.EXTRA_SUBJECT, "Skijumping crash report");
		intent.putExtra(Intent.EXTRA_TEXT, "Ups crash");
		intent.setType("application/zip");
		Log.e("ski jump", "setting data");
		intent.putExtra(Intent.EXTRA_STREAM, Uri.parse("file://" + crashFileName));
		Log.e("ski jump", "Starting activity");
		startActivity(Intent.createChooser(intent, "Send Email"));
	}

}