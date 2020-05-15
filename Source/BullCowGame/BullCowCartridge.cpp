// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"
//#include "UnrealMathUtility.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
  //  FMath::RandRange(0, 10);
    Super::BeginPlay();
    Dictionary = SortIsogram(Numbers);
    SetupGame();
  

}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{ 
    if (bGameOver) {
        ClearScreen();
        SetupGame();
   }else{
        ProcessGuess(PlayerInput);
    
    }
}

void UBullCowCartridge::SetupGame() {

    PrintLine(TEXT("Welcome to the Bull&Cow!"));
    PrintLine(TEXT("Type something and press ENTER"));
    bGameOver = false;

  //  for (size_t i = 0; i < Dictionary.Num(); i++)
  //  {
  //      PrintLine(TEXT("Words in dictionary: %s "), *Dictionary[i]);
  //  }
    
    HiddenWord = Dictionary[FMath::RandRange(0, Dictionary.Num()-1)];
    PrintLine(*HiddenWord);
    Lives = HiddenWord.Len();

     PrintLine(TEXT("The word contains %i letters"), HiddenWord.Len());
     PrintLine(TEXT("You have %i lives"), Lives);

}

void UBullCowCartridge::EndGame() {
    bGameOver = true;
   // PrintLine(TEXT("You lost!"));
    PrintLine(TEXT("The word was: %s"), *HiddenWord);
    PrintLine(TEXT("Press ENTER to play again"));

}

void UBullCowCartridge::ProcessGuess(const FString& Guess) {
    FBullCowCount Score;
    if (HiddenWord.Len() == Guess.Len()) {
        if (!IsIsogram(Guess))
        {
            PrintLine(TEXT("Your word isn't isogram!"));
           // PrintLine(TEXT("Now you have only %i lives left"), --Lives);
        }
        if (HiddenWord == Guess) {
            PrintLine(TEXT("Correct"));
            bGameOver = true;
            EndGame();
        }
        else {
            PrintLine(TEXT("So sad bro, you didn't guess"));
            PrintLine(TEXT("Now you have only %i lives left"), --Lives);

             Score =  GetBullCows(Guess);
            PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.BullCount, Score.CowCount);

            if (Lives == 0) {
                EndGame();
                return;
            }
        }
    }
    else
    {
        --Lives;
        if (Lives == 0) {
            PrintLine(TEXT("So sad bro, you lost"));
            EndGame();
        }
        else {
            PrintLine(FString::Printf(TEXT("Not the same count of letters, you need to type: %i"), HiddenWord.Len()));
            PrintLine(TEXT("Now you have only %i lives left"), Lives);

            Score =   GetBullCows(Guess);
            PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.BullCount, Score.CowCount);
        }
    }

}

bool UBullCowCartridge::IsIsogram(const FString& Checking) const {
    int32 leng = Checking.Len();
    
    for (size_t i = 0; i < leng; i++)
    {
        for (size_t k = i+1; k < leng; k++)
        {
            if (Checking[i] == Checking[k]) {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::SortIsogram(const TArray<FString>& Dictionary) const{
   // int32 Length = Dictionary.Num();
    TArray<FString> IsoDictionary;
    for (FString Word : Dictionary)
    {
        if (IsIsogram(Word)) {
            IsoDictionary.Emplace(Word);
        }
    }
    return IsoDictionary;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const {
    FBullCowCount Counter{ 0,0 };
    
    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex]) {
            Counter.BullCount++;
            continue;
        }
        for (int32 HiddenWordIndex = 0; HiddenWordIndex < HiddenWord.Len(); HiddenWordIndex++)
        {
            if (HiddenWord[GuessIndex] == Guess[HiddenWordIndex]) {
                Counter.CowCount++;
                break;
            }
        }
        
    }
    return Counter;
}