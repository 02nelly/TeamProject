package org.techtown.kuda;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

import com.google.android.material.bottomnavigation.BottomNavigationView;

public class MainActivity extends AppCompatActivity {

    ImageView imageView;
    Button button;
    MenuItem menuItem;

    ReciptFragment reciptFragment;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        imageView = findViewById(R.id.imageMyPage);
        imageView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, MyPageActivity.class);
                startActivity(intent);
            }
        });

        reciptFragment = new ReciptFragment();
        getSupportFragmentManager().beginTransaction().replace(R.id.menu_frame_layout, reciptFragment).commit();

        BottomNavigationView bottomNavigationView = findViewById(R.id.menu_bottom_navigation);
        bottomNavigationView.setOnNavigationItemSelectedListener(new BottomNavigationView.OnNavigationItemSelectedListener() {
            @Override
            public boolean onNavigationItemSelected(@NonNull MenuItem item) {
                if (item.getItemId() == R.id.menu_menu) {
                    Intent intent1 = new Intent(MainActivity.this, RefActivity.class);
                    startActivity(intent1);
                    Toast.makeText(MainActivity.this, "냉장고", Toast.LENGTH_LONG).show();
                    return true;
                } else if (item.getItemId() == R.id.menu_home) {
                    Toast.makeText(MainActivity.this, "홈", Toast.LENGTH_LONG).show();
                    return true;
                } else if (item.getItemId() == R.id.menu_board) {
                    Intent intent2 = new Intent(MainActivity.this, NoticeActivity.class);
                    startActivity(intent2);
                    Toast.makeText(MainActivity.this, "게시판", Toast.LENGTH_LONG).show();
                    return true;
                } else if (item.getItemId() == R.id.menu_bookmark) {
                    Intent intent3 = new Intent(MainActivity.this, BookmarkActivity.class);
                    startActivity(intent3);
                    Toast.makeText(MainActivity.this, "즐겨찾기", Toast.LENGTH_LONG).show();
                    return true;
                }
                return false;
            }
        });
    }
}