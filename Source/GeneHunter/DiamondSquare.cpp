

#include "DiamondSquare.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

ADiamondSquare::ADiamondSquare()
{
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
	ProceduralMesh->SetupAttachment(GetRootComponent());

}

void ADiamondSquare::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Vertices.Reset();
	Triangles.Reset();
	UV0.Reset();

	CreateVertices();
	CreateTriangles();

	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UV0, Normals, Tangents);


	ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UV0, TArray<FColor>(), Tangents, true);
	ProceduralMesh->SetMaterial(0, Material);

}

void ADiamondSquare::BeginPlay()
{
	Super::BeginPlay();
	
}


void ADiamondSquare::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ADiamondSquare::get_height(int x, int y)
{
	float Z = FMath::PerlinNoise2D(FVector2D(x * NoiseScale + 0.1, y * NoiseScale + 0.1)) * ZMultiplier;
	Z += FMath::PerlinNoise2D(FVector2D(x * NoiseScale * 3 + 0.1, y * NoiseScale * 3 + 0.1)) * ZMultiplier * 0.2;
	float Mountain_variable = FMath::PerlinNoise2D(FVector2D(x * NoiseScale * 0.5, y * NoiseScale * 0.5));
	if (Mountain_variable > Mountain_Thresh)
	{
		Z += Mountain_variable * ZMultiplier * 30 * (Mountain_variable - Mountain_Thresh);
	}

    return Z;
}

void ADiamondSquare::CreateVertices()
{
	for( int X = 0; X <= XSize; X++)
	{
		for( int Y = 0; Y <= YSize; Y++)
		{

			Vertices.Add(FVector(X * Scale - (XSize * Scale * 0.5), Y * Scale - (YSize * Scale * 0.5), get_height(X,Y)));
			UV0.Add(FVector2D(X * UVScale, Y * UVScale));

			//DrawDebugSphere(GetWorld(), FVector( X * Scale, Y * Scale, 0), 25.0f, 16, FColor::Red, true, -1.0f, 0u, 0.0f);

		}
	}
}

void ADiamondSquare::CreateTriangles()
{
	int Vertex = 0;

	for( int X = 0; X < XSize; ++X)
	{
		for( int Y = 0; Y < YSize; ++Y)
		{
			Triangles.Add(Vertex);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 1);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 2);
			Triangles.Add(Vertex + YSize + 1);

			++Vertex;
		}
		++Vertex;
	}	
}

