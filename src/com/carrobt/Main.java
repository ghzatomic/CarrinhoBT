package com.carrobt;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import android.widget.ToggleButton;

public class Main extends PadraoActivity  implements SensorEventListener {

	private SensorManager sensorManager;
	Thread workerThread;
	byte[] readBuffer;
	int readBufferPosition;
	int counter;
	volatile boolean stopWorker;

	private String BT_CARRO_NOME = "linvor";
	
	Integer REQ_BT_ENABLE = 1;

	private Button BTN_NORTE = null;
	private Button BTN_SUL = null;
	private Button BTN_LESTE = null;
	private Button BTN_OESTE = null;
	private Button BTN_90MAIS = null;
	private Button BTN_90MENOS = null;
	private Button BTN_180MAIS = null;
	private Button BTN_180MENOS = null;
	private Button BTN_PROCURAR_BT = null;
	private Button BTN_PARARTUDO = null;

	private EditText EDT_STATUS = null;
	private EditText EDT_RETORNO = null;

	private ToggleButton TB_BT = null;
	
	private ToggleButton TB_MODOACELEROMETRO = null;

	private BluetoothDevice deviceBT = null;

	private BluetoothAdapter myBTadapter = null;

	private BluetoothSocket socketBT = null;

	private OutputStream outBT = null;

	private InputStream inBT = null;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		this.setRequestedOrientation(
				ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
		sensorManager=(SensorManager) getSystemService(SENSOR_SERVICE);
        sensorManager.registerListener(this, sensorManager.getDefaultSensor(Sensor.TYPE_ORIENTATION), SensorManager.SENSOR_DELAY_GAME);
		init();
	}

	private void init() {
		this.BTN_NORTE = (Button) findViewById(R.id.BTN_NORTE);
		this.BTN_SUL = (Button) findViewById(R.id.BTN_SUL);
		this.BTN_LESTE = (Button) findViewById(R.id.BTN_LESTE);
		this.BTN_OESTE = (Button) findViewById(R.id.BTN_OESTE);
		this.BTN_90MAIS = (Button) findViewById(R.id.BTN_90MAIS);
		this.BTN_90MENOS = (Button) findViewById(R.id.BTN_90MENOS);
		this.BTN_180MAIS = (Button) findViewById(R.id.BTN_180MAIS);
		this.BTN_180MENOS = (Button) findViewById(R.id.BTN_180MENOS);
		this.BTN_PARARTUDO = (Button) findViewById(R.id.BTN_PARARTUDO);
		this.EDT_RETORNO = (EditText) findViewById(R.id.EDT_RETORNO);
		this.EDT_STATUS = (EditText) findViewById(R.id.EDT_STATUS);
		this.TB_MODOACELEROMETRO = (ToggleButton) findViewById(R.id.TB_MODOACELEROMETRO);
		this.TB_BT = (ToggleButton) findViewById(R.id.TB_BT);
		this.BTN_PROCURAR_BT = (Button) findViewById(R.id.BTN_PROCURAR_BT);
		myBTadapter = BluetoothAdapter.getDefaultAdapter();
		this.setEnabledBotoesControle(false);
		if (myBTadapter.isEnabled()) {
			this.BTN_PROCURAR_BT.setEnabled(true);
			this.TB_BT.setChecked(true);
		} else {
			this.BTN_PROCURAR_BT.setEnabled(false);
			this.TB_BT.setChecked(false);
		}

	}

	public void changeStatus(String status) {

	}

	private void setEnabledBotoesControle(boolean vlr) {
		this.BTN_NORTE.setEnabled(vlr);
		this.BTN_SUL.setEnabled(vlr);
		this.BTN_LESTE.setEnabled(vlr);
		this.BTN_OESTE.setEnabled(vlr);
		this.BTN_PARARTUDO.setEnabled(vlr);
		this.BTN_90MAIS.setEnabled(vlr);
		this.BTN_90MENOS.setEnabled(vlr);
		this.BTN_180MAIS.setEnabled(vlr);
		this.BTN_180MENOS.setEnabled(vlr);
		this.TB_MODOACELEROMETRO.setEnabled(vlr);
	}

