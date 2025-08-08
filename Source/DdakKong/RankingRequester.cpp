// Fill out your copyright notice in the Description page of Project Settings.


#include "RankingRequester.h"


ARankingRequester::ARankingRequester()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ARankingRequester::RequestTop10Rankings()
{
	// HTTP 요청 객체 생성
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest(); 

	// 응답 오면 호출할 함수 바인딩
	Request->OnProcessRequestComplete().BindUObject(this, &ARankingRequester::OnRankingsReceived);

	// Node.js 서버 주소로 GET 요청 설정
	Request->SetURL("http://localhost:3000/getRankings");
	Request->SetVerb("GET"); 

	// 요청 전송
	Request->ProcessRequest(); 
	UE_LOG(LogTemp, Warning, TEXT("Send Data Request To Ranking Server..."));

}

void ARankingRequester::OnRankingsReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed Data Request. Is Server On?"));
		return; 
	}

	// Json 응답 -> FString
	const FString ResponseStr = Response->GetContentAsString(); 

	// Json 배열 파싱하기 위한 리더 생성
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
	TArray<TSharedPtr<FJsonValue>> JsonArray; 

	// JSON 배열 DeSerialize
	if (FJsonSerializer::Deserialize(Reader, JsonArray))
	{
		TArray<FRankData> ReceivedRankings; 

		// 배열 요소 순회 
		for (const TSharedPtr<FJsonValue>& Value : JsonArray) 
		{
			TSharedPtr<FJsonObject> RankObject = Value->AsObject(); 
			FRankData RankData; 

			// PlayerName, Score 추출해 Struct에 저장
			RankData.PlayerName = RankObject->GetStringField(TEXT("playerName"));
			RankData.Score = RankObject->GetIntegerField(TEXT("score")); 
			ReceivedRankings.Add(RankData); 
		}

		// 파싱된 데이터 확인
		UE_LOG(LogTemp, Warning, TEXT("%d개의 랭킹 데이터 성공적 파싱 완료"), ReceivedRankings.Num());
		for(int32 i = 0; i < ReceivedRankings.Num(); i++) {
			UE_LOG(LogTemp, Log, TEXT("순위 %d: %s - %d점"), i+1, *ReceivedRankings[i].PlayerName, ReceivedRankings[i].Score);
		}

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("JSON 파싱 실패. 응답 데이터 확인 필요"));
	}
}
