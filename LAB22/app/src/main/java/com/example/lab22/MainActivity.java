package com.example.lab22;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.*;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("lab22");
    }

    // JNI methods
    public native String helloNative();
    public native long factorial(int n);
    public native String reverseString(String input);
    public native int sumArray(int[] arr);
    public native int[][] multiplyMatrices(int[][] A, int[][] B);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // 🔥 UTILISER LE XML
        setContentView(R.layout.activity_main);

        // Récupération des composants
        Button btnHello = findViewById(R.id.btnHello);
        Button btnFactorial = findViewById(R.id.btnFactorial);
        Button btnReverse = findViewById(R.id.btnReverse);
        Button btnSum = findViewById(R.id.btnSum);
        Button btnMatrix = findViewById(R.id.btnMatrix);

        TextView result = findViewById(R.id.result);
        EditText inputNumber = findViewById(R.id.inputNumber);
        EditText inputText = findViewById(R.id.inputText);

        // 1) Hello
        btnHello.setOnClickListener(v ->
                result.setText(helloNative())
        );

        // 2) Factoriel
        btnFactorial.setOnClickListener(v -> {
            try {
                int n = Integer.parseInt(inputNumber.getText().toString());
                result.setText("Factoriel = " + factorial(n));
            } catch (Exception e) {
                result.setText("Entrer un nombre valide !");
            }
        });

        // 3) Reverse
        btnReverse.setOnClickListener(v -> {
            String text = inputText.getText().toString();
            result.setText(reverseString(text));
        });

        // 4) Sum
        btnSum.setOnClickListener(v -> {
            int[] arr = {1,2,3,4,5};
            result.setText("Somme = " + sumArray(arr));
        });

        // 5) Matrix
        btnMatrix.setOnClickListener(v -> {
            int[][] A = {{1,2},{3,4}};
            int[][] B = {{5,6},{7,8}};
            int[][] C = multiplyMatrices(A,B);

            String res = "";
            for (int[] row : C) {
                for (int val : row) {
                    res += val + " ";
                }
                res += "\n";
            }
            result.setText(res);
        });
    }
}