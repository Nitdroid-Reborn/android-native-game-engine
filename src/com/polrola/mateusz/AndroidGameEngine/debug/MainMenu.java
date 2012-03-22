package com.polrola.mateusz.AndroidGameEngine.debug;


import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

public class MainMenu extends Activity
{
	public static final String INDATA_ENABLE_NEWGAME_BUTTON = "enable_newgame_button";
	
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, 
                                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        
        
        
        setContentView(R.layout.view_menu);
        
        Intent data = getIntent();
        boolean enableNewGameBtn = data.getBooleanExtra(INDATA_ENABLE_NEWGAME_BUTTON, true);
        
        View abouButton = findViewById(R.id.new_game_button);
        abouButton.setEnabled(enableNewGameBtn);
    }
    
    public void onBtnNewGame(View v)
    {
    	startActivity(new Intent(this, NativeGameActivity.class));
    }
    
    public void onBtnCredits(View v)
    {
        startActivity(new Intent(this, Credits.class));
    }
    
    public void onBtnExit(View v)
    {
        finish();
    }
    
	static {
		System.loadLibrary("box2d");
		System.loadLibrary("png");
		System.loadLibrary("android-game-engine");
	}
}
