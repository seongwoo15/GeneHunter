// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/StaticMesh.h"
#include "PAGComponent.generated.h"

class AStaticMeshActor;
class ADiamondSquare;

USTRUCT(BlueprintType)
struct FActorGenerationStructure
{
    GENERATED_BODY()

public:
	//붙일 스테틱 매쉬
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Informations")
    UStaticMesh* Mesh;

	//스테틱 매쉬의 빈도를 나타냄, Softmax값 거치기때문에 아무 실수나 넣어도 됨
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Informations")
    float Frequency;

	//매쉬가 땅에 박히는 깊이
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Informations")
    float Depth;

	//매쉬의 기본 회전자
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Informations")
    FRotator Rotation;

	//매쉬의 크기
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Informations")
    float Size;

    FActorGenerationStructure()
        : Mesh(nullptr), Frequency(0.0f), Depth(0.0f), Rotation(0.0f, 0.0f, 0.0f), Size(1.0f)
    {}
};

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

    // 구조체 형식으로 스폰할 매쉬 정의
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorGenerationStructure")
    TArray<FActorGenerationStructure> Assets;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0, ClampMax = 1), Category = "Params")
	float Frequency_Thresh = 1.0f;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0), Category = "Params")
	float AssetDistance = 1.0f;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0, ClampMax = 0.5), Category = "Params")
	float Displacement = 0.0f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void AttachAsset(UStaticMesh* MeshAsset, FVector SpawnVector, FRotator SpawnRotation);
	void RemoveAsset();
	void OrganizeAsset();
	TArray<float> ApplySoftmax();

private:
	ADiamondSquare* DiamondSquare;
	TArray<AStaticMeshActor*> SpawnedMeshActors;
	int SelectIndexBasedOnProbability(TArray<float>& ProbabilityDistribution);
		
};
