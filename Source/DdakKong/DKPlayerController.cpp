// Fill out your copyright notice in the Description page of Project Settings.


#include "DKPlayerController.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

// 입력 받기 -> BaseShooter의 함수 호출로 연결

void ADKPlayerController::BeginPlay()
{
    Super::BeginPlay(); 

    
    // 1. Start 위젯 표시 및 게임 정지
	if (StartWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(this, StartWidgetClass);
		CurrentWidget->AddToViewport();

		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
		SetPause(true);
	} 

    
}


void ADKPlayerController::StartGame(FString InPlayerName)
{
	PlayerName = InPlayerName;

	// 2. Start 위젯 제거, Main 위젯 표시
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
	}
	if (MainWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(this, MainWidgetClass);
		CurrentWidget->AddToViewport();
	}
}

void ADKPlayerController::OnScoreUpdated(FVector TargetLocation)
{
    // 3. 점수 팝업(+10) 위젯 생성
    if (ScorePopupWidgetClass)
    {
        // 월드 좌표를 화면 좌표로 변환
        FVector2D ScreenLocation;
        ProjectWorldLocationToScreen(TargetLocation, ScreenLocation);

        UUserWidget* ScorePopup = CreateWidget<UUserWidget>(this, ScorePopupWidgetClass);
        ScorePopup->SetPositionInViewport(ScreenLocation);
        
		ScorePopup->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));  // 중앙 기준 정렬
		ScorePopup->AddToViewport(20);

		// 타이머 2초 후 제거
        FTimerHandle TimerHandle;
        // 위젯 제거 
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=]()
        {
            if (ScorePopup && ScorePopup->IsInViewport())
            {
                ScorePopup->RemoveFromParent();
            }
        }), 1.0f, false); // 2초 후 한 번 실행
    
    }
}

void ADKPlayerController::HandleGameEnd()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
    }

    SetPause(true);
    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;
    
    // 게임 종료 시점의 점수를 가져와 서버에 갱신 요청
    if (ABaseShooter* MyShooter = Cast<ABaseShooter>(GetPawn()))
    {
        RequestUpdateScore(PlayerName, MyShooter->GetScore());
    }

    if (EndingWidgetClass)
    {
        CurrentWidget = CreateWidget<UUserWidget>(this, EndingWidgetClass);
        CurrentWidget->AddToViewport();
    }
}

// 점수 갱신/저장 요청
void ADKPlayerController::RequestUpdateScore(const FString& InPlayerName, int32 InScore)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetStringField("playerName", InPlayerName);
    JsonObject->SetNumberField("score", InScore);

    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &ADKPlayerController::OnUpdateScoreResponse);
    Request->SetURL("http://localhost:3000/updateScore"); // 수정된 서버 주소
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");
    Request->SetContentAsString(RequestBody);
    Request->ProcessRequest();

    UE_LOG(LogTemp, Warning, TEXT("점수 갱신 요청: %s - %d점"), *InPlayerName, InScore);
}

// 점수 갱신 응답 처리
void ADKPlayerController::OnUpdateScoreResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("점수 갱신 요청 실패. 서버 상태 확인 필요."));
        return;
    }
    UE_LOG(LogTemp, Log, TEXT("점수 갱신 응답: Status %d, Content: %s"), Response->GetResponseCode(), *Response->GetContentAsString());
}


void ADKPlayerController::SendFinalScore()
{
	ABaseShooter* MyShooter = Cast<ABaseShooter>(GetPawn());
	if (!MyShooter) return;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("playerName", PlayerName);
	JsonObject->SetNumberField("score", MyShooter->GetScore()); // BaseShooter에서 점수 가져오기

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL("http://localhost:3000/saveScore"); // Node.js API 주소
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(RequestBody);
	Request->ProcessRequest();
}

// 랭킹 요청
void ADKPlayerController::RequestAndShowRankings()
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &ADKPlayerController::OnRankingsReceived);
    Request->SetURL("http://localhost:3000/getRankings");
    Request->SetVerb("GET");
    Request->ProcessRequest();
    UE_LOG(LogTemp, Warning, TEXT("랭킹 데이터 요청 시작..."));
}

// 랭킹 응답 처리
void ADKPlayerController::OnRankingsReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("랭킹 데이터 요청 실패. 서버 상태 확인 필요."));
        return;
    }

    TArray<FRankData> RankList; // 파싱된 데이터를 담을 배열
    TArray<TSharedPtr<FJsonValue>> JsonArray;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

    if (FJsonSerializer::Deserialize(Reader, JsonArray))
    {
        for (const TSharedPtr<FJsonValue>& Value : JsonArray)
        {
           TSharedPtr<FJsonObject> RankObject = Value->AsObject();
           FRankData Data;
           Data.PlayerName = RankObject->GetStringField("playerName");
           Data.Score = RankObject->GetIntegerField("score");
           RankList.Add(Data);
        }
        
        // [핵심!] 파싱이 성공하면, 블루프린트로 방송(Broadcast)합니다.
        OnRankingsReceived_BP.Broadcast(RankList);
        UE_LOG(LogTemp, Warning, TEXT("데이터 파싱 성공 및 블루프린트로 Broadcast 완료."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("JSON 데이터 파싱 실패."));
    }
}


void ADKPlayerController::RestartGame()
{
	// 7. 게임 재시작
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}