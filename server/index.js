// 필요한 모듈
const express = require('express');
const mysql = require('mysql2/promise'); // mysql2/promise를 사용해 async/await 지원
const cors = require('cors');

// Express 애플리케이션 생성
const app = express();
const port = 3000;

// CORS와 JSON 본문 파싱을 위한 미들웨어 설정
app.use(cors());
app.use(express.json()); // POST 요청의 body를 json으로 파싱하기 위해 필요 

// 데이터베이스 연결 풀 설정 (Promise 기반)
const db = mysql.createPool({
  host: '127.0.0.1',
  port: 3306,
  user: 'root',
  password: 'sotptkd',
  database: 'Ddakkong',
  waitForConnections: true,
  connectionLimit: 10,
  queueLimit: 0
});

// '/getRankings' 경로로 GET 요청이 오면 실행될 핸들러 (Top 5)
app.get('/getRankings', async (req, res) => {
  console.log('[/getRankings] Top 5 랭킹 데이터 요청 받음');
  try {
    const query = "SELECT playerName, score FROM players ORDER BY score DESC LIMIT 5";
    const [results] = await db.query(query);
    console.log('Top 5 랭킹 데이터 성공적으로 조회:', results);
    res.json(results);
  } catch (err) {
    console.error('데이터베이스 쿼리 실패:', err);
    res.status(500).send('서버에서 데이터를 가져오는 데 실패했습니다.');
  }
});

// '/updateScore' 경로로 POST 요청이 오면 실행될 핸들러
app.post('/updateScore', async (req, res) => {
  const { playerName, score } = req.body; // 요청 body에서 playerName과 score 추출
  console.log(`[/updateScore] 점수 갱신 요청 받음: ${playerName} - ${score}점`);

  if (!playerName || score === undefined) {
    return res.status(400).send('playerName과 score가 필요합니다.');
  }

  try {
    // 1. 플레이어가 이미 존재하는지 확인
    const selectQuery = "SELECT score FROM players WHERE playerName = ?";
    const [existingPlayers] = await db.query(selectQuery, [playerName]);

    if (existingPlayers.length > 0) {
      // 2-a. 플레이어가 존재하면, 새 점수가 기존 점수보다 높을 때만 갱신
      const currentScore = existingPlayers[0].score;
      if (score > currentScore) {
        const updateQuery = "UPDATE players SET score = ? WHERE playerName = ?";
        await db.query(updateQuery, [score, playerName]);
        console.log(`${playerName}의 점수 갱신: ${currentScore} -> ${score}`);
        res.status(200).send(`${playerName}의 점수가 ${score}점으로 갱신되었습니다.`);
      } else {
        console.log(`${playerName}의 점수(${score})가 기존 점수(${currentScore})보다 낮아 갱신하지 않음`);
        res.status(200).send('기존 점수가 더 높아 갱신하지 않았습니다.');
      }
    } else {
      // 2-b. 플레이어가 존재하지 않으면 새로 추가
      const insertQuery = "INSERT INTO players (playerName, score) VALUES (?, ?)";
      await db.query(insertQuery, [playerName, score]);
      console.log(`새로운 플레이어 ${playerName} 추가: ${score}점`);
      res.status(201).send(`새로운 플레이어 ${playerName}이(가) ${score}점으로 등록되었습니다.`);
    }
  } catch (err) {
    console.error('점수 갱신/추가 실패:', err);
    res.status(500).send('서버에서 점수를 처리하는 데 실패했습니다.');
  }
});


// 서버 실행
app.listen(port, () => {
  console.log(`랭킹 서버가 http://localhost:${port} 에서 실행 중입니다.`);
});
