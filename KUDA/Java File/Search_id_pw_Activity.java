package org.techtown.kuda;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class Search_id_pw_Activity extends AppCompatActivity {
    
    // edit, button 들 id 바꾸기

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_search_id_pw);



        EditText editText_id_name = findViewById(R.id.editText_id_name);
        EditText editText_id_number = findViewById(R.id.editText_id_number);
        EditText editText_pw_id = findViewById(R.id.editText_pw_id);
        EditText editText_pw_number = findViewById(R.id.editText_pw_number);

        Button button_id = findViewById(R.id.button_id);
        button_id.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (editText_id_name.getText().toString().length()==0) {
                    Toast.makeText(Search_id_pw_Activity.this, "이름을 입력하세요.", Toast.LENGTH_SHORT).show();
                } else if (editText_id_number.getText().toString().length() == 0) {
                    Toast.makeText(Search_id_pw_Activity.this, "전화번호를 입력하세요.", Toast.LENGTH_SHORT).show();
                } else {
                    Intent intent = new Intent(getApplicationContext(), LoginActivity.class);   // 일단 이동할 화면 임시로 해둠
                    startActivity(intent);
                }
            }
        });

        Button button_pw = findViewById(R.id.button_pw);
        button_pw.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (editText_pw_id.getText().toString().length() == 0) {
                    Toast.makeText(Search_id_pw_Activity.this, "ID를 입력하세요.", Toast.LENGTH_SHORT).show();
                } else if (editText_pw_number.getText().toString().length() == 0) {
                    Toast.makeText(Search_id_pw_Activity.this, "전화번호를 입력하세요.", Toast.LENGTH_SHORT).show();
                } else {
                    Intent intent = new Intent(getApplicationContext(), LoginActivity.class);   // 일단 이동할 화면 임시로 해둠
                    startActivity(intent);
                }
            }
        });
    }
}