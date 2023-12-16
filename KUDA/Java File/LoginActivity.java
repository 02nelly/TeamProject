package org.techtown.kuda;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class LoginActivity extends AppCompatActivity {
    public final String useId= "admin";
    public final String userPass = "admin";
    EditText editTextId;
    EditText editTextPass;
    Button loginButton;
    Button joinMemberButton;
    Button findIdButton;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        loginButton = findViewById(R.id.loginButton);

        loginButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                login();
            }
        });
        joinMemberButton = findViewById(R.id.joinMemberButton);
        joinMemberButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(LoginActivity.this , JoinActivity.class);
                startActivity(intent);
            }
        });
        findIdButton = findViewById(R.id.lookIdAndPassButton);
        findIdButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(LoginActivity.this , Search_id_pw_Activity.class);
                startActivity(intent);
            }
        });
    }
    protected  void login(){
        editTextId = findViewById(R.id.editTextId);
        editTextPass = findViewById(R.id.editTextPass);
        String id = editTextId.getText().toString();
        String pass = editTextPass.getText().toString();
        if(useId.equals(id) && userPass.equals(pass)){
            Toast.makeText(this, "로그인 성공!!!", Toast.LENGTH_SHORT).show();
            Intent intent = new Intent(LoginActivity.this , MainActivity.class);
            startActivity(intent);

        }else if(useId.equals(id) && !userPass.equals(pass)){
            Toast.makeText(this, "비밀번호 일치하지않습니다!!", Toast.LENGTH_SHORT).show();
        }else{
            Toast.makeText(this, "아이디 일치하지않습니다", Toast.LENGTH_SHORT).show();
        }
    }
}