// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "DiamondSquare.h"
#include "PAGComponent.h"

// Sets default values for this component's properties
UPAGComponent::UPAGComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	AActor* Owner = GetOwner();
	if (Owner)
	{
		DiamondSquare = Cast<ADiamondSquare>(Owner);
		if (DiamondSquare)
		{
			// 원하는 함수 호출
			//DiamondSquare->MyFunction();
		}
	}
}


// Called when the game starts
void UPAGComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UPAGComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPAGComponent::RemoveAsset()
{
	for (AStaticMeshActor* Actor : SpawnedMeshActors)
    {
        if (Actor && IsValid(Actor))
        {
            Actor->Destroy();
        }
    }
    SpawnedMeshActors.Empty();
}

TArray<float> UPAGComponent::ApplySoftmax()
{
    TArray<float> Softmax_Frequency;
    Softmax_Frequency.Reserve(Assets.Num());

    if (Assets.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Assets 배열이 비어 있습니다."));
        return Softmax_Frequency;
    }
    float MaxFrequency = Assets[0].Frequency;
    for (const FActorGenerationStructure& Asset : Assets)
    {
        if (Asset.Frequency > MaxFrequency)
        {
            MaxFrequency = Asset.Frequency;
        }
    }
    float SumExp = 0.0f;
    TArray<float> ExpValues;
    ExpValues.Reserve(Assets.Num());

    for (const FActorGenerationStructure& Asset : Assets)
    {
        float ExpValue = FMath::Exp(Asset.Frequency - MaxFrequency);
        ExpValues.Add(ExpValue);
        SumExp += ExpValue;
    }
    for (const float ExpValue : ExpValues)
    {
        float SoftmaxValue = ExpValue / SumExp;
        Softmax_Frequency.Add(SoftmaxValue);
    }
    return Softmax_Frequency;
}


void UPAGComponent::AttachAsset(UStaticMesh* MeshAsset, FVector SpawnVector, FRotator SpawnRotation)
{
	//FRotator SpawnRotation(0.f,FMath::FRandRange(0.0f, 360.0f), 0.f);
	if (MeshAsset)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = GetOwner()->GetInstigator();

		AStaticMeshActor* MeshActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), SpawnVector, SpawnRotation, SpawnParams);
		if (MeshActor)
		{
			// 스폰된 액터의 Static Mesh를 설정합니다.
			MeshActor->GetStaticMeshComponent()->SetStaticMesh(MeshAsset);
			SpawnedMeshActors.Add(MeshActor);
		}
	}	
}

int UPAGComponent::SelectIndexBasedOnProbability(TArray<float>& ProbabilityDistribution)
{
	float RandomNumber = FMath::FRand();
    float Cumulative = 0.0f;
    for (int Index = 0; Index < ProbabilityDistribution.Num(); ++Index)
    {
        Cumulative += ProbabilityDistribution[Index];
        if (RandomNumber < Cumulative)
        {
            return Index;
        }
    }
	return 0;
}

void UPAGComponent::OrganizeAsset()
{
	if(Assets.Num()>0)
	{
		TArray<float> Softmax_Frequency = ApplySoftmax();
		float Scale = DiamondSquare->GetScale();
		float OriginXScale = DiamondSquare->GetXSize() * Scale;
		float OriginYScale = DiamondSquare->GetYSize() * Scale;
		float XSize = DiamondSquare->GetXSize() / AssetDistance;
		float YSize = DiamondSquare->GetYSize() / AssetDistance;
		float NewScale = Scale * AssetDistance;
		for( float X = 0.5; X < XSize; X++)
		{
			for( float Y = 0.5; Y < YSize; Y++)
			{
				if(FMath::FRand() < Frequency_Thresh)
				{
					float TempX = X + FMath::FRand() * Displacement;
					float TempY = Y + FMath::FRand() * Displacement;
					float NearX = FMath::FloorToInt(TempX * AssetDistance);
					float NearY = FMath::FloorToInt(TempY * AssetDistance);
					FVector Normal =  FVector::CrossProduct(
						FVector((NearX+1)*Scale, NearY*Scale,  DiamondSquare->get_height(NearX+1, NearY).first) - FVector(NearX*Scale, NearY*Scale,  DiamondSquare->get_height(NearX, NearY).first),
						FVector(NearX*Scale, (NearY+1)*Scale,  DiamondSquare->get_height(NearX, NearY+1).first) - FVector(NearX*Scale, NearY*Scale,  DiamondSquare->get_height(NearX, NearY).first)
						);
					auto result = DiamondSquare->get_height(TempX * AssetDistance, TempY * AssetDistance);
					float height = result.first;
					int region_value = result.second;
					int index = SelectIndexBasedOnProbability(Softmax_Frequency);
					if(0 == region_value)
					{
						AttachAsset(
							Assets[index].Mesh, 
							FVector(TempX * NewScale - (OriginXScale * 0.5), TempY * NewScale - (OriginYScale * 0.5), height) + Assets[index].Depth * Normal.GetSafeNormal(),
							FRotationMatrix::MakeFromZ(Normal).Rotator() + Assets[index].Rotation
						);
					}
				}
			}
		}
	}
}
