const express = require('express');
const mysql = require('mysql2/promise'); // promise 기반의 최신 mysql2 사용

const app = express();
const port = 3000; // API 서버가 사용할 포트

// ❗️DB 연결 정보: 본인의 DB 설정에 맞게 수정하세요.
const dbConfig = {
    host: '127.0.0.1',          // DB 호스트 주소 (예: 127.0.0.1)
    user: 'sa',      // DB 사용자 이름
    password: 'sotptkd',  // DB 비밀번호
    database: 'Ddakkong'   // 사용할 데이터베이스 이름
};

// '/getRankings' 주소로 GET 요청이 오면 랭킹 데이터를 보내주는 API
app.get('/getRankings', async (req, res) => {
    try {
        // DB에 연결
        const connection = await mysql.createConnection(dbConfig);
        
        // 점수(score)가 높은 순서(DESC)로 10명만 조회
        const [rows] = await connection.execute(
            'SELECT playerName, score FROM Players ORDER BY score DESC LIMIT 10'
        );
        
        // DB 연결 종료
        await connection.end();
        
        // 성공적으로 데이터를 찾았으면 JSON 형태로 응답
        console.log("랭킹 데이터를 성공적으로 전송했습니다.", rows);
        res.json(rows);

    } catch (error) {
        // 에러 발생 시 서버 로그에 기록하고 500 에러 응답
        console.error('DB 조회 중 에러 발생:', error);
        res.status(500).json({ message: '서버에서 오류가 발생했습니다.' });
    }
});

// 서버 실행
app.listen(port, () => {
    console.log(`✅ 랭킹 서버가 http://localhost:${port} 에서 실행 중입니다.`);
    console.log(`테스트 주소: http://localhost:${port}/getRankings`);
});