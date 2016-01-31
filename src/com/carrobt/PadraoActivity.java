package com.carrobt;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;

public class PadraoActivity extends Activity {


	public AlertDialog.Builder buildMessageOK(Context context , String titulo ,String mensagem){
		AlertDialog.Builder dialogo = new AlertDialog.Builder(context);
		dialogo.setTitle(titulo);
		dialogo.setMessage(mensagem);
		dialogo.setNeutralButton("Ok", null);
		return dialogo;
	}
	
}
