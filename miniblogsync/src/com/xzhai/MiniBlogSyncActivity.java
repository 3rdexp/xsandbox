package com.xzhai;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class MiniBlogSyncActivity extends Activity {
	private Button m_WeiboButton = null;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        m_WeiboButton = (Button)this.findViewById(R.id.weibo);
        m_WeiboButton.setOnClickListener(new OnClickListener() {
        	@Override
        	public void onClick(View v) {
        		if (v == m_WeiboButton) 
        		{
        			Intent intent = new Intent();
        			intent.setClass(MiniBlogSyncActivity.this, WeiboActivity.class);
        			startActivity(intent);
        		}
        	}
        });
    }
}