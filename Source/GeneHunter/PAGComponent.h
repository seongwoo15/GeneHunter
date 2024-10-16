// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorGenerationStructure.h"
#include "PAGComponent.generated.h"

class UStaticMesh;
class AStaticMeshActor;
class ADiamondSquare;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GENEHUNTER_API UPAGComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPAGComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    // 페어링된 Mesh와 Region을 저장하는 배열
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paired Mesh Regions")
    TArray<FActorGenerationStructure> Assets;
	TArray<float> ApplySoftmax();
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0, ClampMax = 1), Category = "Params")
	float Frequency_Thresh = 1.0f;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0), Category = "Params")
	float AssetDistance = 1.0f;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0, ClampMax = 0.5), Category = "Params")
	float Displacement = 0.0f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void AttachAsset(UStaticMesh* MeshAsset, FVector SpawnVector);
	void RemoveAsset();
	void OrganizeAsset();

private:
	ADiamondSquare* DiamondSquare;
	TArray<AStaticMeshActor*> SpawnedMeshActors;
	int SelectIndexBasedOnProbability(TArray<float>& ProbabilityDistribution);
		
};