	public void pararTudoPress(View view) {
		String msg = "5;0";
		msg += "\n";
		try {
			if (outBT != null) {
				outBT.write(msg.getBytes());
				this.EDT_STATUS.setText("Envado Parar");
			}
		} catch (IOException e) {
			super.buildMessageOK(this, "Erro", "Erro ao enviar : " + msg);
		}
	}

	public void nortePress(View view) {
		String msg = "1;0";
		msg += "\n";
		try {
			if (outBT != null) {
				outBT.write(msg.getBytes());
				this.EDT_STATUS.setText("Enviado norte");
			}
		} catch (IOException e) {
			super.buildMessageOK(this, "Erro", "Erro ao enviar : " + msg);
		}
	}

	public void sulPress(View view) {
		String msg = "2;0";
		msg += "\n";
		try {
			if (outBT != null) {
				outBT.write(msg.getBytes());
				this.EDT_STATUS.setText("Enviado sul");
			}
		} catch (IOException e) {
			super.buildMessageOK(this, "Erro", "Erro ao enviar : " + msg);
		}
	}

	public void mais90Press(View view) {
		String msg = "3;400";
		msg += "\n";
		try {
			if (outBT != null) {
				outBT.write(msg.getBytes());
				this.EDT_STATUS.setText("Enviado +90");
			}
		} catch (IOException e) {
			super.buildMessageOK(this, "Erro", "Erro ao enviar : " + msg);
		}
	}
	
	public void menos90Press(View view) {
		String msg = "4;400";
		msg += "\n";
		try {
			if (outBT != null) {
				outBT.write(msg.getBytes());
				this.EDT_STATUS.setText("Enviado -90");
			}
		} catch (IOException e) {
			super.buildMessageOK(this, "Erro", "Erro ao enviar : " + msg);
		}
	}
	
	public void mais180Press(View view) {
		String msg = "3;800";
		msg += "\n";
		try {
			if (outBT != null) {
				outBT.write(msg.getBytes());
				this.EDT_STATUS.setText("Enviado +180");
			}
		} catch (IOException e) {
			super.buildMessageOK(this, "Erro", "Erro ao enviar : " + msg);
		}
	}
	
	public void menos180Press(View view) {
		String msg = "4;800";
		msg += "\n";
		try {
			if (outBT != null) {
				outBT.write(msg.getBytes());
				this.EDT_STATUS.setText("Enviado -180");
			}
		} catch (IOException e) {
			super.buildMessageOK(this, "Erro", "Erro ao enviar : " + msg);
		}
	}
	
	public void lestePress(View view) {
		String msg = "4;50";
		msg += "\n";
		try {
			if (outBT != null) {
				outBT.write(msg.getBytes());
				this.EDT_STATUS.setText("Enviado leste");
			}
		} catch (IOException e) {
			super.buildMessageOK(this, "Erro", "Erro ao enviar : " + msg);
		}
	}

	public void oestePress(View view) {
		String msg = "3;50";
		msg += "\n";
		try {
			if (outBT != null) {
				outBT.write(msg.getBytes());
				this.EDT_STATUS.setText("Enviado oeste");
			}
		} catch (IOException e) {
			super.buildMessageOK(this, "Erro", "Erro ao enviar : " + msg);
		}
	}

