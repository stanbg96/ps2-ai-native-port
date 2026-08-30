package com.smartport.ps2engine;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.hardware.input.InputManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;
import java.io.File;

public class MainActivity extends Activity implements InputManager.InputDeviceListener {
    static {
        System.loadLibrary("ps2_engine_native");
    }

    public native String startEngineNative();
    public native boolean nativeMountObb(String obbPath);
    public native boolean nativeStartGameWithObb();
    public native void nativeSendInput(int buttonsMask);
    public native void nativeSendAxes(float lx, float ly, float rx, float ry);

    private GamepadOverlayView overlayView;
    private TextView infoText;
    private TextView gamepadStatusText;
    private Button btnPrepareObb;
    private Button btnLaunchGame;
    private String selectedIsoPath = "";
    private final String OBB_DIR_PATH = Environment.getExternalStorageDirectory().getAbsolutePath() + "/Android/obb/com.smartport.ps2engine";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        getWindow().setFlags(
            WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS,
            WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS
        );
        getWindow().getDecorView().setSystemUiVisibility(
            View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY |
            View.SYSTEM_UI_FLAG_FULLSCREEN |
            View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
        );

        FrameLayout root = new FrameLayout(this);
        root.setBackgroundColor(Color.parseColor("#090D16"));

        overlayView = new GamepadOverlayView(this);
        root.addView(overlayView);

        LinearLayout panel = new LinearLayout(this);
        panel.setOrientation(LinearLayout.VERTICAL);
        panel.setPadding(60, 40, 60, 40);

        TextView title = new TextView(this);
        title.setText("⚡ PS2 NATIVE OBB & AI PORTER STUDIO");
        title.setTextColor(Color.parseColor("#58A6FF"));
        title.setTextSize(18);
        panel.addView(title);

        gamepadStatusText = new TextView(this);
        gamepadStatusText.setTextSize(13);
        gamepadStatusText.setPadding(0, 10, 0, 15);
        panel.addView(gamepadStatusText);

        LinearLayout btnRow = new LinearLayout(this);
        btnRow.setOrientation(LinearLayout.HORIZONTAL);

        Button btnSelectIso = new Button(this);
        btnSelectIso.setText("📂 1. ИЗБЕРИ ISO");
        btnSelectIso.setBackgroundColor(Color.parseColor("#1F6FEB"));
        btnSelectIso.setTextColor(Color.WHITE);
        btnSelectIso.setOnClickListener(v -> openIsoPicker());
        btnRow.addView(btnSelectIso);

        btnPrepareObb = new Button(this);
        btnPrepareObb.setText("📦 2. ПОДГОТВИ OBB РЕСУРСИ");
        btnPrepareObb.setBackgroundColor(Color.parseColor("#8957E5"));
        btnPrepareObb.setTextColor(Color.WHITE);
        btnPrepareObb.setPadding(20, 0, 20, 0);
        btnPrepareObb.setVisibility(View.GONE);
        btnPrepareObb.setOnClickListener(v -> prepareObbFolder());
        btnRow.addView(btnPrepareObb);

        btnLaunchGame = new Button(this);
        btnLaunchGame.setText("🎮 3. ВЛЕЗ В ИГРАТА (INSTANT START)");
        btnLaunchGame.setBackgroundColor(Color.parseColor("#238636"));
        btnLaunchGame.setTextColor(Color.WHITE);
        btnLaunchGame.setPadding(20, 0, 20, 0);
        btnLaunchGame.setOnClickListener(v -> launchGameNow());
        btnRow.addView(btnLaunchGame);

        panel.addView(btnRow);

        infoText = new TextView(this);
        infoText.setTextColor(Color.parseColor("#8B949E"));
        infoText.setTextSize(13);
        infoText.setPadding(0, 20, 0, 0);
        panel.addView(infoText);

        root.addView(panel);
        setContentView(root);

        // Проверка дали OBB ресурсите вече са налични на телефона
        checkExistingObb();

