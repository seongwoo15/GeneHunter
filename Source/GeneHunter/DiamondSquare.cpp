

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
	float rocky_noise = FMath::PerlinNoise2D(FVector2D(x * NoiseScale * 10 + seed, y * NoiseScale * 10 + seed));
	float field_noise = FMath::PerlinNoise2D(FVector2D(x * NoiseScale * 3 + seed, y * NoiseScale * 3 + seed));
	float landscape_height = FMath::PerlinNoise2D(FVector2D(x * NoiseScale + seed, y * NoiseScale + seed)) + field_noise * 0.3;

    double dx = static_cast<double>(x);
    double dy = static_cast<double>(y);
	double magnitude = std::sqrt(dx * dx + dy * dy);
	double angle = std::atan2(dy, dx);

	float River_noise = FMath::PerlinNoise2D(FVector2D(magnitude * NoiseScale + seed, magnitude * angle * NoiseScale * 0.01 + seed));
	float Mountain_noise = FMath::PerlinNoise2D(FVector2D(x * NoiseScale * 0.25 + seed, y * NoiseScale * 0.25 + seed));
	
	float Z = landscape_height * ZMultiplier;
	//Z += FMath::PerlinNoise2D(FVector2D(x * NoiseScale * 3 + 0.1, y * NoiseScale * 3 + 0.1)) * ZMultiplier * 0.2;
	if(Mountain_noise > Mountain_Thresh)
	{
		Z += Mountain_noise * Mountain_noise * (Mountain_noise - Mountain_Thresh) * ZMultiplier * 110;
		Z += rocky_noise * ZMultiplier * 6 * FMath::Clamp((Mountain_noise - Mountain_Thresh), 0.0f, 0.2f);
	}

	else if(River_noise < River_Thresh)
	{
		Z += River_noise * ZMultiplier * 20 * (River_Thresh - River_noise);
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