	public void btOnClick(View view) {
		if (this.TB_BT.isChecked()) {
			if (myBTadapter == null) {
				Toast.makeText(getApplicationContext(),
						"Device doesn't support Bluetooth", Toast.LENGTH_LONG)
						.show();
				this.BTN_PROCURAR_BT.setEnabled(false);
				this.setEnabledBotoesControle(false);
			} else {
				if (!myBTadapter.isEnabled()) {
					Intent enableBtIntent = new Intent(
							BluetoothAdapter.ACTION_REQUEST_ENABLE);
					startActivityForResult(enableBtIntent, REQ_BT_ENABLE);
					Toast.makeText(getApplicationContext(),
							"Enabling Bluetooth!!", Toast.LENGTH_LONG).show();
				}
			}
		} else {
			Toast.makeText(getApplicationContext(), "Disabling Bluetooth!!",
					Toast.LENGTH_LONG).show();
			myBTadapter.disable();
		}
	}

	public void procurarBlueTooth(View view) {
		this.EDT_STATUS.setText("Procurando " + BT_CARRO_NOME);
		Set<BluetoothDevice> pairedDevices = myBTadapter.getBondedDevices();
		if (pairedDevices.size() > 0) {
			for (BluetoothDevice device : pairedDevices) {
				this.EDT_STATUS.setText("Vendo:" + device.getName());
				if (device.getName().equals(BT_CARRO_NOME)) // Note, you will
															// need to change
															// this to match the
															// name of your
															// device
				{
					deviceBT = device;
					break;
				}
			}
		}
		if (deviceBT != null) {
			try {
				Toast.makeText(getApplicationContext(),
						"Encontrou o " + BT_CARRO_NOME, Toast.LENGTH_LONG)
						.show();
				carregarSocketBT();
			} catch (Exception e) {
				Toast.makeText(getApplicationContext(),
						"Erro ao carregar o socket !", Toast.LENGTH_LONG)
						.show();
				this.EDT_STATUS.setText("Erro de conex�o");
			}
		} else {
			Toast.makeText(getApplicationContext(),
					"N�o achou o dispositivo : " + BT_CARRO_NOME,
					Toast.LENGTH_LONG).show();
		}
	}

	private void carregarSocketBT() throws IOException {
		this.EDT_STATUS.setText("Montando conex�o");
		UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb"); // Standard
																				// SerialPortService
																				// ID
		socketBT = deviceBT.createRfcommSocketToServiceRecord(uuid);
		socketBT.connect();
		outBT = socketBT.getOutputStream();
		inBT = socketBT.getInputStream();
		this.setEnabledBotoesControle(true);
		beginListenForData();
		this.EDT_STATUS.setText("Aguardando...");
	}

	public void sairClick(View view) {
		super.finish();
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		if (requestCode == REQ_BT_ENABLE) {
			if (resultCode == RESULT_OK) {
				Toast.makeText(getApplicationContext(),
						"BlueTooth is now Enabled", Toast.LENGTH_LONG).show();
				this.BTN_PROCURAR_BT.setEnabled(true);
				this.EDT_STATUS.setText("Aguardando ...");
			}
			if (resultCode == RESULT_CANCELED) {
				Toast.makeText(getApplicationContext(),
						"Erro ao abilitar o BlueTooth", Toast.LENGTH_LONG)
						.show();
				// /finish();
			}
		}
	}// onActivityResult

