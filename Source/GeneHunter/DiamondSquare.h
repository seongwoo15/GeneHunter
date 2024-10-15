
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "DiamondSquare.generated.h"


class UMaterialInterface;
class AStaticMeshActor;
UCLASS()
class GENEHUNTER_API ADiamondSquare : public AActor
{
	GENERATED_BODY()
	
public:	
	ADiamondSquare();

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
	TArray<AStaticMeshActor*> SpawnedMeshActors;

	void CreateVertices();
	void CreateTriangles();
	std::pair<float, int> get_height(float x, float y);
	void attach_asset(FString asset_path, FVector SpawnVector);

};
