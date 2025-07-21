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
		// Main 위젯의 생성자나 애니메이션에서 카운트다운을 시작하고,
		// 카운트다운이 끝나면 SetPause(false), SetInputMode(FInputModeGameOnly)를 호출하세요.
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
        ScorePopup->AddToViewport(); // 생성된 위젯은 애니메이션 후 스스로 사라집니다.
    }
}

void ADKPlayerController::HandleGameEnd()
{
	// 4. 게임 종료: Main 위젯 제거, 점수 전송, Ending 위젯 표시
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
	}

	SetPause(true);
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
	
	SendFinalScore(); // 서버에 점수 전송

	if (EndingWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(this, EndingWidgetClass);
		CurrentWidget->AddToViewport();
	}
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

void ADKPlayerController::RequestAndShowRankings()
{
	// 5. 랭킹 요청
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ADKPlayerController::OnRankingsReceived);
	Request->SetURL("http://localhost:3000/getRankings");
	Request->SetVerb("GET");
	Request->ProcessRequest();
}

void ADKPlayerController::OnRankingsReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid()) return;

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(Reader, JsonArray))
	{
		TArray<FRankData> RankList;
		for (const TSharedPtr<FJsonValue>& Value : JsonArray)
		{
			TSharedPtr<FJsonObject> RankObject = Value->AsObject();
			FRankData Data;
			Data.PlayerName = RankObject->GetStringField("playerName");
			Data.Score = RankObject->GetIntegerField("score");
			RankList.Add(Data);
		}
		DisplayRankingWidget(RankList); // 파싱된 데이터로 위젯 표시 함수 호출
	}
}

void ADKPlayerController::DisplayRankingWidget(const TArray<FRankData>& RankList)
{
    // 6. 랭킹 위젯을 동적으로 생성하고 데이터 채우기
    if (RankingWidgetClass)
    {
        UUserWidget* RankingWidget = CreateWidget<UUserWidget>(this, RankingWidgetClass);
        if (!RankingWidget) return;

        // 랭킹 위젯 블루프린트에서 'RankingListBox' 이름으로 만든 Vertical Box를 찾아야 합니다.
        UVerticalBox* ListBox = Cast<UVerticalBox>(RankingWidget->GetWidgetFromName("RankingListBox"));
        if (ListBox && RankEntryWidgetClass)
        {
            ListBox->ClearChildren(); // 기존 목록 지우기

            for (const FRankData& RankData : RankList)
            {
                // 여기서 'WBP_RankEntry' 위젯을 동적으로 생성하고
                // 텍스트를 RankData 값으로 설정한 뒤
                // ListBox->AddChild()를 통해 자식으로 추가하는 블루프린트 로직이 필요합니다.
                // 이 부분을 블루프린트에서 구현하는 것이 더 유연합니다.
            }
        }
        RankingWidget->AddToViewport();
    }
}


void ADKPlayerController::RestartGame()
{
	// 7. 게임 재시작
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}