	void beginListenForData() {
		final Handler handler = new Handler();
		final byte delimiter = 10; // This is the ASCII code for a newline
									// character

		stopWorker = false;
		readBufferPosition = 0;
		readBuffer = new byte[1024];
		workerThread = new Thread(new Runnable() {
			public void run() {
				while (!Thread.currentThread().isInterrupted() && !stopWorker) {
					try {
						int bytesAvailable = inBT.available();
						if (bytesAvailable > 0) {
							byte[] packetBytes = new byte[bytesAvailable];
							inBT.read(packetBytes);
							for (int i = 0; i < bytesAvailable; i++) {
								byte b = packetBytes[i];
								if (b == delimiter) {
									byte[] encodedBytes = new byte[readBufferPosition];
									System.arraycopy(readBuffer, 0,
											encodedBytes, 0,
											encodedBytes.length);
									final String data = new String(
											encodedBytes, "US-ASCII").replace("\n", "").replace("/n", "").replace("\0", "").replace(" ", "");
									readBufferPosition = 0;

									handler.post(new Runnable() {
										public void run() {
											EDT_RETORNO.setText(data);
											if (data.contains("1")){
												EDT_RETORNO.setText("Indo norte!");
											}else if (data.contains("2")){
												EDT_RETORNO.setText("Indo sul!");
											}else if (data.contains("3")){
												EDT_RETORNO.setText("virando oeste !!");
											}else if (data.contains("4")){
												EDT_RETORNO.setText("Virando leste !!");
											}else if (data.contains("5")){
												EDT_RETORNO.setText("Parei !!");
											}
										}
									});
								} else {
									readBuffer[readBufferPosition++] = b;
								}
							}
						}
					} catch (IOException ex) {
						stopWorker = true;
					}
				}
			}
		});

		workerThread.start();
	}

	@Override
	public void onAccuracyChanged(Sensor arg0, int arg1) {
	}

	@Override
	public void onSensorChanged(SensorEvent event) {
		if (event.sensor.getType() == Sensor.TYPE_ORIENTATION) {
			if(TB_MODOACELEROMETRO.isChecked()){
		        float[] values = event.values.clone();
		        //SensorManager.getRotationMatrix (R, null, aValues, mValues);
		        values = event.values;
                //this.EDT_STATUS.setText(values[0]+"  "+values[1]+"  "+values[2]);
		        
				if(values[1] >= 20.0){
					String msg = "1;0";
					msg += "\n";
					try {
						if (outBT != null) {
							outBT.write(msg.getBytes());
							this.EDT_STATUS.setText("Enviado norte");
						}
					} catch (IOException e) {
						super.buildMessageOK(this, "Erro", "Erro ao enviar : " + msg);
					}
				}else if(values[1] <= -20.0){
					String msg = "2;0";
					msg += "\n";
					try {
						if (outBT != null) {
							outBT.write(msg.getBytes());
							this.EDT_STATUS.setText("Enviado sul");
						}
					} catch (IOException e) {
						super.buildMessageOK(this, "Erro", "Erro ao enviar : " + msg);
					}
				}else if(values[2] <= -20.0){
					String msg = "3;0";
					msg += "\n";
					try {
						if (outBT != null) {
							outBT.write(msg.getBytes());
							this.EDT_STATUS.setText("Enviado oeste");
						}
					} catch (IOException e) {
						super.buildMessageOK(this, "Erro", "Erro ao enviar : " + msg);
					}
					
				}else if(values[2] >= 20.0){
					String msg = "4;0";
					msg += "\n";
					try {
						if (outBT != null) {
							outBT.write(msg.getBytes());
							this.EDT_STATUS.setText("Enviado leste");
						}
					} catch (IOException e) {
						super.buildMessageOK(this, "Erro", "Erro ao enviar : " + msg);
					}
				}else if((values[1] >= -10.0) && (values[1] <= 10.0)){
					String msg = "5;0";
					msg += "\n";
					try {
						if (outBT != null) {
							outBT.write(msg.getBytes());
							this.EDT_STATUS.setText("Envado Parar");
						}
					} catch (IOException e) {
						super.buildMessageOK(this, "Erro", "Erro ao enviar : " + msg);
					}
				}else if((values[2] >= -10.0) && (values[2] <= 10.0)){
					String msg = "5;0";
					msg += "\n";
					try {
						if (outBT != null) {
							outBT.write(msg.getBytes());
							this.EDT_STATUS.setText("Envado Parar");
						}
					} catch (IOException e) {
						super.buildMessageOK(this, "Erro", "Erro ao enviar : " + msg);
					}
				}
			}
			//ax = event.values[0];
			//ay = event.values[1];
			//az = event.values[2];
		}
	}
	
}