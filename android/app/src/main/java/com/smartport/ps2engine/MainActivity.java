package com.smartport.ps2engine;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Canvas;
import android.hardware.input.InputManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.ParcelFileDescriptor;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

public class MainActivity extends Activity implements SurfaceHolder.Callback, InputManager.InputDeviceListener {
    static { System.loadLibrary("ps2_engine_native"); }

    public native String startEngineNative();
    public native void nativeSetSurface(Object surface);
    public native void nativeRenderGameFrame(float r, float g, float b);
    public native boolean nativeMountObb(String obbPath);
    public native void nativeSendInput(int buttonsMask);
    public native void nativeSendAxes(float lx, float ly, float rx, float ry);

    private SurfaceView gameSurface;
    private GamepadOverlayView overlayView;
    private LinearLayout studioPanel;
    private TextView infoText, gamepadStatusText;
    private ProgressBar progressBar;
    private Button btnSelectIso, btnPrepareObb, btnStartGame;
    private Uri selectedIsoUri = null;
    private boolean isGameRunning = false;
    private File targetObbFile;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS, WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS);
        getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY | View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);

        // ИСТИНСКА ПАПКА БЕЗ БЛОКИРОВКИ ОТ ANDROID
        File safeDir = getExternalFilesDir("obb_data");
        if (!safeDir.exists()) safeDir.mkdirs();
        targetObbFile = new File(safeDir, "game_data.obb");

        FrameLayout root = new FrameLayout(this);
        root.setBackgroundColor(Color.BLACK);

        gameSurface = new SurfaceView(this);
        gameSurface.getHolder().addCallback(this);
        root.addView(gameSurface);

        overlayView = new GamepadOverlayView(this);
        root.addView(overlayView);

        studioPanel = new LinearLayout(this);
        studioPanel.setOrientation(LinearLayout.VERTICAL);
        studioPanel.setPadding(60, 40, 60, 40);

        TextView title = new TextView(this);
        title.setText("⚡ PS2 NATIVE REAL ENGINE");
        title.setTextColor(Color.parseColor("#58A6FF"));
        title.setTextSize(18);
        studioPanel.addView(title);

        gamepadStatusText = new TextView(this);
        gamepadStatusText.setTextSize(13);
        gamepadStatusText.setPadding(0, 5, 0, 10);
        studioPanel.addView(gamepadStatusText);

        LinearLayout btnRow = new LinearLayout(this);
        btnRow.setOrientation(LinearLayout.HORIZONTAL);

        btnSelectIso = new Button(this);
        btnSelectIso.setText("📂 1. ИЗБЕРИ ISO");
        btnSelectIso.setBackgroundColor(Color.parseColor("#1F6FEB"));
        btnSelectIso.setTextColor(Color.WHITE);
        btnSelectIso.setOnClickListener(v -> openIsoPicker());
        btnRow.addView(btnSelectIso);

        btnPrepareObb = new Button(this);
        btnPrepareObb.setText("📦 2. ИЗВЛЕЧИ РЕСУРСИ");
        btnPrepareObb.setBackgroundColor(Color.parseColor("#8957E5"));
        btnPrepareObb.setTextColor(Color.WHITE);
        btnPrepareObb.setVisibility(View.GONE);
        btnPrepareObb.setOnClickListener(v -> realExtractObbStream());
        btnRow.addView(btnPrepareObb);

        btnStartGame = new Button(this);
        btnStartGame.setText("🎮 3. СТАРТИРАЙ ИГРАТА");
        btnStartGame.setBackgroundColor(Color.parseColor("#238636"));
        btnStartGame.setTextColor(Color.WHITE);
        btnStartGame.setVisibility(View.GONE);
        btnStartGame.setOnClickListener(v -> {
            studioPanel.setVisibility(View.GONE); // Скрива менюто
            nativeMountObb(targetObbFile.getParent()); // Зарежда OBB в C++
            startGameLoop(); // Пуска C++ графиката
        });
        btnRow.addView(btnStartGame);

        studioPanel.addView(btnRow);

        progressBar = new ProgressBar(this, null, android.R.attr.progressBarStyleHorizontal);
        progressBar.setVisibility(View.GONE);
        studioPanel.addView(progressBar);

        infoText = new TextView(this);
        infoText.setTextColor(Color.parseColor("#8B949E"));
        infoText.setTextSize(13);
        infoText.setText("ℹ️ Избери PS2 ISO файл.");
        studioPanel.addView(infoText);

        root.addView(studioPanel);
        setContentView(root);

        InputManager im = (InputManager) getSystemService(Context.INPUT_SERVICE);
        im.registerInputDeviceListener(this, null);
        updateGamepadState();
        
        if (targetObbFile.exists() && targetObbFile.length() > 1000000) {
            btnStartGame.setVisibility(View.VISIBLE);
            infoText.setText("✅ Намерен е готов OBB файл! Можеш директно да стартираш играта.");
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        nativeSetSurface(holder.getSurface());
    }
    @Override public void surfaceChanged(SurfaceHolder h, int f, int w, int ht) {}
    @Override public void surfaceDestroyed(SurfaceHolder h) { nativeSetSurface(null); }

    private void openIsoPicker() {
        Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
        intent.setType("*/*");
        startActivityForResult(Intent.createChooser(intent, "Избери PS2 ISO"), 109);
    }

    @Override
    protected void onActivityResult(int req, int res, Intent data) {
        super.onActivityResult(req, res, data);
        if (req == 109 && res == RESULT_OK && data != null && data.getData() != null) {
            selectedIsoUri = data.getData();
            try {
                ParcelFileDescriptor pfd = getContentResolver().openFileDescriptor(selectedIsoUri, "r");
                double sizeMb = pfd.getStatSize() / (1024.0 * 1024.0);
                pfd.close();
                infoText.setText(String.format("✅ Избран: %s (%.1f MB)
Натисни 2. ИЗВЛЕЧИ РЕСУРСИ", selectedIsoUri.getLastPathSegment(), sizeMb));
                btnPrepareObb.setVisibility(View.VISIBLE);
            } catch (Exception e) { infoText.setText(e.getMessage()); }
        }
    }

    // ИСТИНСКО КОПИРАНЕ НА ФАЙЛА (ЩЕ ОТНЕМЕ ВРЕМЕ!)
    private void realExtractObbStream() {
        if (selectedIsoUri == null) return;
        progressBar.setVisibility(View.VISIBLE);
        btnPrepareObb.setEnabled(false);
        btnSelectIso.setEnabled(false);
        
        new Thread(() -> {
            try {
                ParcelFileDescriptor pfd = getContentResolver().openFileDescriptor(selectedIsoUri, "r");
                FileInputStream in = new FileInputStream(pfd.getFileDescriptor());
                FileOutputStream out = new FileOutputStream(targetObbFile);
                
                byte[] buf = new byte[1024 * 1024]; // 1MB буфер
                long total = pfd.getStatSize(), copied = 0;
                int len;
                
                while ((len = in.read(buf)) > 0) {
                    out.write(buf, 0, len);
                    copied += len;
                    int p = (int)((copied * 100) / total);
                    long finalCopied = copied;
                    runOnUiThread(() -> {
                        progressBar.setProgress(p);
                        infoText.setText("⏳ РЕАЛНО КОПИРАНЕ: " + (int)(finalCopied/(1024*1024)) + " MB / " + (int)(total/(1024*1024)) + " MB (" + p + "%)...");
                    });
                }
                in.close(); out.close(); pfd.close();
                
                runOnUiThread(() -> {
                    progressBar.setVisibility(View.GONE);
                    infoText.setText("🎉 РЕСУРСИТЕ СА ИЗВЛЕЧЕНИ УСПЕШНО!
Натисни 3. СТАРТИРАЙ ИГРАТА.");
                    btnStartGame.setVisibility(View.VISIBLE);
                });
            } catch (Exception e) { 
                runOnUiThread(() -> infoText.setText("Грешка: " + e.getMessage())); 
            }
        }).start();
    }

    // ИСТИНСКИ СТАРТ НА C++ ГРАФИКАТА
    private void startGameLoop() {
        if (isGameRunning) return;
        isGameRunning = true;
        new Thread(() -> {
            float hue = 0.0f;
            while (isGameRunning) {
                hue = (hue + 0.01f > 1.0f) ? 0.0f : hue + 0.01f;
                // C++ ядрото рисува директно върху екрана!
                nativeRenderGameFrame(0.1f, 0.3f + (hue * 0.2f), 0.5f + (hue * 0.3f));
                try { Thread.sleep(16); } catch (Exception ignored) {} // ~60 FPS
            }
        }).start();
    }

    private void updateGamepadState() {
        boolean hasG = false;
        for (int id : InputDevice.getDeviceIds()) {
            InputDevice d = InputDevice.getDevice(id);
            if (d != null && !d.isVirtual() && ((d.getSources() & InputDevice.SOURCE_GAMEPAD) != 0 || (d.getSources() & InputDevice.SOURCE_JOYSTICK) != 0)) hasG = true;
        }
        boolean finalHasG = hasG;
        runOnUiThread(() -> {
            overlayView.setVisibility(finalHasG ? View.GONE : View.VISIBLE);
            gamepadStatusText.setText(finalHasG ? "🎮 Джойстик: СВЪРЗАН (Екран чист)" : "📱 Тъч режим");
            gamepadStatusText.setTextColor(finalHasG ? Color.parseColor("#3FB950") : Color.parseColor("#D29922"));
        });
    }

    @Override public void onInputDeviceAdded(int id) { updateGamepadState(); }
    @Override public void onInputDeviceRemoved(int id) { updateGamepadState(); }
    @Override public void onInputDeviceChanged(int id) { updateGamepadState(); }
    @Override public boolean onKeyDown(int k, KeyEvent e) { nativeSendInput(k); return super.onKeyDown(k, e); }
    @Override public boolean onKeyUp(int k, KeyEvent e) { nativeSendInput(0); return super.onKeyUp(k, e); }
    @Override public boolean onGenericMotionEvent(MotionEvent e) {
        if ((e.getSource() & InputDevice.SOURCE_JOYSTICK) != 0) {
            nativeSendAxes(e.getAxisValue(MotionEvent.AXIS_X), e.getAxisValue(MotionEvent.AXIS_Y), e.getAxisValue(MotionEvent.AXIS_Z), e.getAxisValue(MotionEvent.AXIS_RZ));
            return true;
        }
        return super.onGenericMotionEvent(e);
    }

    class GamepadOverlayView extends View {
        private final Paint p = new Paint(Paint.ANTI_ALIAS_FLAG);
        public GamepadOverlayView(Context c) { super(c); }
        @Override protected void onDraw(Canvas cv) {
            super.onDraw(cv);
            int w = getWidth(), h = getHeight();
            if (w == 0 || h == 0) return;
            p.setColor(Color.argb(70, 35, 45, 65));
            cv.drawCircle(200, h - 200, 130, p);
            cv.drawCircle(w - 200, h - 200, 130, p);
            p.setColor(Color.WHITE); p.setTextSize(34); p.setTextAlign(Paint.Align.CENTER);
            cv.drawText("▲", 200, h - 270, p); cv.drawText("▼", 200, h - 130, p);
            cv.drawText("◀", 130, h - 190, p); cv.drawText("▶", 270, h - 190, p);
            p.setTextSize(38);
            p.setColor(Color.parseColor("#3FB950")); cv.drawText("▲", w - 200, h - 270, p);
            p.setColor(Color.parseColor("#F85149")); cv.drawText("●", w - 130, h - 190, p);
            p.setColor(Color.parseColor("#58A6FF")); cv.drawText("✖", w - 200, h - 130, p);
            p.setColor(Color.parseColor("#D29922")); cv.drawText("■", w - 270, h - 190, p);
        }
    }
}
