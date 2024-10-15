
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "DiamondSquare.generated.h"


class UMaterialInterface;

UCLASS()
class GENEHUNTER_API ADiamondSquare : public AActor
{
	GENERATED_BODY()
	
public:	
	ADiamondSquare();

	UPROPERTY(EditAnywhere)
	int seed = 0;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	int XSize = 0;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	int YSize = 0;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	float ZMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	float NoiseScale = 1.0f;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	float Mountain_Thresh = 0.3f;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = -1))
	float River_Thresh = -0.3f;


	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001))
	float Scale = 0;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001))
	float UVScale = 0;


protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UProceduralMeshComponent* ProceduralMesh;
	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector2D> UV0;
	TArray<FVector> Normals;
	TArray<struct FProcMeshTangent> Tangents;

	void CreateVertices();
	void CreateTriangles();
	float get_height(int x, int y);

};