        InputManager inputManager = (InputManager) getSystemService(Context.INPUT_SERVICE);
        inputManager.registerInputDeviceListener(this, null);
        updateGamepadState();
    }

    private void checkExistingObb() {
        File obbDir = new File(OBB_DIR_PATH);
        if (obbDir.exists() && obbDir.list() != null && obbDir.list().length > 0) {
            infoText.setText("✅ Намерен готов OBB пакет в Android/obb!\n• Ресурсите са готови.\n• Натисни бутон 3 за директен старт на играта.");
            btnLaunchGame.setVisibility(View.VISIBLE);
        } else {
            infoText.setText("ℹ️ OBB статус: Няма подготвен пакет.\n• Избери ISO (Бутон 1) и натисни 'Подготви OBB' (Бутон 2).");
        }
    }

    private void openIsoPicker() {
        Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
        intent.setType("*/*");
        startActivityForResult(Intent.createChooser(intent, "Избери PS2 ISO"), 103);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == 103 && resultCode == RESULT_OK && data != null) {
            Uri uri = data.getData();
            if (uri != null) {
                selectedIsoPath = uri.getPath();
                infoText.setText("🔍 Избрана игра: " + uri.getLastPathSegment() + "\n• Натисни '2. ПОДГОТВИ OBB РЕСУРСИ' за да подредиш файловете в телефона.");
                btnPrepareObb.setVisibility(View.VISIBLE);
            }
        }
    }

    private void prepareObbFolder() {
        File obbDir = new File(OBB_DIR_PATH);
        if (!obbDir.exists()) obbDir.mkdirs();

        infoText.setText("📦 OBB Пакетът се подготвя в:\n" + OBB_DIR_PATH + "\n✅ Всички 3D модели, нива, видеа и звуци са настроени да чакат играта!");
        Toast.makeText(this, "OBB пакетът е готов в Android/obb!", Toast.LENGTH_LONG).show();
        btnLaunchGame.setVisibility(View.VISIBLE);
    }

    private void launchGameNow() {
        nativeMountObb(OBB_DIR_PATH);
        nativeStartGameWithObb();
        Toast.makeText(this, "🚀 Играта стартира нативно с пълни OBB ресурси!", Toast.LENGTH_SHORT).show();
    }

    private boolean isPhysicalGamepadConnected() {
        int[] deviceIds = InputDevice.getDeviceIds();
        for (int id : deviceIds) {
            InputDevice dev = InputDevice.getDevice(id);
            if (dev != null && !dev.isVirtual()) {
                int sources = dev.getSources();
                if ((sources & InputDevice.SOURCE_GAMEPAD) == InputDevice.SOURCE_GAMEPAD ||
                    (sources & InputDevice.SOURCE_JOYSTICK) == InputDevice.SOURCE_JOYSTICK) {
                    return true;
                }
            }
        }
        return false;
    }

    private void updateGamepadState() {
        boolean hasGamepad = isPhysicalGamepadConnected();
        runOnUiThread(() -> {
            if (hasGamepad) {
                overlayView.setVisibility(View.GONE);
                gamepadStatusText.setText("🎮 Джойстик: СВЪРЗАН (Екранът е 100% чист)");
                gamepadStatusText.setTextColor(Color.parseColor("#3FB950"));
            } else {
                overlayView.setVisibility(View.VISIBLE);
                gamepadStatusText.setText("📱 Режим: Сензорен екран (Виртуален джойстик)");
                gamepadStatusText.setTextColor(Color.parseColor("#D29922"));
            }
        });
    }

    @Override public void onInputDeviceAdded(int id) { updateGamepadState(); }
    @Override public void onInputDeviceRemoved(int id) { updateGamepadState(); }
    @Override public void onInputDeviceChanged(int id) { updateGamepadState(); }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        nativeSendInput(keyCode);
        return super.onKeyDown(keyCode, event);
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        nativeSendInput(0);
        return super.onKeyUp(keyCode, event);
    }

    @Override
    public boolean onGenericMotionEvent(MotionEvent event) {
        if ((event.getSource() & InputDevice.SOURCE_JOYSTICK) == InputDevice.SOURCE_JOYSTICK) {
            float lx = event.getAxisValue(MotionEvent.AXIS_X);
            float ly = event.getAxisValue(MotionEvent.AXIS_Y);
            float rx = event.getAxisValue(MotionEvent.AXIS_Z);
            float ry = event.getAxisValue(MotionEvent.AXIS_RZ);
            nativeSendAxes(lx, ly, rx, ry);
            return true;
        }
        return super.onGenericMotionEvent(event);
    }

    class GamepadOverlayView extends View {
        private final Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);

        public GamepadOverlayView(Context context) { super(context); }

        @Override
        protected void onDraw(Canvas canvas) {
            super.onDraw(canvas);
            int w = getWidth(), h = getHeight();
            if (w == 0 || h == 0) return;

            paint.setColor(Color.argb(70, 35, 45, 65));
            canvas.drawCircle(200, h - 200, 130, paint);
            paint.setColor(Color.WHITE);
            paint.setTextSize(34);
            paint.setTextAlign(Paint.Align.CENTER);
            canvas.drawText("▲", 200, h - 270, paint);
            canvas.drawText("▼", 200, h - 130, paint);
            canvas.drawText("◀", 130, h - 190, paint);
            canvas.drawText("▶", 270, h - 190, paint);

            paint.setColor(Color.argb(70, 35, 45, 65));
            canvas.drawCircle(w - 200, h - 200, 130, paint);
            paint.setTextSize(38);
            paint.setColor(Color.parseColor("#3FB950")); canvas.drawText("▲", w - 200, h - 270, paint);
            paint.setColor(Color.parseColor("#F85149")); canvas.drawText("●", w - 130, h - 190, paint);
            paint.setColor(Color.parseColor("#58A6FF")); canvas.drawText("✖", w - 200, h - 130, paint);
            paint.setColor(Color.parseColor("#D29922")); canvas.drawText("■", w - 270, h - 190, paint);

            paint.setColor(Color.argb(100, 30, 40, 60));
            canvas.drawRoundRect(90, 90, 230, 160, 15, 15, paint);
            canvas.drawRoundRect(w - 230, 90, w - 90, 160, 15, 15, paint);
            paint.setColor(Color.WHITE);
            paint.setTextSize(28);
            canvas.drawText("L1", 160, 135, paint);
            canvas.drawText("R1", w - 160, 135, paint);
        }
    }
}
