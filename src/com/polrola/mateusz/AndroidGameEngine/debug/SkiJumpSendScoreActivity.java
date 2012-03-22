package com.polrola.mateusz.AndroidGameEngine.debug;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

public class SkiJumpSendScoreActivity extends Activity
{
    private final String TAG = "native-activity";

    public static final String INDATA_SCORE = "hiscores_score";
    public static final String INDATA_NAME = "hiscores_defname";
    
    public static final int RESULT_CANCELED = 0;
    public static final int RESULT_SUCCESS = 1;
    public static final int RESULT_ERROR = -1;

    private final String ADDRESS = "http://mms.rootnode.net/server_magik";
    private final String SALTED_HASH = "-##%s_rawr!";

    private final String REQ_ADD_SCORE = "set";

    private String mPlayerDefName = "";
    private int mPlayerScore = -1;

    private WakeLock wakeLock;
    
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.dialog_hiscores);

        Context context = (Context)this;
		PowerManager powerManager = (PowerManager)context.getSystemService(Context.POWER_SERVICE);
		wakeLock = powerManager.newWakeLock(PowerManager.FULL_WAKE_LOCK, "native-activity");
		
        Intent data = getIntent();
        mPlayerScore = data.getIntExtra(INDATA_SCORE, -1);
        mPlayerDefName = data.getStringExtra(INDATA_NAME);
        

        if (mPlayerDefName != null && mPlayerDefName.length() > 0)
        {
            ((EditText) findViewById(R.id.hiscores_name))
                    .setText(mPlayerDefName);
        }

        setResult(RESULT_CANCELED);
    }

    @Override
    protected void onResume() {
    	// TODO Auto-generated method stub
    	super.onResume();
    	wakeLock.acquire();
    }
    @Override
    protected void onPause() {
    	// TODO Auto-generated method stub
    	super.onPause();
    	wakeLock.release();
    }
    
    public void onBtnSend(View v)
    {
        EditText inputName = (EditText) findViewById(R.id.hiscores_name);
        final String name = inputName.getEditableText().toString();

        if (name == null || name.length() <= 0)
        {
            Toast.makeText(this, "Wpisz imię w polu powyżej",
                    Toast.LENGTH_SHORT).show();
        }
        else if (name.length() > 20)
        {
            Toast.makeText(this, "Maksymalnie 20 znaków", Toast.LENGTH_SHORT)
                    .show();
        }
        else if (mPlayerScore < 0)
        {
            Toast.makeText(this, "Wynik jest niepoprawny", Toast.LENGTH_SHORT)
            .show();
        }
        else
        {
        	/*boolean r = AddScore(name, mPlayerScore);
            setResult(r ? RESULT_SUCCESS : RESULT_ERROR);
            finish();*/
            Thread t = new Thread()
            {
                public void run()
                {
                    boolean r = AddScore(name, mPlayerScore);
                    setResult(r ? RESULT_SUCCESS : RESULT_ERROR);
                    finish();
                }
            };
            t.start();
        }
    }

    public static String BytesToHex(byte[] b)
    {
        String result = "";
        for (int i = 0; i < b.length; i++)
        {
            result += Integer.toString((b[i] & 0xff) + 0x100, 16).substring(1);
        }
        return result;
    }

    private String MD5(String text)
    {
        try
        {
            byte[] bytes = MessageDigest.getInstance("MD5").digest(
                    String.format(SALTED_HASH, text).getBytes("utf-8"));
            return BytesToHex(bytes);
        }
        catch (NoSuchAlgorithmException e)
        {
            e.printStackTrace();
        }
        catch (UnsupportedEncodingException e)
        {
            e.printStackTrace();
        }
        return null;
    }

    private boolean AddScore(String name, int pts)
    {
        try
        {
        	Log.d(TAG, "add score");
            HttpClient mHttpClient = new DefaultHttpClient();
            HttpGet get = new HttpGet(ADDRESS 
                    + "?request=" + REQ_ADD_SCORE
                    + "&name=" + name 
                    + "&pts=" + pts);
            get.addHeader("x-security", MD5(name + pts));

            HttpResponse response = mHttpClient.execute(get);
            if (response != null)
            {
                boolean success = response.getStatusLine().getStatusCode() == 200;
                BufferedReader is = new BufferedReader(new InputStreamReader(
                        response.getEntity().getContent()));

                String responseString = "";
                String line;
                while ((line = is.readLine()) != null)
                {
                    responseString += line + "\n";
                }
                
                if (success)
                {
                    //Log.d(TAG, "Server returned: " + responseString);
                }
                else
                {
                    Log.e(TAG, "Server returned an error "
                            + response.getStatusLine().getStatusCode() + ": "
                            + responseString);
                }
                
                return success;
            }
        }
        catch (ClientProtocolException e)
        {
            e.printStackTrace();
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }

        return false;
    }
}