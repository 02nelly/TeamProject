package org.techtown.kuda;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import java.util.ArrayList;
import java.util.List;

public class NoticeActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_notice);

        ListView listView = findViewById(R.id.noticeListView1);

        // 데이터 담을 List
        List<String> itemList = new ArrayList<String>();

        // 데이터 추가
        itemList.add("공지글)  " + "내용");
        for (int i=1; i<4; i++) {
            itemList.add(i+"등 인기글)  " + "내용");
        }
        for(int i=4; i<12; i++) {
            itemList.add("유저)  " + "내용");
        }

        // adapter 생성
        ArrayAdapter<String> adapter = new ArrayAdapter(this, android.R.layout.simple_list_item_1, itemList);

        // listView랑 adapter 연결
        listView.setAdapter(adapter);

    }
}