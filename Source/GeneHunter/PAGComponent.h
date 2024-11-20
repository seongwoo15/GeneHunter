// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/StaticMesh.h"
#include "PAGComponent.generated.h"

class AStaticMeshActor;
class ADiamondSquare;

//Structure which saves informations for mesh
USTRUCT(BlueprintType)
struct FActorGenerationStructure
{
    GENERATED_BODY()

public:
	//Static mesh to be attached
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Informations")
    UStaticMesh* Mesh;

	//Frequency of meshes, any float can be the input value
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Informations")
    float Frequency;

	//Depth of Mesh
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Informations")
    float Depth;

	//base rotator of mesh
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Informations")
    FRotator Rotation;

	//size of mesh
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

    // Define mesh as Structure 
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
