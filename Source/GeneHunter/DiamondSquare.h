// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "DiamondSquare.generated.h"


class UMaterialInterface;
class UPAGComponent;

//Generate procedural landscape mesh 
UCLASS()
class GENEHUNTER_API ADiamondSquare : public AActor
{
	GENERATED_BODY()
	
public:	
	ADiamondSquare();


protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere)
	int seed = 60;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	int XSize = 150;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	int YSize = 150;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	float ZMultiplier = 800.0f;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	float NoiseScale = 0.03f;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	float Mountain_Thresh = 0.28f;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = -1))
	float River_Thresh = -0.35f;


	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001))
	float Scale = 150;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001))
	float UVScale = 0.2;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;
	UPROPERTY(EditAnywhere)
	UPAGComponent* PAGComp;
	
public:	
	virtual void Tick(float DeltaTime) override;
	int GetXSize() const { return XSize; }
	int GetYSize() const { return YSize; }
	int GetScale() const { return Scale; }
	std::pair<float, int> get_height(float x, float y);
	
private:
	UProceduralMeshComponent* ProceduralMesh;

	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector2D> UV0;
	TArray<FVector> Normals;
	TArray<struct FProcMeshTangent> Tangents;

	void CreateVertices();
	void CreateTriangles();


};
